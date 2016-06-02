/**
 * @file iM880A_RadioInterface.h
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


#ifndef iM880A_RADIOINTERFACE_H_
#define iM880A_RADIOINTERFACE_H_


//------------------------------------------------------------------------------
//
// Include Files
//
//------------------------------------------------------------------------------
//#include "bsp.h"
#include "inttypes.h"
#include <stdbool.h>
#include "RadioDefs.h"



//------------------------------------------------------------------------------
//
//	Section Defines
//
//------------------------------------------------------------------------------

#define DEVICE_ADDR_LEN      4
#define KEY_LEN             16
#define EUI_LEN              8

//------------------------------------------------------------------------------
//
// General Declaration
//
//------------------------------------------------------------------------------

typedef uint16_t        TWiMODLRResult;

//------------------------------------------------------------------------------
//
// Definition of Result/Error Codes
//
//------------------------------------------------------------------------------

typedef enum
{
    WiMODLR_RESULT_OK = 0,
    WiMODLR_RESULT_PAYLOAD_LENGTH_ERROR,
    WiMODLR_RESULT_PAYLOAD_PTR_ERROR,
    WiMODLR_RESULT_TRANMIT_ERROR
}TWiMDLRResultcodes;


//------------------------------------------------------------------------------
//
// Service Access Point Identifier
//
//------------------------------------------------------------------------------

#define DEVMGMT_ID                          0x01
#define LORAWAN_ID                          0x10

//------------------------------------------------------------------------------
//
// Device Management Message Identifier
//
//------------------------------------------------------------------------------

// Status Codes
#define	DEVMGMT_STATUS_OK					0x00
#define	DEVMGMT_STATUS_ERROR				0x01
#define	DEVMGMT_STATUS_CMD_NOT_SUPPORTED	0x02
#define	DEVMGMT_STATUS_WRONG_PARAMETER		0x03
#define DEVMGMT_STATUS_WRONG_DEVICE_MODE    0x04

// Message IDs
#define DEVMGMT_MSG_PING_REQ                0x01
#define DEVMGMT_MSG_PING_RSP                0x02

#define DEVMGMT_MSG_GET_DEVICEINFO_REQ      0x03
#define DEVMGMT_MSG_GET_DEVICEINFO_RSP      0x04

#define DEVMGMT_MSG_GET_FW_VERSION_REQ      0x05
#define DEVMGMT_MSG_GET_FW_VERSION_RSP      0x06


//------------------------------------------------------------------------------
//
// LoRaWAN Endpoint Message Identifier
//
//------------------------------------------------------------------------------

// Status Codes
#define LORAWAN_STATUS_OK	                0x00	// Operation successful
#define LORAWAN_STATUS_ERROR	            0x01	// Operation failed
#define LORAWAN_STATUS_CMD_NOT_SUPPORTED	0x02	// Command is not supported
#define LORAWAN_STATUS_WRONG_PARAMETER	    0x03	// HCI message contains wrong parameter
#define LORAWAN_STATUS_DEVICE_NOT_ACTIVATED	0x05	// Device is not activated
#define LORAWAN_STATUS_DEVICE_BUSY	        0x06	// Device is busy
#define LORAWAN_STATUS_LENGTH_ERROR	        0x08	// Radio packet length invalid

// Message IDs
#define LORAWAN_MSG_ACTIVATE_DEVICE_REQ	    0x01
#define LORAWAN_MSG_ACTIVATE_DEVICE_RSP	    0x02
#define LORAWAN_MSG_SET_JOIN_PARAM_REQ	    0x05
#define LORAWAN_MSG_SET_JOIN_PARAM_RSP	    0x06
#define LORAWAN_MSG_JOIN_NETWORK_REQ	    0x09
#define LORAWAN_MSG_JOIN_NETWORK_RSP	    0x0A
#define LORAWAN_MSG_JOIN_NETWORK_TX_IND	    0x0B
#define LORAWAN_MSG_JOIN_NETWORK_IND	    0x0C
#define LORAWAN_MSG_SEND_UDATA_REQ	        0x0D
#define LORAWAN_MSG_SEND_UDATA_RSP	        0x0E
#define LORAWAN_MSG_SEND_UDATA_TX_IND	    0x0F
#define LORAWAN_MSG_RECV_UDATA_IND	        0x10
#define LORAWAN_MSG_SEND_CDATA_REQ	        0x11
#define LORAWAN_MSG_SEND_CDATA_RSP	        0x12
#define LORAWAN_MSG_SEND_CDATA_TX_IND	    0x13
#define LORAWAN_MSG_RECV_CDATA_IND	        0x14
#define LORAWAN_MSG_RECV_ACK_IND	        0x15
#define LORAWAN_MSG_GET_STATUS_REQ	        0x17
#define LORAWAN_MSG_GET_STATUS_RSP	        0x18




//------------------------------------------------------------------------------
//
// HCI Message Declaration
//
//------------------------------------------------------------------------------

// message header size: 2 bytes for SapID + MsgID
#define WIMODLR_HCI_MSG_HEADER_SIZE	 	2

// message payload size
#define WIMODLR_HCI_MSG_PAYLOAD_SIZE	300

// frame check sequence field size: 2 bytes for CRC16
#define WIMODLR_HCI_MSG_FCS_SIZE		2



#define LORA_MAC_PORT                   0x02




// visible max. buffer size for lower SLIP layer
#define WIMODLR_HCI_RX_MESSAGE_SIZE (WIMODLR_HCI_MSG_HEADER_SIZE\
                                    +WIMODLR_HCI_MSG_PAYLOAD_SIZE\
                                    +WIMODLR_HCI_MSG_FCS_SIZE)


//------------------------------------------------------------------------------
//
// public available Data Types
//
//------------------------------------------------------------------------------


typedef struct
{
    // Payload Length Information, not transmitted over UART interface !
    uint16_t  Length;

    // Service Access Point Identifier
    uint8_t   SapID;

    // Message Identifier
    uint8_t   MsgID;

    // Payload Field
    uint8_t   Payload[WIMODLR_HCI_MSG_PAYLOAD_SIZE];

    // Frame Check Sequence Field
    uint8_t   CRC16[WIMODLR_HCI_MSG_FCS_SIZE];

}TWiMODLR_HCIMessage;




typedef void (*TRadioInterfaceCbMsgIndication)      (uint8_t* msg, uint8_t length, TRadioFlags trxFlags);
typedef void (*TRadioInterfaceCbLoRaWANHCIResponse) (uint8_t msgID, uint8_t* msg, uint8_t length);
typedef void (*TRadioInterfaceCbDevMgmtHCIResponse) (uint8_t msgID, uint8_t* msg, uint8_t length);


//------------------------------------------------------------------------------
//
//  Section Prototypes
//
//------------------------------------------------------------------------------

void 
iM880A_Init(void);

void 
iM880A_RegisterRadioCallbacks(TRadioInterfaceCbMsgIndication        cbMsgInd,
                              TRadioInterfaceCbLoRaWANHCIResponse   cbLoRaWANHCIRsp,
                              TRadioInterfaceCbDevMgmtHCIResponse   cbDevMgmtHCIRsp);

TWiMODLRResult 
iM880A_PingRequest();

TWiMDLRResultcodes 
iM880A_DirectDeviceActivation(uint32_t deviceAddress, uint8_t* nwkSessionKey, uint8_t* appSessionKey);

TWiMDLRResultcodes
iM880A_SetJoinParameters(uint8_t* appEUI, uint8_t* deviceEUI, uint8_t* deviceKey);

TWiMDLRResultcodes
iM880A_JoinNetworkRequest(void);

TWiMDLRResultcodes
iM880A_SendUDataTelegram(uint8_t* payload, uint16_t length);

TWiMDLRResultcodes
iM880A_SendCDataTelegram(uint8_t* payload, uint16_t length);



#endif
