/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define LED1_RED_Pin GPIO_PIN_0
#define LED1_RED_GPIO_Port GPIOA
#define LED1_GRN_Pin GPIO_PIN_1
#define LED1_GRN_GPIO_Port GPIOA
#define LED1_BLU_Pin GPIO_PIN_2
#define LED1_BLU_GPIO_Port GPIOA
#define LED1_REDA3_Pin GPIO_PIN_3
#define LED1_REDA3_GPIO_Port GPIOA
#define LED1_GRNA4_Pin GPIO_PIN_4
#define LED1_GRNA4_GPIO_Port GPIOA
#define LED1_BLUA5_Pin GPIO_PIN_5
#define LED1_BLUA5_GPIO_Port GPIOA
#define LED_Status_Pin GPIO_PIN_8
#define LED_Status_GPIO_Port GPIOD
#define PWswitch_Pin GPIO_PIN_9
#define PWswitch_GPIO_Port GPIOD
#define Sync_IN_Pin GPIO_PIN_13
#define Sync_IN_GPIO_Port GPIOD
#define Sync_IN_EXTI_IRQn EXTI15_10_IRQn
#define Sync_OUT_Pin GPIO_PIN_14
#define Sync_OUT_GPIO_Port GPIOD
#define Daisy_OUT_Pin GPIO_PIN_10
#define Daisy_OUT_GPIO_Port GPIOA
#define Alrm_Pin GPIO_PIN_0
#define Alrm_GPIO_Port GPIOD
#define Redy_Pin GPIO_PIN_1
#define Redy_GPIO_Port GPIOD
#define LED_RE_Pin GPIO_PIN_4
#define LED_RE_GPIO_Port GPIOD
#define LED_YE_Pin GPIO_PIN_5
#define LED_YE_GPIO_Port GPIOD
#define LED_GR_Pin GPIO_PIN_6
#define LED_GR_GPIO_Port GPIOD
#define LED_YE2_Pin GPIO_PIN_7
#define LED_YE2_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
