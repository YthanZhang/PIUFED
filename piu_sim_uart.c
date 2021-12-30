//
// Created by YthanZhang on 2021/12/30.
//
/*******************************************************************************
 * Copyright 2021 Ythan(Ethan) Zhang
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 * This software component is licensed by Ythan Zhang under the
 *                           BSD-2-Clause license.
 * You may not use this file except in compliance with the license.
 * You may obtain a copy of the License at: opensource.org/licenses/BSD-2-Clause
 ******************************************************************************/


#include "piu_sim_uart.h"


uint8_t simRxBuffer = 0;
uint8_t simTxBuffer = 0;


static uint8_t rxCounter = 0;
static uint8_t txCounter = 0;

bool flag_rxComplete = false;
bool flag_txComplete = false;


void txSendBit(void (*setTxFunc)(bool))
{
    switch (txCounter)
    {
    case (0): {    // Start bit
        setTxFunc(0);
        break;
    }
    case (1):    // Data bits
    case (2):
    case (3):
    case (4):
    case (5):
    case (6):
    case (7):
    case (8): {
        setTxFunc(0x01 & simTxBuffer);
        simTxBuffer = simTxBuffer >> 1;
        break;
    }
    case (9): {    // Stop bit
        setTxFunc(1);
        break;
    }
    case (10): {    // Stop bit and set flag
        setTxFunc(1);
        flag_txComplete = true;
        break;
    }
    default:    // no tx required;
        return;
    }

    ++txCounter;
}

void rxReceiveBit(bool rxVal)
{
    switch (rxCounter)
    {
    case (0): {    // Check start bit
        if (rxVal)
        {
            // move rx counter to default to stop receive
            rxCounter = 8;
        }
        break;
    }
    case (1):    // Receive bits
    case (2):
    case (3):
    case (4):
    case (5):
    case (6):
    case (7): {
        simRxBuffer = (simRxBuffer << 1) | rxVal;
        break;
    }
    case (8): {
        simRxBuffer     = (simRxBuffer << 1) | rxVal;
        flag_rxComplete = true;
        break;
    }
    default: {
        return;
    }
    }

    ++rxCounter;
}


bool simUART_GPIOUpdate(bool rxVal)
{
    if (rxVal == 0)
    {
        rxCounter = 0;
    }
    else
    {
        return false;
    }

    rxReceiveBit(0);

    return rxCounter <= 8;
}


bool simUART_TIMUpdate(bool rxVal, void (*setTxFunc)(bool))
{
    if (rxCounter <= 8)
    {
        rxReceiveBit(0);
    }
    if (txCounter <= 10)
    {
        txSendBit(setTxFunc);
    }

    return rxCounter <= 8 || txCounter <= 10;
}
