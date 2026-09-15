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
#include "stm32g4xx_hal.h"

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
#define IN_S4_Pin GPIO_PIN_13
#define IN_S4_GPIO_Port GPIOC
#define IN_S5_Pin GPIO_PIN_14
#define IN_S5_GPIO_Port GPIOC
#define IN_S6_Pin GPIO_PIN_15
#define IN_S6_GPIO_Port GPIOC
#define ADC1_BACK_1_Pin GPIO_PIN_0
#define ADC1_BACK_1_GPIO_Port GPIOA
#define ADC1_FRONT_1_1_Pin GPIO_PIN_1
#define ADC1_FRONT_1_1_GPIO_Port GPIOA
#define ADC1_FRONT_2_1_Pin GPIO_PIN_2
#define ADC1_FRONT_2_1_GPIO_Port GPIOA
#define ADC2_BACK_2_Pin GPIO_PIN_4
#define ADC2_BACK_2_GPIO_Port GPIOA
#define ADC2_FRONT_1_2_Pin GPIO_PIN_5
#define ADC2_FRONT_1_2_GPIO_Port GPIOA
#define ADC2_FRONT_2_2_Pin GPIO_PIN_6
#define ADC2_FRONT_2_2_GPIO_Port GPIOA
#define ADC3_FRONT_1_3_Pin GPIO_PIN_0
#define ADC3_FRONT_1_3_GPIO_Port GPIOB
#define ADC3_FRONT_2_3_Pin GPIO_PIN_1
#define ADC3_FRONT_2_3_GPIO_Port GPIOB
#define FLT_S1_Pin GPIO_PIN_2
#define FLT_S1_GPIO_Port GPIOB
#define EFUSE_EN1_Pin GPIO_PIN_10
#define EFUSE_EN1_GPIO_Port GPIOB
#define EFUSE_EN2_Pin GPIO_PIN_11
#define EFUSE_EN2_GPIO_Port GPIOB
#define EFUSE_EN3_Pin GPIO_PIN_12
#define EFUSE_EN3_GPIO_Port GPIOB
#define EFUSE_EN4_Pin GPIO_PIN_13
#define EFUSE_EN4_GPIO_Port GPIOB
#define EFUSE_EN5_Pin GPIO_PIN_14
#define EFUSE_EN5_GPIO_Port GPIOB
#define EFUSE_EN6_Pin GPIO_PIN_15
#define EFUSE_EN6_GPIO_Port GPIOB
#define IN_S1_Pin GPIO_PIN_6
#define IN_S1_GPIO_Port GPIOC
#define IN_S2_Pin GPIO_PIN_10
#define IN_S2_GPIO_Port GPIOC
#define IN_S3_Pin GPIO_PIN_11
#define IN_S3_GPIO_Port GPIOC
#define FLT_S2_Pin GPIO_PIN_3
#define FLT_S2_GPIO_Port GPIOB
#define FLT_S3_Pin GPIO_PIN_4
#define FLT_S3_GPIO_Port GPIOB
#define FLT_S4_Pin GPIO_PIN_5
#define FLT_S4_GPIO_Port GPIOB
#define FLT_S5_Pin GPIO_PIN_6
#define FLT_S5_GPIO_Port GPIOB
#define FLT_S6_Pin GPIO_PIN_7
#define FLT_S6_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
