//
// Created by YthanZhang on 2021/12/27.
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


#include "piu_margined_linear.h"

#include <stdbool.h>


static float calcLinear(uint16_t highFlatVal,
                        uint16_t lowFlatVal,
                        uint16_t highLinearPoint,
                        uint16_t lowLinearPoint)
{
    return (float)(highFlatVal - lowFlatVal) /
           (float)(highLinearPoint - lowLinearPoint);
}


piu_MarginedLinear* piu_MarginedLinear_construct(
    piu_MarginedLinear* marginedLinear,
    uint16_t offPoint,
    uint16_t onPoint,
    uint16_t lowLinearPoint,
    uint16_t highLinearPoint,
    uint16_t stepUpPoint,
    uint16_t stepDownPoint,
    uint16_t offVal,
    uint16_t lowFlatVal,
    uint16_t highFlatVal,
    uint16_t maxFlatVal)
{
    marginedLinear->currentState = piu_MarginState_Off;

    marginedLinear->offPoint        = offPoint;
    marginedLinear->onPoint         = onPoint;
    marginedLinear->lowLinearPoint  = lowLinearPoint;
    marginedLinear->highLinearPoint = highLinearPoint;
    marginedLinear->stepUpPoint     = stepUpPoint;
    marginedLinear->stepDownPoint   = stepDownPoint;

    marginedLinear->offVal      = offVal;
    marginedLinear->lowFlatVal  = lowFlatVal;
    marginedLinear->highFlatVal = highFlatVal;
    marginedLinear->maxFlatVal  = maxFlatVal;

    marginedLinear->linearRate =
        calcLinear(highFlatVal, lowFlatVal, highLinearPoint, lowLinearPoint);

    marginedLinear->lastInput = 0;
    
    return marginedLinear;
}


uint16_t piu_MarginedLinear_input(piu_MarginedLinear* marginedLinear,
                                  uint16_t inputVal)
{
    marginedLinear->lastInput = inputVal;

    while (true)
    {
        switch (marginedLinear->currentState)
        {
        case (piu_MarginState_Off): {
            if (inputVal > marginedLinear->onPoint)
            {
                marginedLinear->currentState = piu_MarginState_LowFlat;
                continue;
            }
            break;
        }
        case (piu_MarginState_LowFlat): {
            if (inputVal > marginedLinear->lowLinearPoint)
            {
                marginedLinear->currentState = piu_MarginState_Linear;
                continue;
            }
            else if (inputVal <= marginedLinear->offPoint)
            {
                marginedLinear->currentState = piu_MarginState_Off;
                continue;
            }
            break;
        }
        case (piu_MarginState_Linear): {
            if (inputVal > marginedLinear->highLinearPoint)
            {
                marginedLinear->currentState = piu_MarginState_HighFlat;
                continue;
            }
            else if (inputVal <= marginedLinear->lowLinearPoint)
            {
                marginedLinear->currentState = piu_MarginState_LowFlat;
                continue;
            }
            break;
        }
        case (piu_MarginState_HighFlat): {
            if (inputVal > marginedLinear->stepUpPoint)
            {
                marginedLinear->currentState = piu_MarginState_MaxFlat;
                continue;
            }
            else if (inputVal <= marginedLinear->highLinearPoint)
            {
                marginedLinear->currentState = piu_MarginState_Linear;
                continue;
            }
            break;
        }
        case (piu_MarginState_MaxFlat): {
            if (inputVal <= marginedLinear->stepDownPoint)
            {
                marginedLinear->currentState = piu_MarginState_HighFlat;
                continue;
            }
            break;
        }
        default: {
            marginedLinear->currentState = piu_MarginState_Off;
            break;
        }
        }
        break;
    }

    return piu_MarginedLinear_output(marginedLinear);
}

uint16_t piu_MarginedLinear_output(piu_MarginedLinear* marginedLinear)
{
    switch (marginedLinear->currentState)
    {
    case (piu_MarginState_Off): {
        return marginedLinear->offVal;
    }
    case (piu_MarginState_LowFlat): {
        return marginedLinear->lowFlatVal;
    }
    case (piu_MarginState_Linear): {
        return (uint16_t)((marginedLinear->linearRate *
                           (float)marginedLinear->lastInput) +
                          (float)marginedLinear->lowFlatVal);
    }
    case (piu_MarginState_HighFlat): {
        return marginedLinear->highFlatVal;
    }
    case (piu_MarginState_MaxFlat): {
        return marginedLinear->maxFlatVal;
    }
    default: {
        return marginedLinear->offVal;
    }
    }
}

uint16_t piu_MarginedLinear_updateOffPoint(piu_MarginedLinear* marginedLinear,
                                           uint16_t newOffPoint)
{
    return marginedLinear->offPoint = newOffPoint;
}
uint16_t piu_MarginedLinear_updateOnPoint(piu_MarginedLinear* marginedLinear,
                                          uint16_t newOnPoint)
{
    return marginedLinear->onPoint = newOnPoint;
}
uint16_t piu_MarginedLinear_updateLowLinearPoint(
    piu_MarginedLinear* marginedLinear,
    uint16_t newLowLinearPoint)
{
    marginedLinear->lowLinearPoint = newLowLinearPoint;

    marginedLinear->linearRate = calcLinear(marginedLinear->highFlatVal,
                                            marginedLinear->lowFlatVal,
                                            marginedLinear->highLinearPoint,
                                            marginedLinear->lowLinearPoint);

    return marginedLinear->lowLinearPoint;
}
uint16_t piu_MarginedLinear_updateHighLinearPoint(
    piu_MarginedLinear* marginedLinear,
    uint16_t newHighLinearPoint)
{
    marginedLinear->highLinearPoint = newHighLinearPoint;

    marginedLinear->linearRate = calcLinear(marginedLinear->highFlatVal,
                                            marginedLinear->lowFlatVal,
                                            marginedLinear->highLinearPoint,
                                            marginedLinear->lowLinearPoint);

    return marginedLinear->highLinearPoint;
}
uint16_t piu_MarginedLinear_updateStepUpPoint(piu_MarginedLinear* marginedLinear,
                                              uint16_t newStepUpPoint)
{
    return marginedLinear->stepUpPoint = newStepUpPoint;
}
uint16_t piu_MarginedLinear_updateStepDownPoint(
    piu_MarginedLinear* marginedLinear,
    uint16_t newStepDownPoint)
{
    return marginedLinear->stepDownPoint = newStepDownPoint;
}

uint16_t piu_MarginedLinear_updateOffVal(piu_MarginedLinear* marginedLinear,
                                         uint16_t newOffVal)
{
    return marginedLinear->offVal = newOffVal;
}
uint16_t piu_MarginedLinear_updateLowFlatVal(piu_MarginedLinear* marginedLinear,
                                             uint16_t newLowFlatVal)
{
    marginedLinear->lowFlatVal = newLowFlatVal;
    
    marginedLinear->linearRate = calcLinear(marginedLinear->highFlatVal,
                                            marginedLinear->lowFlatVal,
                                            marginedLinear->highLinearPoint,
                                            marginedLinear->lowLinearPoint);
    
    return marginedLinear->lowFlatVal;
}
uint16_t piu_MarginedLinear_updateHighFlatVal(piu_MarginedLinear* marginedLinear,
                                              uint16_t newHighFlatVal)
{
    marginedLinear->highFlatVal = newHighFlatVal;
    
    marginedLinear->linearRate = calcLinear(marginedLinear->highFlatVal,
                                            marginedLinear->lowFlatVal,
                                            marginedLinear->highLinearPoint,
                                            marginedLinear->lowLinearPoint);
    
    return marginedLinear->highFlatVal;
}
uint16_t piu_MarginedLinear_updateMaxFlatVal(piu_MarginedLinear* marginedLinear,
                                             uint16_t newMaxFlatVal)
{
    return marginedLinear->maxFlatVal = newMaxFlatVal;
}
