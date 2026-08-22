/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"
#include  <stdio.h>

#include  "timeslice.h"
#include "key.h"
#include "led.h"
#include "taskschedule.h"
#include "ntc_3950.h"
#include "DS18B20.h"
#include "oled.h"
#include "interface.h"
#include "AHT10.h"
#include "BH1750.h"
#include "thingcloud.h"
#include "alarm.h"
#include "delay.h"
#include "customize.h"
#include "controller.h"
#include "Beep.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY_Pin GPIO_PIN_13
#define KEY_GPIO_Port GPIOC
#define DS18B20_IO_Pin GPIO_PIN_1
#define DS18B20_IO_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_8
#define LED_RED_GPIO_Port GPIOC
#define BEEP_Pin GPIO_PIN_8
#define BEEP_GPIO_Port GPIOA
#define Dev_Ctrl1_Pin GPIO_PIN_11
#define Dev_Ctrl1_GPIO_Port GPIOA
#define Dev_Ctrl1A15_Pin GPIO_PIN_15
#define Dev_Ctrl1A15_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

#define  LED_RED_ON()    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET)
#define  LED_RED_OFF()   HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET)
#define  BEEP_ON()       HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET)
#define  BEEP_OFF()      HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET)


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
