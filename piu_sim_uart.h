//
// Created by YthanZhang on 2021/12/30.
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
 * @brief piu_sim_uart.h provides functionality for sim/soft uart
 * @note Sim UART relies heavily on correct timing to work properly.
 * @note This util is written with the assumption of a setup of soft UART with:
 * one GPIO interrupt triggering on start bit, one hardware timer or high
 * accuracy software timer interrupt for consecutive input detections.
 * @note For UART to work at a certain baud, the timer interrupt interval should
 * be set to trigger once for every bit
 *
 * @example This example is Sim/Soft UART in half duplex mode.
 *  For <b>Rx</b>, assuming the <b>piu_SimUART</b> variable is named
 * <b>sUART</b>:
 * @code
 *  void GPIO_FALLING_EDGE_IT_HANDLE(void)
 *  {
 *      if(GPIO_1 == 0)
 *      {
 *          delay_us(UART_HALF_BIT_TIME);
 *          // delay for half of the bit's duration
 *
 *          if(piu_SimUART_GPIOUpdate(&sUART, GPIO_1))
 *          {
 *              GPIO_InterruptDisable();
 *              TIMER_StartCounter();
 *          }
 *      }
 *  }
 *
 *  void TIMER_OVERFLOW_IT_HANDLE(void)
 *  {
 *      // since only one timer is used, full duplex is not possible
 *      if!(piu_SimUART_halfDuplexTIMUpdate(&sUART, GPIO_1))
 *      {
 *          TIMER_StopCounter();
 *          TIMER_ResetCounter();
 *          GPIO_InterruptEnable();
 *      }
 *  }
 * @endcode
 * @n @n
 * @example For <b>Tx</b>, the timer interrupt setup doesn't need to change,
 * just enable timer counter after calling <b>piu_SimUART_sendTx</b>:
 * @code
 *  piu_SimUART_sendTx(&sUART, 0x55);
 *  TIMER_StartCounter();
 * @endcode
 */


#ifndef PIU_SIM_UART_H
#define PIU_SIM_UART_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stdint.h>


#define PIU_SIM_UART_RX_COUNT_MAX 9
#define PIU_SIM_UART_TX_COUNT_MAX 10


typedef struct piu_struct_SimUART
{
    bool flag_rxComplete;
    bool flag_txComplete;

    bool flag_rxFrameErr;

    uint8_t rxBuffer;
    uint8_t txBuffer;

    uint8_t rxCounter;
    uint8_t txCounter;

    void (*setTxFunc)(bool txVal);
} piu_SimUART;


/**
 * @brief Use this macro to create a piu_SimUART struct
 * @note A piu_SimUART struct created with this macro doesn't need to call
 *      <b>piu_SimUART_construct</b>
 * @param SET_TX_FUNC A function pointer to a function that sets the Tx pin's
 *      high/low.@n
 *      This function should takes on boolean argument.@n
 *      The function pointer must <b>NOT</b> be @p NULL
 */
#define PIU_SIM_UART_MAKE(SET_TX_FUNC)                                         \
    {                                                                          \
        false, true, false, 0, 0, PIU_SIM_UART_RX_COUNT_MAX + 1,               \
            PIU_SIM_UART_TX_COUNT_MAX + 1, (SET_TX_FUNC)                       \
    }

/**
 * @brief Call this function to initialized a piu_SimUART struct
 * @param simUART Pointer to an uninitialized piu_SimUART struct
 * @param setTxFunc A function pointer to a function that sets the Tx pin's
 *      high/low.@n
 *      This function should takes on boolean argument.@n
 *      The function pointer must <b>NOT</b> be @p NULL
 * @return Pointer to the same piu_SimUART struct passed in
 */
piu_SimUART* piu_SimUART_construct(piu_SimUART* simUART,
                                   void (*setTxFunc)(bool));

/**
 * @brief Called in timer interrupt, see example at the start of the file
 * @note
 * @param simUART Pointer to a piu_SimUART struct
 * @param rxVal The Rx pin's value
 * @return <b>true</b> if the process is still continuing, <b>false</b> if the
 *      process is complete and the timer can stop and reset
 */
bool piu_SimUART_halfDuplexTIMUpdate(piu_SimUART* simUART, bool rxVal);
/**
 * @brief Called in GPIO interrupt, see example at the start of the file
 * @param simUART Pointer to a piu_SimUART struct
 * @param rxVal The Rx Pin's value
 * @return <b>true</b> if the pin value is correct and should stop GPIO
 *      interrupt and start timer, <b>false</b> for incorrect start bit and
 *      should do nothing
 */
bool piu_SimUART_GPIOUpdate(piu_SimUART* simUART, bool rxVal);

/**
 * @brief Get the latest rx result
 * @note Return value is invalid if @p flag_rxFrameErr is <b>true</b>
 * @note Trying to read rx when rx complete is not true may result in invalid
 *      result
 * @param simUART Pointer to a piu_SimUART struct
 * @return The latest rx result
 */
uint8_t piu_SimUART_getRx(piu_SimUART* simUART);
/**
 * @brief Configure sim UART to send data
 * @note Must enable timer & timer interrupt for data to actually be sent
 * @param simUART Pointer to a piu_SimUART struct
 * @param val The 8 bits of data to send
 * @return <b>true</b> if data is set, <b>false</b> if the previous tx has not
 *      finished and the new value could not be sent
 */
bool piu_SimUART_sendTx(piu_SimUART* simUART, uint8_t val);

/**
 * @brief Get if the latest rx has completed and has not yet been retrieved
 * @param simUART Pointer to a piu_SimUART struct
 * @return <b>true</b> if the latest rx has completed and has not yet been
 *      retrieved, <b>false</b> otherwise
 */
bool piu_SimUART_getRxComplete(piu_SimUART* simUART);
/**
 * @brief Get if the latest tx has completed
 * @param simUART Pointer to a piu_SimUART struct
 * @return <b>true</b> if the latest tx has completed, <b>false</b> otherwise
 */
bool piu_SimUART_getTxComplete(piu_SimUART* simUART);
/**
 * @brief Get if the latest rx attempt has ended in error
 * @param simUART Pointer to a pui_SimUART struct
 * @return <b>true</b> if the latest rx attempt has ended in error, <b>false</b>
 * otherwise
 */
bool piu_SimUART_getRxFrameErr(piu_SimUART* simUART);


#ifdef __cplusplus
}
#endif

#endif    // PIU_SIM_UART_H
