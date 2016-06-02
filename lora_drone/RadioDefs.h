/**
 * @file RadioDefs.h
 * <!------------------------------------------------------------------------>
 * @brief @ref REF "Radio Definitions"
 *
 * @par Project:
 * <!------------------------------------------------------------------------>
 * <!--
 * @par Description:
 *
 * This file contains typedefs and defines for every parameter and parameter set
 * that is used within the whole Radio driver section.
 *
 * <!------------------------------------------------------------------------>
 * <!--
 * @remarks
 * - [...]
 * - [...]
 * -->
 * <!------------------------------------------------------------------------
 * Copyright (c) 2013
 * IMST GmbH
 * Carl-Friedrich Gauss Str. 2
 * 47475 Kamp-Lintfort
 * -------------------------------------------------------------------------->
 * @author Tobias Parketny (TPa), IMST
 * <!------------------------------------------------------------------------
 * Target OS:    none
 * Target CPU:   independent
 * Compiler:     IAR C/C++ Compiler
 * -------------------------------------------------------------------------->
 * @internal
 * @par Revision History:
 * <PRE>
 * -----+---------+------+--------+--------------------------------------------
 * VER  |    DATE | TIME | AUTHOR | CHANGES
 * -----+---------+------+--------+--------------------------------------------
 * 0.1  | 27.05.2013     | TPa    | Created
 *
 * </PRE>

 Copyright (c) 2013 IMST GmbH.
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

#ifndef __RadioDefs_H__
#define __RadioDefs_H__

//#include "bsp.h"
#include "inttypes.h"
#include <stdbool.h>

//------------------------------------------------------------------------------
//  Common Transceicer States & Flags
//------------------------------------------------------------------------------

typedef enum
{
    trx_RXDone,
    trx_TXDone,
    trx_ACKDone,
}TRadioFlags;



//------------------------------------------------------------------------------
//  RF Frame Size Definitions
//------------------------------------------------------------------------------


//! maximum radio frame size - FIFO buffer Size, shared for RX and TX
#define RADIO_FRAME_PHY_PAYLOAD_SIZE    (255)

//! radio frame header size with network (group) address byte
//! 1 byte mac control field
//! 1 byte tx group address
//! 2 bytes destination address
//! 1 byte rx group address
//! 2 bytes source address
#define RADIO_FRAME_MAC_HEADER_SIZE     (7)

//! Size of CRC
#define RADIO_FRAME_MAC_CRC_SIZE        (2)

//! radio frame payload size
#define RADIO_FRAME_MAC_PAYLOAD_SIZE    (RADIO_FRAME_PHY_PAYLOAD_SIZE - RADIO_FRAME_MAC_HEADER_SIZE)

//! radio frame minimum size
#define RADIO_FRAME_MIN_SIZE            RADIO_FRAME_MAC_HEADER_SIZE

//! radio frame payload size available on application layer
#define RADIO_FRAME_APP_PAYLOAD_SIZE    RADIO_FRAME_MAC_PAYLOAD_SIZE


// Radio Message
typedef struct RadioMsg_T
{
    //! Length Field
    uint8_t   Length;

    //! Control field
    uint8_t   Ctrl;

    //! Destination Group Address
    uint8_t   DstGroupAddr;

    //! Destination Address
    uint8_t   DstAddr[2];

    //! Source Group Address
    uint8_t   SrcGroupAddr;

    //! Source Address
    uint8_t   SrcAddr[2];

    //! Payload Field with Message dependent content
    uint8_t   Payload[RADIO_FRAME_MAC_PAYLOAD_SIZE];

    //! timestamp
    uint32_t  TimeStamp;

    //! RSSI value
    int16_t   Rssi;

    //! SNR value
    int8_t    Snr;

}TRadioMsg;



//------------------------------------------------------------------------------
// eof
//------------------------------------------------------------------------------
#endif
