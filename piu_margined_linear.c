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
    return ((float)highFlatVal - (float)lowFlatVal) /
           ((float)highLinearPoint - (float)lowLinearPoint);
}


piu_MarginedLinear piu_MarginedLinear_make(uint16_t xOff,
                                           uint16_t xOn,
                                           uint16_t xLowLinear,
                                           uint16_t xHighLinear,
                                           uint16_t xStepDown,
                                           uint16_t xStepUp,
                                           uint16_t yOff,
                                           uint16_t yLowFlat,
                                           uint16_t yHighFlat,
                                           uint16_t yMaxFlat)
{
    piu_MarginedLinear ml = {};
    piu_MarginedLinear_construct(&ml,
                                 xOff,
                                 xOn,
                                 xLowLinear,
                                 xHighLinear,
                                 xStepDown,
                                 xStepUp,
                                 yOff,
                                 yLowFlat,
                                 yHighFlat,
                                 yMaxFlat);
    return ml;
}


piu_MarginedLinear* piu_MarginedLinear_construct(
    piu_MarginedLinear* marginedLinear,
    uint16_t xOff,
    uint16_t xOn,
    uint16_t xLowLinear,
    uint16_t xHighLinear,
    uint16_t xStepDown,
    uint16_t xStepUp,
    uint16_t yOff,
    uint16_t yLowFlat,
    uint16_t yHighFlat,
    uint16_t yMaxFlat)
{
    piu_MarginedLinear_updateInput(marginedLinear,
                                   xOff,
                                   xOn,
                                   xLowLinear,
                                   xHighLinear,
                                   xStepDown,
                                   xStepUp);
    piu_MarginedLinear_updateOutput(marginedLinear,
                                    yOff,
                                    yLowFlat,
                                    yHighFlat,
                                    yMaxFlat);

    marginedLinear->lastInput    = xOff;
    marginedLinear->currentState = piu_MarginState_Off;

    return marginedLinear;
}


uint16_t piu_MarginedLinear_setX(piu_MarginedLinear* marginedLinear,
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

    return piu_MarginedLinear_getY(marginedLinear);
}

uint16_t piu_MarginedLinear_getY(const piu_MarginedLinear* marginedLinear)
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
                           (float)(marginedLinear->lastInput -
                                   marginedLinear->lowLinearPoint)) +
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

void piu_MarginedLinear_updateInput(piu_MarginedLinear* marginedLinear,
                                    uint16_t xOff,
                                    uint16_t xOn,
                                    uint16_t xLowLinear,
                                    uint16_t xHighLinear,
                                    uint16_t xStepDown,
                                    uint16_t xStepUp)
{
    marginedLinear->currentState = piu_MarginState_Off;

    // Check and fix inversion, this struct/class/statemachine is only defined
    // when these values are in this order:
    // offPoint <= onPoint <= lowLinearPoint <= highLinearPoint <= stepDownPoint
    // <= stepUpPoint
    if (xOn < xOff)
    {
        xOn = xOff;
    }
    if (xLowLinear < xOn)
    {
        xLowLinear = xOn;
    }
    if (xHighLinear < xLowLinear)
    {
        xHighLinear = xLowLinear;
    }
    if (xStepDown < xHighLinear)
    {
        xStepDown = xHighLinear;
    }
    if (xStepUp < xStepDown)
    {
        xStepUp = xStepDown;
    }

    marginedLinear->offPoint        = xOff;
    marginedLinear->onPoint         = xOn;
    marginedLinear->lowLinearPoint  = xLowLinear;
    marginedLinear->highLinearPoint = xHighLinear;
    marginedLinear->stepDownPoint   = xStepDown;
    marginedLinear->stepUpPoint     = xStepUp;

    marginedLinear->linearRate = calcLinear(marginedLinear->highFlatVal,
                                            marginedLinear->lowFlatVal,
                                            xHighLinear,
                                            xLowLinear);

    piu_MarginedLinear_setX(marginedLinear, marginedLinear->lastInput);
}

void piu_MarginedLinear_updateOutput(piu_MarginedLinear* marginedLinear,
                                     uint16_t yOff,
                                     uint16_t yLowFlat,
                                     uint16_t yHighFlat,
                                     uint16_t yMaxFlat)
{
    marginedLinear->offVal      = yOff;
    marginedLinear->lowFlatVal  = yLowFlat;
    marginedLinear->highFlatVal = yHighFlat;
    marginedLinear->maxFlatVal  = yMaxFlat;

    marginedLinear->linearRate = calcLinear(yHighFlat,
                                            yLowFlat,
                                            marginedLinear->highLinearPoint,
                                            marginedLinear->lowLinearPoint);
}
