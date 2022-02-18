/*
 *******************************************************************************
 * Copyright (c) 2018, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

// Pin number
const PinName digitalPin[] = {
  PC_13,  //D0  - 
  PC_14,  //D1  -
  PC_15,  //D2  - 
  PF_0, //D3
  PF_1,  //D4  - 
  PB_2,  //D5  - 
  PB_4,  //D6  - 
  PA_4,  //D7
  PA_5, //D8
  PA_6, //D9  - 
  PA_0, //D10  -
  PA_1, //D11 - 
  PA_2,  //D12 - USART2_TX
  PA_3,  //D13 - USART2_RX
  PB_10, //D14
  PB_11, //D15
  PB_12, //D16
  PB_13, //D17
  PB_14, //D18 - 
  PB_15, //D19
  PA_8, //D20 - pin29, analog range start
  PA_9,  //D21/A0
  PA_10,  //D22/A1
  PA_11,  //D23/A2
  PA_12,  //D24/A3 -
  PA_13,  //D25/A4
  PA_14,  //D26/A5
  PA_15,  //D27/A6 - pin38, analog range end
  PB_3,  //D28/A7
  PB_4,  //D29/A8
  PB_5,  //D30/A9
  PB_6, //D31 - UART3_TX
  PB_7, //D32 - UART3_RX
  PB_8,  //D33 - BOOT1
  PB_9, //D34 - SWDI0
};

// If analog pins are not contiguous in the digitalPin array:
// Add the analogInputPin array without defining NUM_ANALOG_FIRST
// Analog (Ax) pin number array
// where x is the index to retrieve the digital pin number
const uint32_t analogInputPin[] = {
  20,
  21,
  22,
  23, // Not sure about this one beecasue NUM is 9 in the header, and this is RCX
  24,
  25,
  26,
  27,
};

#ifdef __cplusplus
}
#endif

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

WEAK void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  // RCC_PeriphCLKInitTypeDef PeriphClkInit;

  /**Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  //RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV4;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    _Error_Handler(__FILE__, __LINE__);
  }

  /**Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  //RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  //if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
    _Error_Handler(__FILE__, __LINE__);
  }

  // PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART3
                                       // | RCC_PERIPHCLK_I2C1;
  // PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  // PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  // PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  // if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
    // _Error_Handler(__FILE__, __LINE__);
  // }
}

#ifdef __cplusplus
}
#endif
