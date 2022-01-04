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
 * @example For <b>Rx</b>, assuming the <b>piu_SimUART</b> variable is named
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
 *      if!(piu_SimUART_TIMUpdate(&sUART, GPIO_1))
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


typedef struct piu_struct_SimUART
{
    bool flag_rxComplete;
    bool flag_txComplete;

    bool flag_rxFrameError;

    uint8_t rxBuffer;
    uint8_t txBuffer;

    uint8_t rxCounter;
    uint8_t txCounter;

    void (*setTxFunc)(bool txVal);
} piu_SimUART;


/**
 * @brief Call this function to initialized a piu_SimUART struct
 * @param simUART Pointer to an uninitialized piu_SimUART struct
 * @param setTxFunc Function pointer to a function that sets the Tx pin's
 *      high/low
 * @return Pointer to the same piu_SimUART struct passed in
 */
piu_SimUART* piu_SimUART_construct(piu_SimUART* simUART,
                                   void (*setTxFunc)(bool));

/**
 * @brief Called in timer interrupt, see example at the start of the file
 * @param simUART Pointer to a piu_SimUART struct
 * @param rxVal The Rx pin's value
 * @return <b>true</b> if the process is still continuing, <b>false</b> if the
 *      process is complete and the timer can stop and reset
 */
bool piu_SimUART_TIMUpdate(piu_SimUART* simUART, bool rxVal);
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
 * @note Return value is invalid if @p flag_rxFrameError is <b>true</b>
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


#ifdef __cplusplus
}
#endif

#endif    // PIU_SIM_UART_H
