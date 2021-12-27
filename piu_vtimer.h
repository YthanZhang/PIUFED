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


#ifndef PIU_VTIMER_H
#define PIU_VTIMER_H

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
    piu_VTCDir countDirection;

    bool flag_overflow;
    bool flag_overOverflow;
    bool flag_counterActive;
} piu_VTimer;


/**
 * @brief Use this function to initialized a piu_VTimer struct
 * @param vTimer Pointer to an uninitialized piu_VTimer struct
 * @param counterReloadValue The reload value the timer counter will be set to
 *      on counter overflow or on manual counter reload
 * @param counterDirection The direction counter should go, see piu_VTCDir
 * @param timerMode Should timer stop after one overflow or be continuous, see
 *      piu_VTMode
 * @param callbackFunc The callback function piu_VTimer_tick will call on
 *      overflow event, set to @p NULL for no callback
 * @return Pointer to the same piu_VTimer struct passed in
 */
piu_VTimer* piu_VTimer_construct(piu_VTimer* vTimer,
                                 uint16_t counterReloadValue,
                                 piu_VTCDir counterDirection,
                                 piu_VTMode timerMode,
                                 void (*callbackFunc)(void));

/**
 * @brief Should call this function for timer update, preferably with a
 *      consistent interval
 * @param vTimer Pointer to a piu_VTimer struct
 */
void piu_VTimer_tick(piu_VTimer* vTimer);

/**
 * @brief Use this function to set new counter reload value
 * @note The counter reload value can be changed at any time, even when the
 *      counter is active
 * @param vTimer piu_VTimer struct
 * @param counterReloadValue New counter reload value
 * @return The counter reload value
 */
uint16_t piu_VTimer_setCounterReloadValue(piu_VTimer* vTimer,
                                          uint16_t counterReloadValue);
/**
 * @brief Use this function to set counter direction
 * @warning A new value can only be set when counter is not active. If called
 *      when counter is active, the count direction will <b>NOT</b> change and
 *      the original count direction will be returned
 * @warning When a new direction is set, the counter value will reload
 * @param vTimer Pointer to a piu_VTimer struct
 * @param countDirection New count direction
 * @return The count direction
 */
piu_VTCDir piu_VTimer_setCountDirection(piu_VTimer* vTimer,
                                        piu_VTCDir countDirection);
/**
 * @brief Use this function to set time to be in one shot mode or in continuous
 *      mode
 * @note Timer mode can be changed at any time, even when the counter is
 *      active
 * @param vTimer Pointer to a piu_VTimer struct
 * @param timerMode New timer mode, can be either @p piu_VTMode_OneShot or @p
 * piu_VTMode_Continuous
 * @return The timer mode
 */
piu_VTMode piu_VTimer_setTimerMode(piu_VTimer* vTimer, piu_VTMode timerMode);
/**
 * @brief Use this function to set callback function on counter overflow
 * @note Set to NULL to disable callback function
 * @warning A new callback can only be set when counter is not active. If called
 *      when counter is active, the callback function will <b>NOT</b> change
 * @param vTimer Pointer to a piu_VTimer struct
 * @param callbackFunc Pointer to new callback function, use @p NULL to disable
 *      callback
 */
void piu_VTimer_setCallback(piu_VTimer* vTimer, void (*callbackFunc)(void));

/**
 * @brief Start counter, have no effect if counter already active
 * @param vTimer Pointer to a piu_VTimer struct
 * @return The current counter value
 */
uint16_t piu_VTimer_startCounter(piu_VTimer* vTimer);
/**
 * @brief Stop counter, have no effect if counter is not active
 * @param vTimer Pointer to a piu_VTimer struct
 * @return The current counter value
 */
uint16_t piu_VTimer_stopCounter(piu_VTimer* vTimer);
/**
 * @brief Reset counter to counter reload value
 * @note This function will have effect regardless if counter is active or not
 * @param vTimer Pointer to a piu_VTimer struct
 * @return The counter reload value
 */
uint16_t piu_VTimer_reloadCounter(piu_VTimer* vTimer);
/**
 * @brief Get the current counter value
 * @param vTimer Pointer to a piu_VTimer struct
 * @return The counter value
 */
uint16_t piu_VTimer_getCounter(piu_VTimer* vTimer);
/**
 * @brief Get the counter reload value
 * @param vTimer Pointer to a piu_VTimer struct
 * @return The counter reload value
 */
uint16_t piu_VTimer_getCounterReloadValue(piu_VTimer* vTimer);
/**
 * @brief Get count direction
 * @param vTimer Pointer to a piu_VTimer struct
 * @return The count direction
 */
piu_VTCDir piu_VTimer_getCountDirection(piu_VTimer* vTimer);

/**
 * @brief Get overflow flag
 * @note The flag will be cleared automatically when this function is called
 * @param vTimer Pointer to a piu_VTimer struct
 * @return @p true if there were an overflow since the last call, else return @p
 *      false
 */
bool piu_VTimer_getOverflow(piu_VTimer* vTimer);
/**
 * @brief Get over overflow flag
 * @note If the overflow flag was still set when the second overflow happened,
 *      the over overflow flag will be set
 * @note This flag must be cleared explicitly with function
 *      piu_VTimer_clearOverOverflow
 * @param vTimer Pointer to a piu_VTimer struct
 * @return @p true if there were an over overflow, else return @p false
 */
bool piu_VTimer_getOverOverflow(piu_VTimer* vTimer);
/**
 * @brief Check if counter/timer is currently active
 * @param vTimer Pointer to a piu_VTimer struct
 * @return @p true if counter/timer is active, else return @p false
 */
bool piu_VTimer_getCounterActive(piu_VTimer* vTimer);

/**
 * @brief Clear over overflow flag
 * @note this flag can only be cleared explicitly with this function
 * @param vTimer Pointer to a piu_VTimer struct
 * @return @p true if the over overflow flag was set prior to the call of this
 *      function, else return @p false
 */
bool piu_VTimer_clearOverOverflow(piu_VTimer* vTimer);



#ifdef __cplusplus
}
#endif

#endif    // PIU_VTIMER_H
