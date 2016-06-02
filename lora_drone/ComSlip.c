/**
 * @file ComSlip.c
 * <!------------------------------------------------------------------------>
 * @brief @ref REF "SLIP Driver Implementation"
 *
 * @par Project:
 * <!------------------------------------------------------------------------>
 * <!--
 * @par Description:
 *
 * Implementation of SLIP Driver
 * <!------------------------------------------------------------------------>
 * <!--
 * @remarks
 * - [...]
 * - [...]
 * -->
 * <!------------------------------------------------------------------------
 * Copyright (c) 2009
 * IMST GmbH
 * Carl-Friedrich Gauss Str. 2
 * 47475 Kamp-Lintfort
 * -------------------------------------------------------------------------->
 * @author Kai vorm Walde (KvW), IMST
 * <!------------------------------------------------------------------------
 * Target OS:    independent
 * Target CPU:   independent
 * Compiler:     IAR C/C++ Compiler
 * -------------------------------------------------------------------------->
 * @internal
 * @par Revision History:
 * <PRE>
 * ---------------------------------------------------------------------------
 * Version | Date       | Author | Comment
 * ---------------------------------------------------------------------------
 * 0.1     | 12.05.2009 | KvW    | Created
 * 0.2     | 14.09.2011 | KvW    | cleanup
 *
 * </PRE>

 Copyright (c) 2009 IMST GmbH.
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

//------------------------------------------------------------------------------
//
//  Include Files
//
//------------------------------------------------------------------------------

#include "ComSlip.h"
#include "hwuart.h"
//------------------------------------------------------------------------------
//
//  Protocol Definitions
//
//------------------------------------------------------------------------------

// SLIP Protocol Characters
#define SLIP_END					0xC0
#define	SLIP_ESC					0xDB
#define	SLIP_ESC_END				0xDC
#define	SLIP_ESC_ESC				0xDD

// SLIP Receiver/Decoder States
#define SLIPDEC_IDLE_STATE          0
#define	SLIPDEC_START_STATE			1
#define	SLIPDEC_IN_FRAME_STATE		2
#define	SLIPDEC_ESC_STATE			3


uart_handle_t* lora;

typedef struct
{
    // upper layer client
    TComSlipCbByteIndication    RxIndication;
    
    // receiver/decoder state
    int                         RxState;

    // rx buffer index for next decodedrxByte
    uint16_t                      RxIndex;

    // size of RxBuffer
    uint16_t                      RxBufferSize;

    // pointer to RxBuffer
    uint8_t*                      RxBuffer;
}TComSlip;


TComSlip ComSlip;

//------------------------------------------------------------------------------
//
//  ComSlip_Init
//
//------------------------------------------------------------------------------
void
ComSlip_Init()
{
    // init to idle state, no rx-buffer avaliable
    ComSlip.RxState         =   SLIPDEC_IDLE_STATE;
    ComSlip.RxIndex         =   0;
    ComSlip.RxBuffer        =   0;
    ComSlip.RxBufferSize    =   0;
    
    // Init Uart Interfaces 
    //LDDUsart_Init();
    
    // Register ComSlip_ProcessRxByte at LDDUART
    //LDDUsart_RegisterClient(ComSlip_ProcessRxByte);
    //TODO: Make this setting for all supported devices
//#if defined(PLATFORM_OCTA_GATEWAY)
    //lora = uart_init(0, 115200, 0);//P1
    lora = uart_init(1, 115200, 3); //P4
//#elif defined(PLATFORM_EFM32HG_STK3400)
//    lora = uart_init(1, 115200, 4);
//#else
//	#error NO LORA MODULE SUPPORTED
//#endif
	uart_enable(lora);
	uart_set_rx_interrupt_callback(lora, &ComSlip_ProcessRxByte);
	uart_rx_interrupt_enable(lora);
}

//------------------------------------------------------------------------------
//
//  RegisterClient
//
//  @brief: register upper layer client
//
//------------------------------------------------------------------------------

void
ComSlip_RegisterClient(TComSlipCbByteIndication cbRxIndication)
{
    ComSlip.RxIndication = cbRxIndication;
}

//------------------------------------------------------------------------------
//
//  StoreRxByte
//
//  @brief: store SLIP decoded rxByte
//
//------------------------------------------------------------------------------

void
ComSlip_StoreRxByte(uint8_t rxByte)
{
    if (ComSlip.RxIndex < ComSlip.RxBufferSize)
        ComSlip.RxBuffer[ComSlip.RxIndex++] = rxByte;
}

//------------------------------------------------------------------------------
//
//  ProcessRxMsg
//
//  @brief: forward decoded SLIP message to upper layer and get new rx-buffer
//
//------------------------------------------------------------------------------

bool
ComSlip_ProcessRxMsg()
{
    // forward received message to upper layer and allocate new rx-buffer
    if(ComSlip.RxIndication)
    {
        (*ComSlip.RxIndication)(ComSlip.RxBuffer, ComSlip.RxIndex);
    }

    // new buffer avaliable ?
    if (ComSlip.RxBuffer != 0)
    {
        // yes, keep receiver enabled
        return true;
    }

    // no further rx-buffer, stop receiver/decoder
    return false;
}

//------------------------------------------------------------------------------
//
//  SendMessage
//
//  @brief: send a message as SLIP frame
//
//------------------------------------------------------------------------------

bool
ComSlip_SendMessage(uint8_t* msg, uint16_t msgLength)
{
    // send start of SLIP message
	uart_send_byte(lora, SLIP_END);

    // iterate over all message bytes
    while(msgLength--)
    {
        switch (*msg)
        {
            case SLIP_END:
            	uart_send_byte(lora, SLIP_ESC);
            	uart_send_byte(lora, SLIP_ESC_END);
                break;

            case SLIP_ESC:
            	uart_send_byte(lora, SLIP_ESC);
            	uart_send_byte(lora, SLIP_ESC_ESC);
                break;

            default:
            	uart_send_byte(lora,*msg);
                break;
        }
        // next byte
        msg++;
    }

    // send end of SLIP message
    uart_send_byte(lora, SLIP_END);

    // always ok
    return true;
}

//------------------------------------------------------------------------------
//
//  SetRxBuffer
//
//  @brief: configure a rx-buffer and enable receiver/decoder
//
//------------------------------------------------------------------------------

bool
ComSlip_SetRxBuffer(uint8_t* rxBuffer, uint16_t  rxBufferSize)
{
    // receiver in IDLE state and client already registered ?
    if((ComSlip.RxState == SLIPDEC_IDLE_STATE) && ComSlip.RxIndication)
    {
        // same buffer params
        ComSlip.RxBuffer        = rxBuffer;
        ComSlip.RxBufferSize    = rxBufferSize;

        // enable decoder
        ComSlip.RxState = SLIPDEC_START_STATE;

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
//
//  ProcessRxData
//
//  @brief: process received byte stream
//------------------------------------------------------------------------------

void
ComSlip_ProcessRxByte(uint8_t rxData)
{
    // get rxByte
    uint8_t rxByte = rxData;

    // decode according to current state
    switch(ComSlip.RxState)
    {
        case    SLIPDEC_START_STATE:
                // start of SLIP frame ?
                if(rxByte == SLIP_END)
                {
                    // init read index
                    ComSlip.RxIndex = 0;

                    // next state
                    ComSlip.RxState = SLIPDEC_IN_FRAME_STATE;
                }
                break;

        case    SLIPDEC_IN_FRAME_STATE:
                switch(rxByte)
                {
                    case    SLIP_END:
                            // data received ?
                            if(ComSlip.RxIndex > 0)
                            {
                                // yes, process rx message, get new buffer
                                if (ComSlip_ProcessRxMsg())
                                {
                                    ComSlip.RxState = SLIPDEC_START_STATE;
                                }
                                else
                                {
                                    // disable decoder, temp. no buffer avaliable
                                    ComSlip.RxState = SLIPDEC_IDLE_STATE;
                                }
                            }
                            // init read index
                            ComSlip.RxIndex = 0;
                            break;

                    case  SLIP_ESC:
                            // enter escape sequence state
                            ComSlip.RxState = SLIPDEC_ESC_STATE;
                            break;

                    default:
                            // store byte
                            ComSlip_StoreRxByte(rxByte);
                            break;
                }
                break;

        case    SLIPDEC_ESC_STATE:
                switch(rxByte)
                {
                    case    SLIP_ESC_END:
                            ComSlip_StoreRxByte(SLIP_END);
                            // quit escape sequence state
                            ComSlip.RxState = SLIPDEC_IN_FRAME_STATE;
                            break;

                    case    SLIP_ESC_ESC:
                            ComSlip_StoreRxByte(SLIP_ESC);
                            // quit escape sequence state
                            ComSlip.RxState = SLIPDEC_IN_FRAME_STATE;
                            break;

                    default:
                            // abort frame receiption
                            ComSlip.RxState = SLIPDEC_START_STATE;
                            break;
                }
                break;

        default:
                break;
    }
}

//------------------------------------------------------------------------------
// end of file
//------------------------------------------------------------------------------
