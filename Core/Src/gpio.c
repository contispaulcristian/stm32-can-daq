/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, EFUSE_EN3_Pin|EFUSE_EN6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, EFUSE_EN1_Pin|EFUSE_EN2_Pin|IN_S6_Pin|EFUSE_EN5_Pin
                          |EFUSE_EN4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : IN_S3_Pin FLT_S3_Pin FLT_S6_Pin IN_S5_Pin */
  GPIO_InitStruct.Pin = IN_S3_Pin|FLT_S3_Pin|FLT_S6_Pin|IN_S5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : EFUSE_EN3_Pin EFUSE_EN6_Pin */
  GPIO_InitStruct.Pin = EFUSE_EN3_Pin|EFUSE_EN6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : FLT_S1_Pin IN_S1_Pin FLT_S2_Pin IN_S2_Pin
                           FLT_S5_Pin IN_S4_Pin FLT_S4_Pin */
  GPIO_InitStruct.Pin = FLT_S1_Pin|IN_S1_Pin|FLT_S2_Pin|IN_S2_Pin
                          |FLT_S5_Pin|IN_S4_Pin|FLT_S4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : EFUSE_EN1_Pin EFUSE_EN2_Pin IN_S6_Pin EFUSE_EN5_Pin
                           EFUSE_EN4_Pin */
  GPIO_InitStruct.Pin = EFUSE_EN1_Pin|EFUSE_EN2_Pin|IN_S6_Pin|EFUSE_EN5_Pin
                          |EFUSE_EN4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
