/* * OSS-7 - An opensource implementation of the DASH7 Alliance Protocol for ultra
 * lowpower wireless sensor communication
 *
 * Copyright 2015 University of Antwerp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// #include "hwleds.h"
// #include "hwsystem.h"
// #include "scheduler.h"
// #include "timer.h"
// #include "assert.h"
// #include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

// #include "hwadc.h"
// 
// #include "d7ap_stack.h"
// #include "fs.h"

#include "hwuart.h"
// #if (!defined PLATFORM_EFM32GG_STK3700 && !defined PLATFORM_EFM32HG_STK3400 && !defined PLATFORM_OCTA_GATEWAY)
// 	#error Mismatch between the configured platform and the actual platform.
// #endif

// #include "userbutton.h"
// HCI wrapper file
#include "iM880A_RadioInterface.h"
#include "CRC16.h"
// #include "console.h"
// #include "em_gpio.h"
// #include "em_cmu.h"
#define SENSOR_UPDATE	TIMER_TICKS_PER_SEC * 1
#define SENDALP
//#define PASSTROUGH

//------------------------------------------------------------------------------
//
//	Section Defines
//
//------------------------------------------------------------------------------

#define TX_LENGTH                       16
#define POWERUP_DELAY                   500         // ms
#define TX_DELAY                        10000       // ms
#define DEVICE_ADDR                     0x4F430002


// select activation method
//
#define ACTIVATION_METHOD_DIRECT        1
//#define ACTIVATION_METHOD_OTA           1

// select telegram type
//
//#define UNCONFIRMED_DATA_TELEGRAMS      1
#define CONFIRMED_DATA_TELEGRAMS        1

#define MODULE_D7AP_FIFO_COMMAND_BUFFER_SIZE 100
#define ALP_CMD_HANDLER_ID 'D'

//------------------------------------------------------------------------------
//
//	Section Typedefs
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//
//	Section RAM
//
//------------------------------------------------------------------------------

static uint8_t txBuffer[TX_LENGTH] = {0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF };

#if defined(ACTIVATION_METHOD_DIRECT)

// Direct Activation Parameters

static uint8_t nwkSessionKey[KEY_LEN] = {0xB0, 0x6C, 0x24, 0x9B, 0xEE, 0x6D, 0xE2, 0x5A, 0x92, 0x41, 0xD5, 0xF6, 0x16, 0x51, 0x78, 0xA7 };

static uint8_t appSessionKey[KEY_LEN] = {0x94, 0xA6, 0xA3, 0x62, 0x8D, 0xC3, 0x54, 0x28, 0x4A, 0x73, 0xDB, 0xFF, 0x80, 0x77, 0x51, 0x76 };

#else

// OTA Parameters

static uint8_t appEUI[EUI_LEN]        = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22 };

static uint8_t deviceEUI[EUI_LEN]     = {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28 };

static uint8_t deviceKey[KEY_LEN]     = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };

#endif

//------------------------------------------------------------------------------
//
//	CbDevMgmtHCIResponse
//
//------------------------------------------------------------------------------
//!
//! @brief: Handle HCI response messages of SAP DEVMGMT_ID
//!
//------------------------------------------------------------------------------
static void
CbDevMgmtHCIResponse(uint8_t          msgID,
                     uint8_t*         msg,
                     uint8_t          length)
{
    switch(msgID)
    {
        case    DEVMGMT_MSG_PING_RSP:
                // handle ping response here
                break;

        default:
                // handle unsupported MsgIDs here
                break;
    }
}

//------------------------------------------------------------------------------
//
//	CbMsgIndication
//
//------------------------------------------------------------------------------
//!
//! @brief: Handle TX/RX/ACK radio messages
//!
//------------------------------------------------------------------------------
static void
CbMsgIndication(uint8_t*          msg,
                uint8_t           length,
                TRadioFlags     trxFlags)
{
    if(trxFlags == trx_RXDone)
    {
        // Radio Msg received
		// FIXME
		//mainEvent |= MSG_RECEIVED;
		printf("msg received");
    }
    else if (trxFlags == trx_TXDone)
    {
        // TX was successfull
        // FIXME
		//mainEvent |= MSG_SENT;
		printf("msg sent");
    }
    else if (trxFlags == trx_ACKDone)
    {
        // Ack received
        //mainEvent |= ACK_RECEIVED;
		printf("ack received");
    }
}

//------------------------------------------------------------------------------
//
//	CbLoRaWANHCIResponse
//
//------------------------------------------------------------------------------
//!
//! @brief: Handle HCI response messages of SAP LORAWAN_ID
//!
//------------------------------------------------------------------------------
static void
CbLoRaWANHCIResponse(uint8_t          msgID,
                     uint8_t*         msg,
                     uint8_t          length)
{
    switch(msgID)
    {
        case    LORAWAN_MSG_ACTIVATE_DEVICE_RSP:
                break;

        case    LORAWAN_MSG_SET_JOIN_PARAM_RSP:
                break;

        case    LORAWAN_MSG_SEND_UDATA_RSP:
                break;

        case    LORAWAN_MSG_SEND_CDATA_RSP:
                break;

        case    LORAWAN_MSG_GET_STATUS_RSP:
                break;

        default:
                // handle unsupported MsgIDs here
                break;
    }
}




void send_message()
{
   
    //LoraWAN
#if defined(UNCONFIRMED_DATA_TELEGRAMS)

        // Unreliable Data Transmission
        if(iM880A_SendUDataTelegram(txBuffer, TX_LENGTH) != WiMODLR_RESULT_OK)
               ; // handle faults

#elif defined(CONFIRMED_DATA_TELEGRAMS)

        // Confirmed Data Transmission
        if(iM880A_SendCDataTelegram(txBuffer, TX_LENGTH) != WiMODLR_RESULT_OK)
               ; // handle faults

#endif
}



void iM880A_setup()
{
	// Test connection
	  iM880A_PingRequest();

	#if defined(ACTIVATION_METHOD_DIRECT)

	    // Direct Device Activation
	    //
	    iM880A_DirectDeviceActivation(DEVICE_ADDR, nwkSessionKey, appSessionKey);

	#else

	    // Wireless Network Activation (OTA)
	    //
	    iM880A_SetJoinParameters(appEUI, deviceEUI, deviceKey);

	    iM880A_JoinNetworkRequest();

	#endif

}

void bootstrap()
{
	// Initialize radio driver
    iM880A_Init();

	// Register callback functions for receive / send
    iM880A_RegisterRadioCallbacks(CbMsgIndication, CbLoRaWANHCIResponse, CbDevMgmtHCIResponse);

	// whatever the timer and scheduler did before, they pull in a lot of deps
    iM880A_setup();
    //timer_post_task_delay(&iM880A_setup, TIMER_TICKS_PER_SEC * 1);


}

int main(int argc, char** argv) {
	bootstrap();
	printf("-------------- after bootstrap ---------------");
	printf("result of send: %i\n", iM880A_SendCDataTelegram((uint8_t*) "Test", 4));
	// "Test" as integers looks like this: 84, 101, 115, 116
	uart_check_input(lora);
}



