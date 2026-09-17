/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fdcan.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CAN_VISION_MASK_ID  0x60
#define CAN_LINE_MASK_ID    0x50

#define COMMAND_SET_THRESHOLDS 0x20
#define COMMAND_SET_MODE       0x21

#define SCANNING 1
#define FAST     2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t counter = 0U; /* TODO: delete later. debug purpose only */
uint16_t adc1_buffer[3];
uint16_t adc2_buffer[3];
uint16_t adc3_buffer[2];
GPIO_PinState vision_buffer[6];

// Legacy Protocol Variables
uint8_t lineMeasurements[8] = {0};
uint8_t lineThresholds[8] = {20, 20, 20, 20, 20, 20, 20, 20};
uint8_t state = FAST;
uint8_t previousDetection = 0;
uint32_t lastSendTime = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Vision_Sensors_Read(void);
void Vision_Sensors_Handler(void);

void Line_Sensors_Handler(void);

void FDCAN_Transmit(uint32_t id, const uint8_t *buffer, uint8_t buffer_size);
void FDCAN_Restart(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_FDCAN1_Init();
  /* USER CODE BEGIN 2 */

  /* Enable Vision Sensors */
  HAL_GPIO_WritePin(EFUSE_EN1_GPIO_Port, EFUSE_EN1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EFUSE_EN2_GPIO_Port, EFUSE_EN2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EFUSE_EN3_GPIO_Port, EFUSE_EN3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EFUSE_EN4_GPIO_Port, EFUSE_EN4_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EFUSE_EN5_GPIO_Port, EFUSE_EN5_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EFUSE_EN6_GPIO_Port, EFUSE_EN6_Pin, GPIO_PIN_RESET);
   
  /* Calibrate the ADC */ 
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
 
  /* Start DMA acq */
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc1_buffer, 3);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc2_buffer, 3);
  HAL_ADC_Start_DMA(&hadc3, (uint32_t*)adc3_buffer, 2);

  /* Configure FDCAN to accept all incoming messages into FIFO0 */
  HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);
  HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);

  /* Start CAN and activate RX Interrupt */
  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) {
      Error_Handler();
  }
  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
      Error_Handler();
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    counter++; /* TODO: Delete later */
    Vision_Sensors_Read();
    Vision_Sensors_Handler();
    Line_Sensors_Handler();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
  * @brief  This function used to handle the line sensors
  * @retval None
  */
void Line_Sensors_Handler (void)
{
    // Compress 12-bit DMA (0-4095) down to 8-bit Legacy (0-255)
    lineMeasurements[0] = (uint8_t)(adc1_buffer[0] >> 4);
    lineMeasurements[1] = (uint8_t)(adc1_buffer[1] >> 4);
    lineMeasurements[2] = (uint8_t)(adc1_buffer[2] >> 4);
    lineMeasurements[3] = (uint8_t)(adc2_buffer[0] >> 4);
    lineMeasurements[4] = (uint8_t)(adc2_buffer[1] >> 4);
    lineMeasurements[5] = (uint8_t)(adc2_buffer[2] >> 4);
    lineMeasurements[6] = (uint8_t)(adc3_buffer[0] >> 4);
    lineMeasurements[7] = (uint8_t)(adc3_buffer[1] >> 4);

    if (state == SCANNING) {
        if (HAL_GetTick() - lastSendTime > 10) {
            FDCAN_Transmit(CAN_LINE_MASK_ID, lineMeasurements, 8);
            lastSendTime = HAL_GetTick();
        }
    } 
    else if (state == FAST) {
        uint8_t currentDetection = 0;
        
        for (uint8_t i = 0; i < 8; i++) {
            if (lineMeasurements[i] < lineThresholds[i]) {
                currentDetection |= (1 << i);
            }
        }

        // 3ms Debounce check
        if ((currentDetection != previousDetection) && (HAL_GetTick() - lastSendTime > 3)) {
            for (uint8_t i = 0; i < 8; i++) {
                uint8_t currBit = (currentDetection >> i) & 0x01;
                uint8_t prevBit = (previousDetection >> i) & 0x01;
                
                if (currBit != prevBit) {
                    FDCAN_Transmit(CAN_LINE_MASK_ID + i, &currBit, 1);
                }
            }
            previousDetection = currentDetection;
            lastSendTime = HAL_GetTick();
        }
    }
}

/**
  * @brief  This function used to handle the line sensors
  * @retval None
  */
void Vision_Sensors_Read (void)
{
  vision_buffer[0] = !HAL_GPIO_ReadPin(IN_S1_GPIO_Port, IN_S1_Pin); /* Temp not used. */
  vision_buffer[1] = !HAL_GPIO_ReadPin(IN_S2_GPIO_Port, IN_S2_Pin); /* Temp not used. */
  vision_buffer[2] = !HAL_GPIO_ReadPin(IN_S3_GPIO_Port, IN_S3_Pin); /* Temp not used. */
  vision_buffer[3] = !HAL_GPIO_ReadPin(IN_S4_GPIO_Port, IN_S4_Pin); 
  vision_buffer[4] = !HAL_GPIO_ReadPin(IN_S5_GPIO_Port, IN_S5_Pin);
  vision_buffer[5] = !HAL_GPIO_ReadPin(IN_S6_GPIO_Port, IN_S6_Pin); /* Temp not used. */
  
  return;
}

/**
  * @brief  This function used to handle the line sensors
  * @retval None
  */
void Vision_Sensors_Handler (void)
{
  static uint8_t previousDistance[6] = {0};
    static uint32_t lastDistTime[6] = {0};

    for (uint8_t i = 0; i < 6; i++) {
        uint8_t dist = (uint8_t)vision_buffer[i];
        
        if ((dist != previousDistance[i]) && (HAL_GetTick() - lastDistTime[i] > 10)) {
            previousDistance[i] = dist;
            lastDistTime[i] = HAL_GetTick();
            FDCAN_Transmit(CAN_VISION_MASK_ID + i, &dist, 1);
        }
    }

}

/**
  * @brief  Transmits an Extended ID FDCAN Message
  */
void FDCAN_Transmit(uint32_t id, const uint8_t *buffer, uint8_t buffer_size) 
{
    FDCAN_TxHeaderTypeDef TxHeader;
    TxHeader.Identifier = id;
    TxHeader.IdType = FDCAN_EXTENDED_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = (buffer_size == 8) ? FDCAN_DLC_BYTES_8 : FDCAN_DLC_BYTES_1;
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;

    // if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) > 0) {
    //     HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, buffer);
    // }

    // If adding the message fails (e.g., bus error or mailbox full), restart CAN
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, buffer) != HAL_OK) {
        FDCAN_Restart();
    }
}

/**
  * @brief  This function used to handle the line sensors
  * @retval None
  */
void FDCAN_Restart(void) 
{
    HAL_Delay(1);

    HAL_FDCAN_Stop(&hfdcan1);
    HAL_FDCAN_DeInit(&hfdcan1);
    
    // Re-initialize hardware using the CubeMX generated function
    MX_FDCAN1_Init();

    // Re-apply the filters (CRITICAL: DeInit wipes these out)
    HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);
    HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);

    HAL_FDCAN_Start(&hfdcan1);

    if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief  Listens for configuration commands from the main board
  */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) 
{
    // FDCAN_RxHeaderTypeDef RxHeader;
    // uint8_t RxData[8];

    // if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
    //     if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
            
    //         if ((RxHeader.Identifier & 0xFF) == CAN_LINE_MASK_ID) {
    //             // Command: Set Mode (0x21)
    //             if ((RxHeader.Identifier >> 8) == COMMAND_SET_MODE) {
    //                 state = RxData[0];
    //             } 
    //             // Command: Set Thresholds (0x20)
    //             else if ((RxHeader.Identifier >> 8) == COMMAND_SET_THRESHOLDS) {
    //                 for(int i = 0; i < 8; i++) {
    //                     lineThresholds[i] = RxData[i];
    //                 }
    //             }
    //         }
    //     }
    //     HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    // }
    FDCAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];

    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
        
        // If reading the message fails, restart CAN
        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
            FDCAN_Restart();
            return; // Exit the callback early since data is invalid
        }
            
        if ((RxHeader.Identifier & 0xFF) == CAN_LINE_MASK_ID) {
            // Command: Set Mode (0x21)
            if ((RxHeader.Identifier >> 8) == COMMAND_SET_MODE) {
                state = RxData[0];
            } 
            // Command: Set Thresholds (0x20)
            else if ((RxHeader.Identifier >> 8) == COMMAND_SET_THRESHOLDS) {
                for(int i = 0; i < 8; i++) {
                    lineThresholds[i] = RxData[i];
                }
            }
        }
        
        // If re-arming the interrupt fails, restart CAN
        if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
            FDCAN_Restart();
        }
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
