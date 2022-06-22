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
            if (inputVal > marginedLinear->xOn)
            {
                marginedLinear->currentState = piu_MarginState_LowFlat;
                continue;
            }
            break;
        }
        case (piu_MarginState_LowFlat): {
            if (inputVal > marginedLinear->xLinearLow)
            {
                marginedLinear->currentState = piu_MarginState_Linear;
                continue;
            }
            else if (inputVal <= marginedLinear->xOff)
            {
                marginedLinear->currentState = piu_MarginState_Off;
                continue;
            }
            break;
        }
        case (piu_MarginState_Linear): {
            if (inputVal > marginedLinear->xLinearHigh)
            {
                marginedLinear->currentState = piu_MarginState_HighFlat;
                continue;
            }
            else if (inputVal <= marginedLinear->xLinearLow)
            {
                marginedLinear->currentState = piu_MarginState_LowFlat;
                continue;
            }
            break;
        }
        case (piu_MarginState_HighFlat): {
            if (inputVal > marginedLinear->xStepUp)
            {
                marginedLinear->currentState = piu_MarginState_MaxFlat;
                continue;
            }
            else if (inputVal <= marginedLinear->xLinearHigh)
            {
                marginedLinear->currentState = piu_MarginState_Linear;
                continue;
            }
            break;
        }
        case (piu_MarginState_MaxFlat): {
            if (inputVal <= marginedLinear->xStepDown)
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
        return marginedLinear->yOff;
    }
    case (piu_MarginState_LowFlat): {
        return marginedLinear->yLowFlat;
    }
    case (piu_MarginState_Linear): {
        return (uint16_t)((marginedLinear->linearRate *
                           (float)(marginedLinear->lastInput -
                                   marginedLinear->xLinearLow)) +
                          (float)marginedLinear->yLowFlat);
    }
    case (piu_MarginState_HighFlat): {
        return marginedLinear->yHighFlat;
    }
    case (piu_MarginState_MaxFlat): {
        return marginedLinear->yMaxFlat;
    }
    default: {
        return marginedLinear->yOff;
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

    marginedLinear->xOff        = xOff;
    marginedLinear->xOn         = xOn;
    marginedLinear->xLinearLow  = xLowLinear;
    marginedLinear->xLinearHigh = xHighLinear;
    marginedLinear->xStepDown   = xStepDown;
    marginedLinear->xStepUp     = xStepUp;

    marginedLinear->linearRate = calcLinear(marginedLinear->yHighFlat,
                                            marginedLinear->yLowFlat,
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
    marginedLinear->yOff      = yOff;
    marginedLinear->yLowFlat  = yLowFlat;
    marginedLinear->yHighFlat = yHighFlat;
    marginedLinear->yMaxFlat  = yMaxFlat;

    marginedLinear->linearRate = calcLinear(yHighFlat,
                                            yLowFlat,
                                            marginedLinear->xLinearHigh,
                                            marginedLinear->xLinearLow);
}


void piu_MarginedLinear_updateXOff(piu_MarginedLinear* marginedLinear,
                                   uint16_t xOff)
{
    piu_MarginedLinear_updateInput(marginedLinear,
                                   xOff,
                                   marginedLinear->xOn,
                                   marginedLinear->xLinearLow,
                                   marginedLinear->xLinearHigh,
                                   marginedLinear->xStepDown,
                                   marginedLinear->xStepUp);
}
void piu_MarginedLinear_updateXOn(piu_MarginedLinear* marginedLinear,
                                  uint16_t xOn)
{
    piu_MarginedLinear_updateInput(marginedLinear,
                                   marginedLinear->xOff,
                                   xOn,
                                   marginedLinear->xLinearLow,
                                   marginedLinear->xLinearHigh,
                                   marginedLinear->xStepDown,
                                   marginedLinear->xStepUp);
}
void piu_MarginedLinear_updateXLinearLow(piu_MarginedLinear* marginedLinear,
                                         uint16_t xLinearLow)
{
    piu_MarginedLinear_updateInput(marginedLinear,
                                   marginedLinear->xOff,
                                   marginedLinear->xOn,
                                   xLinearLow,
                                   marginedLinear->xLinearHigh,
                                   marginedLinear->xStepDown,
                                   marginedLinear->xStepUp);
}
void piu_MarginedLinear_updateXLinearHigh(piu_MarginedLinear* marginedLinear,
                                          uint16_t xLinearHigh)
{
    piu_MarginedLinear_updateInput(marginedLinear,
                                   marginedLinear->xOff,
                                   marginedLinear->xOn,
                                   marginedLinear->xLinearLow,
                                   xLinearHigh,
                                   marginedLinear->xStepDown,
                                   marginedLinear->xStepUp);
}
void piu_MarginedLinear_updateXStepDown(piu_MarginedLinear* marginedLinear,
                                        uint16_t xStepDown)
{
    piu_MarginedLinear_updateInput(marginedLinear,
                                   marginedLinear->xOff,
                                   marginedLinear->xOn,
                                   marginedLinear->xLinearLow,
                                   marginedLinear->xLinearHigh,
                                   xStepDown,
                                   marginedLinear->xStepUp);
}
void piu_MarginedLinear_updateXStepUp(piu_MarginedLinear* marginedLinear,
                                      uint16_t xStepUp)
{
    piu_MarginedLinear_updateInput(marginedLinear,
                                   marginedLinear->xOff,
                                   marginedLinear->xOn,
                                   marginedLinear->xLinearLow,
                                   marginedLinear->xLinearHigh,
                                   marginedLinear->xStepDown,
                                   xStepUp);
}

void piu_MarginedLinear_updateYOff(piu_MarginedLinear* marginedLinear,
                                   uint16_t yOff)
{
    piu_MarginedLinear_updateOutput(marginedLinear,
                                    yOff,
                                    marginedLinear->yLowFlat,
                                    marginedLinear->yHighFlat,
                                    marginedLinear->yMaxFlat);
}
void piu_MarginedLinear_updateYLowFlat(piu_MarginedLinear* marginedLinear,
                                       uint16_t yLowFlat)
{
    piu_MarginedLinear_updateOutput(marginedLinear,
                                    marginedLinear->yOff,
                                    yLowFlat,
                                    marginedLinear->yHighFlat,
                                    marginedLinear->yMaxFlat);
}
void piu_MarginedLinear_updateYHighFlat(piu_MarginedLinear* marginedLinear,
                                        uint16_t yHighFlat)
{
    piu_MarginedLinear_updateOutput(marginedLinear,
                                    marginedLinear->yOff,
                                    marginedLinear->yLowFlat,
                                    yHighFlat,
                                    marginedLinear->yMaxFlat);
}
void piu_MarginedLinear_updateYMaxFlat(piu_MarginedLinear* marginedLinear,
                                       uint16_t yMaxFlat)
{
    piu_MarginedLinear_updateOutput(marginedLinear,
                                    marginedLinear->yOff,
                                    marginedLinear->yLowFlat,
                                    marginedLinear->yHighFlat,
                                    yMaxFlat);
}


void piu_MarginedLinear_shrinkRangeX(piu_MarginedLinear* marginedLinear,
                                     uint16_t low,
                                     uint16_t high)
{
    if (low > high)
    {
        return;
    }

    if (marginedLinear->xOff >= low && marginedLinear->xStepUp <= high)
    {
        return;
    }

    if (marginedLinear->xStepUp > high)
    {
        marginedLinear->xStepUp = high;
    }
    if (marginedLinear->xStepDown > marginedLinear->xStepUp)
    {
        marginedLinear->xStepDown = marginedLinear->xStepUp;
    }
    if (marginedLinear->xLinearHigh > marginedLinear->xStepDown)
    {
        marginedLinear->xLinearHigh = marginedLinear->xStepDown;
    }
    if (marginedLinear->xLinearLow > marginedLinear->xLinearHigh)
    {
        marginedLinear->xLinearLow = marginedLinear->xLinearHigh;
    }
    if (marginedLinear->xOn > marginedLinear->xLinearLow)
    {
        marginedLinear->xOn = marginedLinear->xLinearLow;
    }
    if (marginedLinear->xOff > marginedLinear->xOn)
    {
        marginedLinear->xOff = marginedLinear->xOn;
    }

    if (marginedLinear->xOff < low)
    {
        marginedLinear->xOff = low;
    }
    if (marginedLinear->xOn < marginedLinear->xOff)
    {
        marginedLinear->xOn = marginedLinear->xOff;
    }
    if (marginedLinear->xLinearLow < marginedLinear->xOn)
    {
        marginedLinear->xLinearLow = marginedLinear->xOn;
    }
    if (marginedLinear->xLinearHigh < marginedLinear->xLinearLow)
    {
        marginedLinear->xLinearHigh = marginedLinear->xLinearLow;
    }
    if (marginedLinear->xStepDown < marginedLinear->xLinearHigh)
    {
        marginedLinear->xStepDown = marginedLinear->xLinearHigh;
    }
    if (marginedLinear->xStepUp < marginedLinear->xStepDown)
    {
        marginedLinear->xStepUp = marginedLinear->xStepDown;
    }
}


void piu_MarginedLinear_shrinkRangeY(piu_MarginedLinear* marginedLinear,
                                     uint16_t low,
                                     uint16_t high)
{
    if (low > high)
    {
        return;
    }

    if ((marginedLinear->yOff >= low) && (marginedLinear->yMaxFlat <= high))
    {
        return;
    }

    if (marginedLinear->yLowFlat < low)
    {
        marginedLinear->yLowFlat = low;
    }
    if (marginedLinear->yHighFlat < marginedLinear->yLowFlat)
    {
        marginedLinear->yHighFlat = marginedLinear->yLowFlat;
    }
    if (marginedLinear->yMaxFlat < marginedLinear->yHighFlat)
    {
        marginedLinear->yMaxFlat = marginedLinear->yHighFlat;
    }

    if (marginedLinear->yMaxFlat > high)
    {
        marginedLinear->yMaxFlat = high;
    }
    if (marginedLinear->yHighFlat > marginedLinear->yMaxFlat)
    {
        marginedLinear->yHighFlat = marginedLinear->yMaxFlat;
    }
    if (marginedLinear->yLowFlat > marginedLinear->yHighFlat)
    {
        marginedLinear->yLowFlat = marginedLinear->yHighFlat;
    }
    if (marginedLinear->yOff > marginedLinear->yLowFlat)
    {
        marginedLinear->yOff = marginedLinear->yLowFlat;
    }
}
