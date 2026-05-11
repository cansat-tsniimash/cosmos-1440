/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define muks_a_Pin GPIO_PIN_13
#define muks_a_GPIO_Port GPIOC
#define muks_b_Pin GPIO_PIN_14
#define muks_b_GPIO_Port GPIOC
#define muks_c_Pin GPIO_PIN_15
#define muks_c_GPIO_Port GPIOC
#define bus_pipipipipipi____Pin GPIO_PIN_0
#define bus_pipipipipipi____GPIO_Port GPIOA
#define M1_Pin GPIO_PIN_0
#define M1_GPIO_Port GPIOB
#define M0_Pin GPIO_PIN_1
#define M0_GPIO_Port GPIOB
#define ON_OFF_Pin GPIO_PIN_10
#define ON_OFF_GPIO_Port GPIOB
#define krimatoriy_Pin GPIO_PIN_11
#define krimatoriy_GPIO_Port GPIOB
#define GPIO_CS_SD_Pin GPIO_PIN_12
#define GPIO_CS_SD_GPIO_Port GPIOB
#define led_Pin GPIO_PIN_8
#define led_GPIO_Port GPIOA
#define DS18B20_Pin GPIO_PIN_15
#define DS18B20_GPIO_Port GPIOA
#define RF_AUX_Pin GPIO_PIN_3
#define RF_AUX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
