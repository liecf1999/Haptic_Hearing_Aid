/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define A0_2_Pin GPIO_PIN_13
#define A0_2_GPIO_Port GPIOB
#define A1_2_Pin GPIO_PIN_14
#define A1_2_GPIO_Port GPIOB
#define A2_2_Pin GPIO_PIN_15
#define A2_2_GPIO_Port GPIOB
#define A0_1_Pin GPIO_PIN_6
#define A0_1_GPIO_Port GPIOC
#define A1_1_Pin GPIO_PIN_7
#define A1_1_GPIO_Port GPIOC
#define A2_1_Pin GPIO_PIN_8
#define A2_1_GPIO_Port GPIOC
#define DRIVE_EN_Pin GPIO_PIN_9
#define DRIVE_EN_GPIO_Port GPIOC
#define TRIG_Pin GPIO_PIN_11
#define TRIG_GPIO_Port GPIOA
#define N_RES_Pin GPIO_PIN_12
#define N_RES_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
