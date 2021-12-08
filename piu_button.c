//
// Created by YthanZhang on 2021/12/8.
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

#include "piu_button.h"


piu_Button* piu_Button_construct(piu_Button* buttonStruct, uint16_t stableThreshold)
{
    buttonStruct->isPressed           = false;
    buttonStruct->oldPress            = false;
    buttonStruct->stableState         = false;
    buttonStruct->stableCounter       = 0;
    buttonStruct->stableLengthCounter = 0;

    buttonStruct->stableThreshold = stableThreshold;

    return buttonStruct;
}

void piu_Button_updateState(piu_Button* buttonStruct, bool newState)
{
    if (newState != buttonStruct->isPressed)
    {
        buttonStruct->oldPress  = buttonStruct->isPressed;
        buttonStruct->isPressed = newState;
    }
}

void piu_Button_tick(piu_Button* buttonStruct)
{
    if (buttonStruct->isPressed == buttonStruct->stableState &&
        buttonStruct->stableLengthCounter < UINT16_MAX)
    {
        ++(buttonStruct->stableLengthCounter);
    }

    if (buttonStruct->isPressed != buttonStruct->oldPress)
    {
        buttonStruct->oldPress      = buttonStruct->isPressed;
        buttonStruct->stableCounter = 0;
    }

    if (buttonStruct->stableCounter < UINT16_MAX)
    {
        ++(buttonStruct->stableCounter);
    }
    if (buttonStruct->stableCounter == buttonStruct->stableThreshold &&
        buttonStruct->isPressed != buttonStruct->stableState)
    {
        buttonStruct->stableState         = buttonStruct->isPressed;
        buttonStruct->stableLengthCounter = 0;
    }
}

bool piu_Button_stableState(piu_Button* buttonStruct)
{
    return buttonStruct->stableState;
}
uint16_t piu_Button_stableLength(piu_Button* buttonStruct)
{
    return buttonStruct->stableLengthCounter;
}
