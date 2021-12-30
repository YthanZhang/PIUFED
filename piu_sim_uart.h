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


#ifndef PIU_SIM_UART_H
#define PIU_SIM_UART_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stdint.h>


typedef struct piu_struct_SimUART
{
    bool flag_rxComplete;
    bool flag_txComplete;

    bool flag_rxFrameError;

    uint8_t rxBuffer;
    uint8_t txBuffer;

    uint8_t rxCounter;
    uint8_t txCounter;

    void (*setTxFunc)(bool txVal);
} piu_SimUART;


piu_SimUART* piu_SimUART_construct(piu_SimUART* simUART,
                                   void (*setTxFunc)(bool));

bool piu_SimUART_TIMUpdate(piu_SimUART* simUART, bool rxVal);
bool piu_SimUART_GPIOUpdate(piu_SimUART* simUART, bool rxVal);

uint8_t piu_SimUART_getRx(piu_SimUART* simUART);
void piu_SimUART_sendTx(piu_SimUART* simUART, uint8_t val);


#ifdef __cplusplus
}
#endif

#endif    // PIU_SIM_UART_H
