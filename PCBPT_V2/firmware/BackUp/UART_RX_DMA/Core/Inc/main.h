/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define PIN_LIMIT_SW_Pin GPIO_PIN_0
#define PIN_LIMIT_SW_GPIO_Port GPIOA
#define PIN_RS485_DE_Pin GPIO_PIN_1
#define PIN_RS485_DE_GPIO_Port GPIOA
#define PIN_RS495_RE_Pin GPIO_PIN_4
#define PIN_RS495_RE_GPIO_Port GPIOA
#define PIN_MS1_Pin GPIO_PIN_5
#define PIN_MS1_GPIO_Port GPIOA
#define PIN_MS2_Pin GPIO_PIN_6
#define PIN_MS2_GPIO_Port GPIOA
#define PIN_MS3_Pin GPIO_PIN_7
#define PIN_MS3_GPIO_Port GPIOA
#define PIN_LED_Pin GPIO_PIN_1
#define PIN_LED_GPIO_Port GPIOB
#define PIN_DIR_Pin GPIO_PIN_9
#define PIN_DIR_GPIO_Port GPIOA
#define PIN_STEP_Pin GPIO_PIN_10
#define PIN_STEP_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
