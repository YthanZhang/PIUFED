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


#define RX_COUNT_MAX 10
#define TX_COUNT_MAX 10


static void txSendBit(piu_SimUART* simUART)
{
    switch (simUART->txCounter)
    {
    case (0): {    // Start bit
        simUART->setTxFunc(0);
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
        simUART->setTxFunc(0x01 & simUART->txBuffer);
        simUART->txBuffer = simUART->txBuffer >> 1;
        break;
    }
    case (9): {    // Stop bit
        simUART->setTxFunc(1);
        break;
    }
    case (10): {    // Stop bit and set flag
        simUART->setTxFunc(1);
        simUART->flag_txComplete = true;
        break;
    }
    default:    // no tx required;
        return;
    }

    ++(simUART->txCounter);
}

static void rxReceiveBit(piu_SimUART* simUART, bool rxVal)
{
    switch (simUART->rxCounter)
    {
    case (0): {    // Check start bit
        if (rxVal)
        {
            // move rx counter to default + 1 to stop receive
            simUART->rxCounter = RX_COUNT_MAX + 1;
        }
        else
        {
            // else start service, reset rx buffer
            simUART->flag_rxComplete = false;
            simUART->rxBuffer        = 0;
        }
        break;
    }
    case (1):    // Receive bits
    case (2):
    case (3):
    case (4):
    case (5):
    case (6):
    case (7):
    case (8): {
        simUART->rxBuffer = simUART->rxBuffer |
                            (rxVal << (simUART->rxCounter - 1));
        break;
    }
    case (10):
        if (rxVal)  // stop bit 2 correct
        {
            simUART->flag_rxComplete   = true;
            simUART->flag_rxFrameErr   = false;
        }
        // no break
    case (9):
        if (!rxVal) // stop bit 1 or 2 incorrect
        {
            simUART->rxCounter         = RX_COUNT_MAX + 1;
            simUART->flag_rxFrameErr   = true;
            simUART->flag_rxComplete   = false;
        }
        break;
    default: {
        return;
    }
    }

    ++(simUART->rxCounter);
}


piu_SimUART* piu_SimUART_construct(piu_SimUART* simUART,
                                   void (*setTxFunc)(bool))
{
    simUART->rxCounter = RX_COUNT_MAX + 1;
    simUART->txCounter = TX_COUNT_MAX + 1;

    simUART->flag_rxFrameErr   = false;
    simUART->flag_rxComplete   = false;
    simUART->flag_txComplete   = true;

    simUART->rxBuffer = 0;
    simUART->txBuffer = 0;

    simUART->setTxFunc = setTxFunc;

    return simUART;
}


bool piu_SimUART_GPIOUpdate(piu_SimUART* simUART, bool rxVal)
{
    if (rxVal == 0)
    {
        simUART->rxCounter = 0;
    }
    else
    {
        return false;
    }

    rxReceiveBit(simUART, rxVal);

    return simUART->rxCounter <= RX_COUNT_MAX;
}


bool piu_SimUART_TIMUpdate(piu_SimUART* simUART, bool rxVal)
{
    if (simUART->rxCounter <= RX_COUNT_MAX)
    {
        rxReceiveBit(simUART, rxVal);
    }
    if (simUART->txCounter <= TX_COUNT_MAX)
    {
        txSendBit(simUART);
    }

    return simUART->rxCounter <= RX_COUNT_MAX ||
           simUART->txCounter <= TX_COUNT_MAX;
}


uint8_t piu_SimUART_getRx(piu_SimUART* simUART)
{
    if(simUART->flag_rxFrameErr)
    {
        return 0;
    }
    simUART->flag_rxComplete = false;
    return simUART->rxBuffer;
}

bool piu_SimUART_sendTx(piu_SimUART* simUART, uint8_t val)
{
    if (simUART->flag_txComplete)
    {
        simUART->flag_txComplete = false;
        simUART->txBuffer        = val;
        simUART->txCounter       = 0;
        return true;
    }
    return false;
}


bool piu_SimUART_getRxComplete(piu_SimUART* simUART)
{
    return simUART->flag_rxComplete;
}
bool piu_SimUART_getTxComplete(piu_SimUART* simUART)
{
    return simUART->flag_txComplete;
}
bool piu_SimUART_getRxFrameErr(piu_SimUART* simUART)
{
    return simUART->flag_rxFrameErr;
}
