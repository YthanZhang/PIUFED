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


#include "piu_vtimer.h"


piu_VTimer* piu_VTimer_construct(piu_VTimer* vTimer,
                                 uint16_t counterReloadValue,
                                 piu_VTCDir counterDirection,
                                 piu_VTMode timerMode,
                                 void (*callbackFunc)(void))
{
    vTimer->counter            = counterReloadValue;
    vTimer->counterReloadValue = counterReloadValue;
    vTimer->countDirection     = counterDirection;

    vTimer->timerMode = timerMode;

    vTimer->callback = callbackFunc;

    vTimer->flag_overflow      = false;
    vTimer->flag_overOverflow  = false;
    vTimer->flag_counterActive = false;

    return vTimer;
}


void piu_VTimer_tick(piu_VTimer* vTimer)
{
    // only tick if counter is active
    if (!vTimer->flag_counterActive)
    {
        return;
    }

    uint16_t oldCount = vTimer->counter;
    bool overflow     = false;
    if (vTimer->countDirection == piu_VTCDir_Up)
    {
        ++(vTimer->counter);
        // If less than old count, an overflow happened
        overflow = vTimer->counter < oldCount;
    }
    else if (vTimer->countDirection == piu_VTCDir_Down)
    {
        --(vTimer->counter);
        // If greater than old count, an overflow happened
        overflow = vTimer->counter > oldCount;
    }

    if (overflow)
    {
        if (vTimer->flag_overflow)
        {
            vTimer->flag_overOverflow = true;
        }
        vTimer->flag_overflow = true;

        vTimer->counter = vTimer->counterReloadValue;
        
        if (vTimer->timerMode == piu_VTMode_OneShot)
        {
            vTimer->flag_counterActive = false;
        }
        
        if(vTimer->callback != NULL)
        {
            vTimer->callback();
        }
    }
}


uint16_t piu_VTimer_setCounterReloadValue(piu_VTimer* vTimer,
                                          uint16_t counterReloadValue)
{
    return vTimer->counterReloadValue = counterReloadValue;
}

piu_VTCDir piu_VTimer_setCountDirection(piu_VTimer* vTimer,
                                        piu_VTCDir countDirection)
{
    if(!vTimer->flag_counterActive)
    {
        vTimer->countDirection = countDirection;

        piu_VTimer_reloadCounter(vTimer);
    }
    
    return vTimer->countDirection;
}

piu_VTMode piu_VTimer_setTimerMode(piu_VTimer* vTimer, piu_VTMode timerMode)
{
    if(!vTimer->flag_counterActive)
    {
        vTimer->timerMode = timerMode;
    }
    
    return vTimer->timerMode;
}

void piu_VTimer_setCallback(piu_VTimer* vTimer, void (*callbackFunc)(void))
{
    if(!vTimer->flag_counterActive)
    {
        vTimer->callback = callbackFunc;
    }
}


uint16_t piu_VTimer_startCounter(piu_VTimer* vTimer)
{
    uint16_t counter = vTimer->counter;
    vTimer->flag_counterActive = true;
    return counter;
}

uint16_t piu_VTimer_stopCounter(piu_VTimer* vTimer)
{
    vTimer->flag_counterActive = false;
    return vTimer->counter;
}

uint16_t piu_VTimer_reloadCounter(piu_VTimer* vTimer)
{
    return vTimer->counter = vTimer->counterReloadValue;
}

uint16_t piu_VTimer_getCounter(piu_VTimer* vTimer)
{
    return vTimer->counter;
}

uint16_t piu_VTimer_getCounterReloadValue(piu_VTimer* vTimer)
{
    return vTimer->counterReloadValue;
}

piu_VTCDir piu_VTimer_getCountDirection(piu_VTimer* vTimer)
{
    return vTimer->countDirection;
}


bool piu_VTimer_getOverflow(piu_VTimer* vTimer)
{
    bool flag = vTimer->flag_overflow;
    vTimer->flag_overflow = false;
    return flag;
}

bool piu_VTimer_getOverOverflow(piu_VTimer* vTimer)
{
    return vTimer->flag_overOverflow;
}

bool piu_VTimer_getCounterActive(piu_VTimer* vTimer)
{
    return vTimer->flag_counterActive;
}


bool piu_VTimer_clearOverOverflow(piu_VTimer* vTimer)
{
    bool flag = vTimer->flag_overOverflow;
    vTimer->flag_overOverflow = false;
    return flag;
}
