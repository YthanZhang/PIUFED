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


#ifndef PIU_MARGINED_LINEAR_H
#define PIU_MARGINED_LINEAR_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


typedef enum enum_piu_MarginSection
{
    piu_MarginState_Off,
    piu_MarginState_LowFlat,
    piu_MarginState_Linear,
    piu_MarginState_HighFlat,
    piu_MarginState_MaxFlat,
} piu_MarginSection;


typedef struct struct_piu_MarginedLinear
{
    piu_MarginSection currentState;
    
    uint16_t offPoint;
    uint16_t onPoint;
    uint16_t lowLinearPoint;
    uint16_t highLinearPoint;
    uint16_t stepUpPoint;
    uint16_t stepDownPoint;

    uint16_t offVal;
    uint16_t lowFlatVal;
    uint16_t highFlatVal;
    uint16_t maxFlatVal;

    float linearRate;
    
    uint16_t lastInput;
} piu_MarginedLinear;


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
    uint16_t maxFlatVal);


uint16_t piu_MarginedLinear_input(piu_MarginedLinear* marginedLinear,
                                  uint16_t inputVal);
uint16_t piu_MarginedLinear_output(piu_MarginedLinear* marginedLinear);

uint16_t piu_MarginedLinear_updateOffPoint(piu_MarginedLinear* marginedLinear,
                                           uint16_t newOffPoint);
uint16_t piu_MarginedLinear_updateOnPoint(piu_MarginedLinear* marginedLinear,
                                          uint16_t newOnPoint);
uint16_t piu_MarginedLinear_updateLowLinearPoint(
    piu_MarginedLinear* marginedLinear,
    uint16_t newLowLinearPoint);
uint16_t piu_MarginedLinear_updateHighLinearPoint(
    piu_MarginedLinear* marginedLinear,
    uint16_t newHighLinearPoint);
uint16_t piu_MarginedLinear_updateStepUpPoint(piu_MarginedLinear* marginedLinear,
                                              uint16_t newStepUpPoint);
uint16_t piu_MarginedLinear_updateStepDownPoint(
    piu_MarginedLinear* marginedLinear,
    uint16_t newStepDownPoint);

uint16_t piu_MarginedLinear_updateOffVal(piu_MarginedLinear* marginedLinear,
                                         uint16_t newOffVal);
uint16_t piu_MarginedLinear_updateLowFlatVal(piu_MarginedLinear* marginedLinear,
                                             uint16_t newLowFlatVal);
uint16_t piu_MarginedLinear_updateHighFlatVal(piu_MarginedLinear* marginedLinear,
                                              uint16_t newHighFlatVal);
uint16_t piu_MarginedLinear_updateMaxFlatVal(piu_MarginedLinear* marginedLinear,
                                             uint16_t newMaxFlatVal);


#ifdef __cplusplus
}
#endif

#endif    // PIU_MARGINED_LINEAR_H
