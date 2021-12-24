//
// Created by YthanZhang on 2021/12/20.
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


#ifndef PLATFORM_INDEPENDENT_UTILS_FOR_EMBEDDED_DEVELOPMENT_PIU_VTIMER_H
#define PLATFORM_INDEPENDENT_UTILS_FOR_EMBEDDED_DEVELOPMENT_PIU_VTIMER_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stdint.h>


typedef enum piu_enum_VTMode
{
    piu_VTMode_OneShot,
    piu_VTMode_Continuous,
} piu_VTMode;


typedef enum piu_enum_VTCDir
{
    piu_VTCDir_Up,
    piu_VTCDir_Down,
} piu_VTCDir;


typedef struct piu_struct_VTimer
{
    void (*callback)(void);

    piu_VTMode timerMode;

    uint16_t counterReloadValue;
    uint16_t counter;
    piu_VTCDir counterDirection;

    bool flag_overflow;
    bool flag_overOverflow;
    bool flag_counterActive;
} piu_VTimer;


/**
 *
 * @param vTimer
 * @param counterReloadValue
 * @param counterDirection
 * @param timerMode
 * @param callbackFunc
 * @return
 */
piu_VTimer* piu_VTimer_construct(piu_VTimer* vTimer,
                                 uint16_t counterReloadValue,
                                 piu_VTCDir counterDirection,
                                 piu_VTMode timerMode,
                                 void (*callbackFunc)(void));

void piu_VTimer_tick(piu_VTimer* vTimer);

uint16_t piu_VTimer_setCounterReloadValue(piu_VTimer* vTimer,
                                          uint16_t newCounterReloadValue);
piu_VTCDir piu_VTimer_setCountDirection(piu_VTimer* vTimer,
                                        piu_VTCDir countDirection);
piu_VTMode piu_VTimer_setTimerMode(piu_VTimer* vTimer, piu_VTMode timerMode);
void piu_VTimer_setCallback(piu_VTimer* vTimer, void (*callbackFunc)(void));

uint16_t piu_VTimer_startCounter(piu_VTimer* vTimer);
uint16_t piu_VTimer_stopCounter(piu_VTimer* vTimer);
uint16_t piu_VTimer_reloadCounter(piu_VTimer* vTimer);
uint16_t piu_VTimer_getCounter(piu_VTimer* vTimer);

bool piu_VTimer_getOverflow(piu_VTimer* vTimer);
bool piu_VTimer_getOverOverflow(piu_VTimer* vTimer);
bool piu_VTimer_getCounterActive(piu_VTimer* vTimer);



#ifdef __cplusplus
}
#endif

#endif    // PLATFORM_INDEPENDENT_UTILS_FOR_EMBEDDED_DEVELOPMENT_PIU_VTIMER_H
