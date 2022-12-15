/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "uartRingBufDMA.h"
#include <stdio.h>/* для printf */
#include "stdbool.h"
#include <w25qxx.h>
#include <string.h>
extern uint8_t MainBuf;
bool flag = 0;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
int _write(int file, char *ptr, int len) {
	int i;
	HAL_UART_Transmit(&huart3, (uint8_t*) ptr, len, 50);
	for (i = 0; i < len; i++) {
		ITM_SendChar(*ptr++);
	}
	return len;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void W25Qxxx()
{
	  HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Start W25QXX\n", 13, 1000);

	  ///////////////////////////////////// ИНИЦИАЛИЗАЦИЯ ФЛЕШКИ ////////////////////////////////
	  W25qxx_Init();

	  ///////////////////////////////////// ОЧИСТКА ВСЕЙ ФЛЕШКИ ////////////////////////////////
	  //W25qxx_EraseChip();

	  ///////////////////////////////////// ОЧИСТКА БЛОКА ////////////////////////////////
	  W25qxx_EraseBlock(0); // 65536 байт

	  ///////////////////////////////////// ОЧИСТКА СЕКТОРА ////////////////////////////////
	  //W25qxx_EraseSector(0); // 4096 байт

	  ///////////////////////////////////// ЗАПИСЬ ПОБАЙТНО ////////////////////////////////
	  uint8_t b0 = 's';
	  uint8_t b1 = 't';
	  uint8_t b2 = 'D';

	  W25qxx_WriteByte(b0, 25);
	  W25qxx_WriteByte(b1, 26);
	  W25qxx_WriteByte(b2, 27);

	  ///////////////////////////////////// ЧТЕНИЕ ПОБАЙТНО ////////////////////////////////
	  uint8_t buf[64] = {0,};

	  W25qxx_ReadByte(&buf[0], 25);
	  W25qxx_ReadByte(&buf[1], 26);
	  W25qxx_ReadByte(&buf[2], 27);

	  HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buf, strlen((char*)buf), 100);
	  HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"\r\n", 2, 100);

	  ///////////////////////////////////// ЗАПИСЬ СТРАНИЦЫ ////////////////////////////////
	  uint8_t w_buf[] = "istarik.ru";

	  W25qxx_WritePage(w_buf, 0, 28, 10);

	  ///////////////////////////////////// ЧТЕНИЕ СТРАНИЦЫ ////////////////////////////////
	  memset(buf, 0, 64);
	  W25qxx_ReadPage(buf, 0, 28, 10);

	  HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buf, strlen((char*)buf), 100);
	  HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"\r\n", 2, 100);

	  ///////////////////////////////////// ЗАПИСЬ СЕКТОРА ////////////////////////////////
	  W25qxx_WriteSector(w_buf, 0, 1350, 10);

	  ///////////////////////////////////// ЧТЕНИЕ СЕКТОРА ////////////////////////////////
	  memset(buf, 0, 64);
	  W25qxx_ReadSector(buf, 0, 1350, 10);

	  HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buf, strlen((char*)buf), 100);
	  HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"\r\n", 2, 100);

	  ///////////////////////////////////// ЗАПИСЬ БЛОКА ////////////////////////////////
	  W25qxx_WriteBlock(w_buf, 0, 9350, 10);

	  ///////////////////////////////////// ЧТЕНИЕ БЛОКА ////////////////////////////////
	  memset(buf, 0, 64);
	  W25qxx_ReadBlock(buf, 0, 9350, 10);

	  HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buf, strlen((char*)buf), 100);
	  HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"\r\n", 2, 100);


	  /////////////////////////////////// ЧТЕНИЕ ЛЮБОГО КОЛИЧЕСТВА БАЙТ /////////////////
	  memset(buf, 0, 64);
	  W25qxx_ReadBytes(buf, 9350, 10);

	  HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buf, strlen((char*)buf), 100);
	  HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"\r\n", 2, 100);


	  /////////////////////////////////// СТЁРТА ЛИ СТРАНИЦА /////////////////////
	  uint8_t clear = W25qxx_IsEmptyPage(0, 40);

	  if(clear) HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Clear P\n", 8, 100);
	  else HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Not clear P\n", 12, 100);

	  /////////////////////////////////// СТЁРТ ЛИ СЕКТОР //////////////////////
	  clear = W25qxx_IsEmptySector(0, 1360);

	  if(clear) HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Clear S\n", 8, 100);
	  else HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Not clear S\n", 12, 100);

	  /////////////////////////////////// СТЁРТ ЛИ БЛОК ////////////////////////
	  clear = W25qxx_IsEmptyBlock(0, 9360);

	  if(clear) HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Clear B\n", 8, 100);
	  else HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Not clear B\n", 12, 100);

}
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
  MX_SPI2_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	Ringbuf_Init();
	printf("Hello world!\r\n");
//	if (waitFor("hello", 6000) != 1) {
//		Error_Handler();
//	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//printf("MainBuf = %d\r\n", MainBuf);
		if (MainBuf == 55 && flag == 0) { // 7
			flag = 1;
			W25Qxxx();
			printf("Data is recorded to w25qxx! \r\n");
		} else {
		}
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : FLASH_CS_Pin */
  GPIO_InitStruct.Pin = FLASH_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FLASH_CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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

#ifdef  USE_FULL_ASSERT
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
