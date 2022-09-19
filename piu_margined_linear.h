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
extern "C"
{
#endif


#include <stdint.h>
#include <stdbool.h>


typedef enum enum_piu_MarginSection
{
    piu_MarginState_Off,
    piu_MarginState_LowFlat,
    piu_MarginState_Linear,
    piu_MarginState_HighFlat,
    piu_MarginState_MaxFlat,
} piu_MarginSection;


/**
 * @warning You must NEVER directly modify any of the internal data directly,
 *      always use functions that came with the struct to operate with it.
 * @warning Modification of internal data without using related functions may
 *      cause the struct to enter undefined state.
 */
typedef struct struct_piu_MarginedLinear
{
    piu_MarginSection currentState;

    uint16_t xOff;
    uint16_t xOn;
    uint16_t xLinearLow;
    uint16_t xLinearHigh;
    uint16_t xStepDown;
    uint16_t xStepUp;

    uint16_t yOff;
    uint16_t yLowFlat;
    uint16_t yHighFlat;
    uint16_t yMaxFlat;

    float linearRate;

    uint16_t lastInput;
} piu_MarginedLinear;


/**
 * @brief Create a margined linear (state machine)/(function)
 * @param xOff The off input level from <b>on</b> state
 * @param xOn The on input level from <b>off</b> state
 * @param xLowLinear The low linear input level, below this value the output is
 *      @p yLowFlat, above this value the output is linear from @p yLowFlat to
 *      @p yHighFlat until @p xHighLinear
 * @param xHighLinear The high linear input level, below this value the output
 *      is linear from @p yLowFlat to @p yHighFlat , above this value the output
 *      is @p yHighFlat
 * @param xStepDown The output will step down from @p yMaxFlat to @p yHighFlat
 *      when input level goes below this value
 * @param xStepUp The output will step up from @p yHighFlat to @p yMaxFlat when
 *      input level goes above this value
 * @param yOff The off state output value
 * @param yLowFlat The low flat state output value, also the low value of the
 *      linear section
 * @param yHighFlat The high flat state output value, also the high value of the
 *      linear section
 * @param yMaxFlat The maximum output level when input is greater than @p
 *      xStepUp
 * @return A piu_MarginedLinear struct
 */
piu_MarginedLinear piu_MarginedLinear_make(uint16_t xOff,
                                           uint16_t xOn,
                                           uint16_t xLowLinear,
                                           uint16_t xHighLinear,
                                           uint16_t xStepDown,
                                           uint16_t xStepUp,
                                           uint16_t yOff,
                                           uint16_t yLowFlat,
                                           uint16_t yHighFlat,
                                           uint16_t yMaxFlat);


/**
 * @brief Create a margined linear (state machine)/(function)
 * @param marginedLinear Pointer to a empty piu_MarginedLinear struct
 * @param xOff The off input level from <b>on</b> state
 * @param xOn The on input level from <b>off</b> state
 * @param xLowLinear The low linear input level, below this value the output is
 *      @p yLowFlat, above this value the output is linear from @p yLowFlat to
 *      @p yHighFlat until @p xHighLinear
 * @param xHighLinear The high linear input level, below this value the output
 *      is linear from @p yLowFlat to @p yHighFlat , above this value the output
 *      is @p yHighFlat
 * @param xStepDown The output will step down from @p yMaxFlat to @p yHighFlat
 *      when input level goes below this value
 * @param xStepUp The output will step up from @p yHighFlat to @p yMaxFlat when
 *      input level goes above this value
 * @param yOff The off state output value
 * @param yLowFlat The low flat state output value, also the low value of the
 *      linear section
 * @param yHighFlat The high flat state output value, also the high value of the
 *      linear section
 * @param yMaxFlat The maximum output level when input is greater than @p
 *      xStepUp
 * @return Pointer to the struct passed in
 */
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
    uint16_t yMaxFlat);


/**
 * @brief Update state machine with new input value
 * @param marginedLinear Pointer to a piu_MarginedLinear struct
 * @param inputVal the new input value
 * @return The new output value
 */
uint16_t piu_MarginedLinear_setX(piu_MarginedLinear* marginedLinear,
                                 uint16_t inputVal);

/**
 * @brief Get the latest output value
 * @param marginedLinear Pointer to a piu_MarginedLinear struct
 * @return The latest output value
 */
uint16_t piu_MarginedLinear_getY(const piu_MarginedLinear* marginedLinear);


/**
 * @brief Update the state machine with a different set of x values
 * @param marginedLinear Pointer to a piu_MarginedLinear struct
 * @param xOff The new off input level
 * @param xOn  The new on input level
 * @param xLowLinear The new low linear input level
 * @param xHighLinear The new high linear input level
 * @param xStepDown The new step down input level
 * @param xStepUp The new step up input level
 */
void piu_MarginedLinear_updateInput(piu_MarginedLinear* marginedLinear,
                                    uint16_t xOff,
                                    uint16_t xOn,
                                    uint16_t xLowLinear,
                                    uint16_t xHighLinear,
                                    uint16_t xStepDown,
                                    uint16_t xStepUp);
/**
 * @brief Update the state machine with a different set of y values
 * @param marginedLinear Pointer to a piu_MarginedLinear struct
 * @param yOff The new off output value
 * @param yLowFlat The new low flat output value
 * @param yHighFlat The new high flat output value
 * @param yMaxFlat The new max flat output value
 */
void piu_MarginedLinear_updateOutput(piu_MarginedLinear* marginedLinear,
                                     uint16_t yOff,
                                     uint16_t yLowFlat,
                                     uint16_t yHighFlat,
                                     uint16_t yMaxFlat);

void piu_MarginedLinear_updateXOff(piu_MarginedLinear* marginedLinear,
                                   uint16_t xOff);
void piu_MarginedLinear_updateXOn(piu_MarginedLinear* marginedLinear,
                                  uint16_t xOn);
void piu_MarginedLinear_updateXLinearLow(piu_MarginedLinear* marginedLinear,
                                         uint16_t xLinearLow);
void piu_MarginedLinear_updateXLinearHigh(piu_MarginedLinear* marginedLinear,
                                          uint16_t xLinearHigh);
void piu_MarginedLinear_updateXStepDown(piu_MarginedLinear* marginedLinear,
                                        uint16_t xStepDown);
void piu_MarginedLinear_updateXStepUp(piu_MarginedLinear* marginedLinear,
                                      uint16_t xStepUp);

void piu_MarginedLinear_updateYOff(piu_MarginedLinear* marginedLinear,
                                   uint16_t yOff);
void piu_MarginedLinear_updateYLowFlat(piu_MarginedLinear* marginedLinear,
                                       uint16_t yLowFlat);
void piu_MarginedLinear_updateYHighFlat(piu_MarginedLinear* marginedLinear,
                                        uint16_t yHighFlat);
void piu_MarginedLinear_updateYMaxFlat(piu_MarginedLinear* marginedLinear,
                                       uint16_t yMaxFlat);


void piu_MarginedLinear_shrinkRangeX(piu_MarginedLinear* marginedLinear,
                                     uint16_t low,
                                     uint16_t high);
void piu_MarginedLinear_shrinkRangeY(piu_MarginedLinear* marginedLinear,
                                     uint16_t low,
                                     uint16_t high);
bool piu_MarginedLinear_remapY(piu_MarginedLinear* marginedLinear,
                               uint16_t originalLow,
                               uint16_t originalHigh,
                               uint16_t newLow,
                               uint16_t newHigh);


#ifdef __cplusplus
}
#endif

#endif    // PIU_MARGINED_LINEAR_H
