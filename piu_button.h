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


/*******************************************************************************
 * @file piu_button.h
 *
 * The Button struct and related functions(methods) provides tools to handle
 * button input and de-jitter @n
 *
 * The function <b>piu_Button_updateState</b> and <b>piu_Button_tick</b> are the
 *  core of the button utility @n
 *
 * <b>piu_Button_updateState</b> handles state update, should always provide
 *  this function with the latest hardware input state and should be called as
 *  frequent as possible @n
 *
 * <b>piu_Button_tick</b> must be called at a constant interval, this interval
 *  determines the granularity of your control over the button state
 *
 *
 * @example
 * <pre>
 * Wish to have a button press longer than 1ms to be considered valid input: @n
 *  First call <b>piu_Button_construct</b> with a uninitialized <b>Button</b>
 *  struct, with @p stableThreshold set to 5
 *  Then make sure <b>piu_Button_tick</b> is called every 1ms
 *  Feed the latest button state to the <b>Button</b> struct with
 *  <b>piu_Button_updateState</b> constantly
 *
 *  That's it, call <b>piu_Button_stableState</b> to get the stable state of the
 *  button
 *
 * @note If stableThreshold is set to 0, any jitter, even if the duration lands
 *  between two ticks, as long as updateState picked it up, the next tick will
 *  still set stable state to the jitter
 * </pre>
 */



#ifndef PLATFORM_INDEPENDENT_UTILS_FOR_EMBEDDED_DEVELOPMENT_PIU_BUTTON_H
#define PLATFORM_INDEPENDENT_UTILS_FOR_EMBEDDED_DEVELOPMENT_PIU_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stdint.h>

/**
 * @brief This struct holds data relevant to a button
 * @warning All data contained in this struct should be considered private and
 *      should not be accessed or modified directly. Use functions starting
 *      with <b>Button_</b> to access or modify internal data.
 */
typedef struct piu_struct_Button
{
    bool isPressed;            // The button state
    bool oldPress;             // the button state of the previous tick
    bool pendPress;            // The newest button state
    uint16_t stableCounter;    // Ticks the button has been stable

    bool stableState;            // The stable button state
    uint16_t stableThreshold;    // How many ticks has a state sustained for the
                                 // state to be considered stable
    uint16_t stableLengthCounter;    // How many ticks has the current stable
                                     // state has sustained
} piu_Button;


/**
 * @brief Use this function to initialized a piu_Button struct
 * @param buttonStruct Pointer to a uninitialized piu_Button struct
 * @param stableThreshold The threshold for now many ticks are needed to
 *      determine if press or release is stable
 * @return Pointer to the same piu_Button struct passed in
 */
piu_Button* piu_Button_construct(piu_Button* buttonStruct,
                                 uint16_t stableThreshold);


/**
 * @brief Pass the button state to this function
 * @param buttonStruct Pointer to a piu_Button struct
 * @param newState The most current state of the button
 */
void piu_Button_updateState(piu_Button* buttonStruct, bool newState);

/**
 * @brief Should call this function at a constant interval
 * @param buttonStruct Pointer to a piu_Button struct
 */
void piu_Button_tick(piu_Button* buttonStruct);

/**
 * @brief Get the solid state of the button
 * @param buttonStruct Pointer to a piu_Button struct
 * @return @p true if the button is stably pressed, @p false if the button is
 *      stably unpressed
 */
bool piu_Button_stableState(piu_Button* buttonStruct);
/**
 * @brief Get how many ticks the current state has been stable for
 * @param buttonStruct Pointer to a piu_Button struct
 * @return Ticks the current state has been stable for
 */
uint16_t piu_Button_stableLength(piu_Button* buttonStruct);

/**
 * @brief Set new state stable threshold
 * @param buttonStruct Pointer to a piu_Button struct
 * @param stableThreshold The new state stable threshold
 * @return The new stable threshold
 */
uint16_t piu_Button_setStableThreshold(piu_Button* buttonStruct,
                                       uint16_t stableThreshold);


#ifdef __cplusplus
}
#endif

#endif    // PLATFORM_INDEPENDENT_UTILS_FOR_EMBEDDED_DEVELOPMENT_PIU_BUTTON_H
