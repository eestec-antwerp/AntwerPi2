/**
 * @file iM880A_RadioInterface.c
 * <!------------------------------------------------------------------------>
 * @brief @ref REF "WiMODLR HCI Example"
 *
 * @par Project:
 * <!------------------------------------------------------------------------>
 * <!--
 * @par Description:
 *
 *  [Description]
 * -->
 * <!--
 *  @ref [extdocname] "more..."
 *  -->
 * <!------------------------------------------------------------------------>
 * <!--
 * @remarks
 * - [...]
 * - [...]
 * -->
 * <!------------------------------------------------------------------------
 * Copyright (c) 2015
 * IMST GmbH
 * Carl-Friedrich Gauss Str. 2
 * 47475 Kamp-Lintfort
 * -------------------------------------------------------------------------->
 * @author Mathias Hater (MH), IMST
 * <!------------------------------------------------------------------------
 * Target OS:    none
 * Target CPU:   EFM32
 * Compiler:     IAR C/C++ Compiler
 * -------------------------------------------------------------------------->
 * @internal
 * @par Revision History:
 * <PRE>
 * ---------------------------------------------------------------------------
 * Version | Date       | Author | Comment
 * ---------------------------------------------------------------------------
 * 0.1     | 22.01.2015 | MH     | Created
 *
 * </PRE>

 Copyright (c) 2015 IMST GmbH.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are NOT permitted without prior written permission
 of the IMST GmbH.

 THIS SOFTWARE IS PROVIDED BY THE IMST GMBH AND CONTRIBUTORS ``AS IS'' AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE IMST GMBH OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 THIS SOFTWARE IS CLASSIFIED AS: CONFIDENTIAL

 *******************************************************************************/

#include "CRC16.h"
#include "RadioDefs.h"
#include "iM880A_RadioInterface.h"

#define DEST_ADDR_SIZE          3

#define NULL ((char *)0)
//------------------------------------------------------------------------------
// Integer Conversion Macros
//------------------------------------------------------------------------------

#define HINIBBLE(x)         ((uint8_t) (((uint8_t) (x)) >> 4))
#define LONIBBLE(x)         ((uint8_t) (((uint8_t) (x)) & 0xF))
#define HIBYTE(x)           ((uint8_t) (((uint16_t)(x)) >> 8))
#define LOBYTE(x)           ((uint8_t) (((uint16_t)(x)) & 0xFF))
#define HIWORD(x)           ((uint16_t)(((uint32_t)(x)) >> 16))
#define LOWORD(x)           ((uint16_t)(((uint32_t)(x)) & 0xFFFF))

#define MAKEBYTE(lo, hi)    ((uint8_t) (((uint8_t) (lo)) | (((uint8_t) ((uint8_t) (hi))) << 4)))
#define MAKEWORD(lo, hi)    ((uint16_t)(((uint8_t) (lo)) | (((uint16_t)((uint8_t) (hi))) << 8)))
#define MAKEDWORD(lo, hi)   ((uint32_t)(((uint16_t)(lo)) | (((uint32_t)((uint16_t)(hi))) << 16)))

#define SWAPBYTE(x)         MAKEWORD(HIBYTE(x), LOBYTE(x))

//------------------------------------------------------------------------------
// Byte-Order Conversion Macros
//------------------------------------------------------------------------------

#define NTOH16(srcPtr)  MAKEWORD((srcPtr)[0], (srcPtr)[1])

#define HTON16(dstPtr, value)           \
        (dstPtr)[0] = LOBYTE(value);    \
        (dstPtr)[1] = HIBYTE(value);

#define NTOH32(srcPtr)  MAKEDWORD(MAKEWORD((srcPtr)[0], (srcPtr)[1]),MAKEWORD((srcPtr)[2], (srcPtr)[3]))

#define HTON32(dstPtr, value)                   \
        (dstPtr)[0] = LOBYTE(LOWORD(value));    \
        (dstPtr)[1] = HIBYTE(LOWORD(value));    \
        (dstPtr)[2] = LOBYTE(HIWORD(value));    \
        (dstPtr)[3] = HIBYTE(HIWORD(value));


//------------------------------------------------------------------------------
//
//	Section RAM
//
//------------------------------------------------------------------------------

TWiMODLR_HCIMessage TxMessage;
TWiMODLR_HCIMessage RxMessage;


TRadioInterfaceCbMsgIndication          cbMsgIndication;
TRadioInterfaceCbLoRaWANHCIResponse     cbLoRaWANHCIResponse;
TRadioInterfaceCbDevMgmtHCIResponse     cbDevMgmtHCIResponse;




//------------------------------------------------------------------------------
//
//  SendHCIMessage
//
//  @brief  Send a HCI message to the radio module (with or without payload)
//
//------------------------------------------------------------------------------

int
iM880A_SendHCIMessage(uint8_t sapID, uint8_t msgID, uint8_t* payload, uint16_t length)
{
    // 1. check parameter
    //uart_handle_t
    // 1.1 check length
    //
    printf("send_hci: call with sapID=%u, msgID=%u, payload=%s, len=%u\n", sapID, msgID, payload, length);
    if(length > WIMODLR_HCI_MSG_PAYLOAD_SIZE)
    {
        return WiMODLR_RESULT_PAYLOAD_LENGTH_ERROR;
    }

    // 2.  init TxMessage
    //
    // 2.1 init SAP ID
    //
    TxMessage.SapID = sapID;

    // 2.2 init Msg ID
    //
    TxMessage.MsgID = msgID;

    // 2.3 copy payload, if present
    //
    if(payload && length)
    {
        uint8_t*  dstPtr  = TxMessage.Payload;
        int     n       = (int)length;

        // copy bytes
        while(n--)
            *dstPtr++ = *payload++;
    }

    // 3. Calculate CRC16 over header and optional payload
    //
    uint16_t crc16 = CRC16_Calc(&TxMessage.SapID, length + WIMODLR_HCI_MSG_HEADER_SIZE, CRC16_INIT_VALUE);

    // 3.1 get 1's complement
    //
    crc16 = ~crc16;

    // 3.2 attach CRC16 and correct length, lobyte first
    //
    TxMessage.Payload[length++] = LOBYTE(crc16);
    TxMessage.Payload[length++] = HIBYTE(crc16);

    // 4. forward message to SLIP layer
    //    - start transmission with SAP ID
    //    - correct length by header size
    
    printf("  ... prepare to comslip\n");
    
    if (ComSlip_SendMessage(&TxMessage.SapID, length + WIMODLR_HCI_MSG_HEADER_SIZE))
    {
        // ok !
        printf("  ... OK!\n");
        return WiMODLR_RESULT_OK;
    }
    printf("  ... error :(\n");
    // error - SLIP layer couldn't sent message
    return -1;
}





//------------------------------------------------------------------------------
//
//  iM880A_SendUDataTelegram
//
//  @brief  Send out an unreliable (unconfirmed) data telegram
//
//------------------------------------------------------------------------------

TWiMDLRResultcodes
iM880A_SendUDataTelegram(uint8_t* payload, uint16_t length)
{
    TxMessage.Payload[0] = LORA_MAC_PORT;

    if(payload && length)
    {
        uint8_t*  dstPtr  = TxMessage.Payload + 1;
        int     n       = (int)length;

        // copy bytes
        while(n--)
            *dstPtr++ = *payload++;
    }

    return (TWiMDLRResultcodes) iM880A_SendHCIMessage(LORAWAN_ID, LORAWAN_MSG_SEND_UDATA_REQ, NULL, length + 1);
}



//------------------------------------------------------------------------------
//
//  iM880A_SendCDataTelegram
//
//  @brief  Send out an confirmed data telegram
//
//------------------------------------------------------------------------------

TWiMDLRResultcodes
iM880A_SendCDataTelegram(uint8_t* payload, uint16_t length)
{
    TxMessage.Payload[0] = LORA_MAC_PORT;
    if(payload && length)
    {
        uint8_t*  dstPtr  = TxMessage.Payload + 1;
        int     n       = (int)length;
        
        printf("send cdata, length = %i\n", n);
        
        // copy bytes
        while(n--)
            *dstPtr++ = *payload++;
    }

    return (TWiMDLRResultcodes) iM880A_SendHCIMessage(LORAWAN_ID, LORAWAN_MSG_SEND_CDATA_REQ, NULL, length + 1);
}


//------------------------------------------------------------------------------
//
//  iM880A_DirectDeviceActivation
//
//  @brief  Perform Direct Activation
//
//------------------------------------------------------------------------------

TWiMDLRResultcodes
iM880A_DirectDeviceActivation(uint32_t deviceAddress, uint8_t* nwkSessionKey, uint8_t* appSessionKey)
{
    HTON32(&TxMessage.Payload[0], deviceAddress);

    if(nwkSessionKey)
    {
        uint8_t*  dstPtr  = TxMessage.Payload + DEVICE_ADDR_LEN;
        int     n       = KEY_LEN;

        // copy bytes
        while(n--)
            *dstPtr++ = *nwkSessionKey++;
    }

    if(appSessionKey)
    {
        uint8_t*  dstPtr  = TxMessage.Payload + DEVICE_ADDR_LEN + KEY_LEN;
        int     n       = KEY_LEN;

        // copy bytes
        while(n--)
            *dstPtr++ = *appSessionKey++;
    }
    
    return (TWiMDLRResultcodes) iM880A_SendHCIMessage(LORAWAN_ID, LORAWAN_MSG_ACTIVATE_DEVICE_REQ, NULL, DEVICE_ADDR_LEN + KEY_LEN + KEY_LEN);
}



//------------------------------------------------------------------------------
//
//  iM880A_SetJoinParameters
//
//  @brief  Configure the Join parameters
//
//------------------------------------------------------------------------------

TWiMDLRResultcodes
iM880A_SetJoinParameters(uint8_t* appEUI, uint8_t* deviceEUI, uint8_t* deviceKey)
{

    if(appEUI)
    {
        uint8_t*  dstPtr  = TxMessage.Payload;
        int     n       = EUI_LEN;

        // copy bytes
        while(n--)
            *dstPtr++ = *appEUI++;
    }

    if(deviceEUI)
    {
        uint8_t*  dstPtr  = TxMessage.Payload + EUI_LEN;
        int     n       = EUI_LEN;

        // copy bytes
        while(n--)
            *dstPtr++ = *deviceEUI++;
    }

    if(deviceKey)
    {
        uint8_t*  dstPtr  = TxMessage.Payload + EUI_LEN + EUI_LEN;
        int     n       = KEY_LEN;

        // copy bytes
        while(n--)
            *dstPtr++ = *deviceKey++;
    }

    return (TWiMDLRResultcodes) iM880A_SendHCIMessage(LORAWAN_ID, LORAWAN_MSG_SET_JOIN_PARAM_REQ, NULL, EUI_LEN + EUI_LEN + KEY_LEN);
}



//------------------------------------------------------------------------------
//
//  iM880A_JoinNetworkRequest
//
//  @brief   Send out a Join Request message
//
//------------------------------------------------------------------------------

TWiMDLRResultcodes
iM880A_JoinNetworkRequest(void)
{
    return (TWiMDLRResultcodes) iM880A_SendHCIMessage(LORAWAN_ID, LORAWAN_MSG_JOIN_NETWORK_REQ, NULL, 0);
}



//------------------------------------------------------------------------------
//
//  iM880A_PingRequest
//
//  @brief  Send ping to check communication link
//
//------------------------------------------------------------------------------

TWiMODLRResult
iM880A_PingRequest()
{
    return (TWiMDLRResultcodes) iM880A_SendHCIMessage(DEVMGMT_ID, DEVMGMT_MSG_PING_REQ, NULL, 0);
}



//------------------------------------------------------------------------------
//
//  iM880A_CbProcessRxMessage
//
//  @brief: Handle incoming HCI messages
//
//------------------------------------------------------------------------------

uint8_t*
iM880A_CbProcessRxMessage(uint8_t* rxBuffer, uint16_t length)
{
    // 1. check CRC
    if (CRC16_Check(rxBuffer, length, CRC16_INIT_VALUE))
    {
        // 2. check min length, 2 bytes for SapID + MsgID + 2 bytes CRC16
        if(length >= (WIMODLR_HCI_MSG_HEADER_SIZE + WIMODLR_HCI_MSG_FCS_SIZE))
        {
            // add length
            RxMessage.Length = length - (WIMODLR_HCI_MSG_HEADER_SIZE + WIMODLR_HCI_MSG_FCS_SIZE);

            // dispatch completed RxMessage
            // 3. forward message according to SapID / MsgID
            switch(RxMessage.SapID)
            {
                case    DEVMGMT_ID:

                        // forward Msg IDs to application, e.g. ping response
                        (*cbLoRaWANHCIResponse)(RxMessage.MsgID, RxMessage.Payload, length);

                        break;

                case    LORAWAN_ID:
                        // handle TX indications
                        if((RxMessage.MsgID == LORAWAN_MSG_SEND_UDATA_TX_IND)   ||
                           (RxMessage.MsgID == LORAWAN_MSG_SEND_CDATA_TX_IND)   ||
                           (RxMessage.MsgID == LORAWAN_MSG_JOIN_NETWORK_TX_IND))
                        {
                            (*cbMsgIndication)(NULL, 0, trx_TXDone);
                        }
                        // handle RX messages
                        else if((RxMessage.MsgID == LORAWAN_MSG_RECV_UDATA_IND) ||
                                (RxMessage.MsgID == LORAWAN_MSG_RECV_CDATA_IND))
                        {
                            (*cbMsgIndication)(RxMessage.Payload, length, trx_RXDone);
                        }

                        // handle ACKs
                        else if(RxMessage.MsgID == LORAWAN_MSG_RECV_ACK_IND)
                        {
                            (*cbMsgIndication)(NULL, 0, trx_ACKDone);
                        }

                        // handle other responses
                        else
                        {
                            (*cbLoRaWANHCIResponse)(RxMessage.MsgID, RxMessage.Payload, length);
                        }

                        break;

                default:
                        // handle unsupported SapIDs here
                        break;
            }
        }
    }
    else
    {
        // handle CRC error
    }

    // return same buffer again, keep receiver enabled
    return &RxMessage.SapID;

}


//------------------------------------------------------------------------------
//
//  iM880A_Init
//
//  @brief: Initialize radio interface
//
//------------------------------------------------------------------------------

void
iM880A_Init(void)
{
    // Init Slip Layer
    ComSlip_Init();
    ComSlip_RegisterClient(iM880A_CbProcessRxMessage);

    // pass first RxBuffer and enable receiver/decoder
    ComSlip_SetRxBuffer(&RxMessage.SapID, (uint16_t)WIMODLR_HCI_RX_MESSAGE_SIZE);
}


//------------------------------------------------------------------------------
//
//  iM880A_RegisterRadioCallbacks
//
//  @brief: Set callback functions for incoming HCI messages
//
//------------------------------------------------------------------------------

void
iM880A_RegisterRadioCallbacks(TRadioInterfaceCbMsgIndication        cbMsgInd,
                              TRadioInterfaceCbLoRaWANHCIResponse   cbLoRaWANHCIRsp,
                              TRadioInterfaceCbDevMgmtHCIResponse   cbDevMgmtHCIRsp)
{
    cbMsgIndication      = cbMsgInd;
    cbLoRaWANHCIResponse = cbLoRaWANHCIRsp;
    cbDevMgmtHCIResponse = cbDevMgmtHCIRsp;
}

