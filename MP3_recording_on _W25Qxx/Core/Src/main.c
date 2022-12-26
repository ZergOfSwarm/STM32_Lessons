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
 *MP3_recording_on _W25Qxx.ioc
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>/* для printf */
#include "stdbool.h"
#include <w25qxx.h>
#include <string.h>
#define SIZE 524        // Укажи размер немного больще получаемых данных, для /0!
#define pagesize 256    // Укажи размер страницы
char MainBuf[SIZE] = { 0, };
char Rx_data[SIZE] = { 0, };

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
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
int z, a, n, j, x, y, flag = 0;

void printCharArray(char *arr, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		printf("%c, ", arr[i]);
	}
	printf("\n");
}

void W25Qxxx() {
	///////////////////////////////////// ИНИЦИАЛИЗАЦИЯ ФЛЕШКИ ////////////////////////////////
	W25qxx_Init();
	///////////////////////////////////// ОЧИСТКА ВСЕЙ ФЛЕШКИ ////////////////////////////////
	W25qxx_EraseChip();
	///////////////////////////////////// ОЧИСТКА БЛОКА ////////////////////////////////
//	  W25qxx_EraseBlock(0); // 65536 байт
	///////////////////////////////////// ОЧИСТКА СЕКТОРА ////////////////////////////////
	//W25qxx_EraseSector(0); // 4096 байт
	///////////////////////////////////// ЗАПИСЬ ПОБАЙТНО ////////////////////////////////
//	  uint8_t b0 = 's';
//	  uint8_t b1 = 't';
//	  uint8_t b2 = 'D';
//
//	  W25qxx_WriteByte(b0, 25);
//	  W25qxx_WriteByte(b1, 26);
//	  W25qxx_WriteByte(b2, 27);
	///////////////////////////////////// ЧТЕНИЕ ПОБАЙТНО ////////////////////////////////
//	  uint8_t buf[64] = {0,};
//
//	  W25qxx_ReadByte(&buf[0], 25);
//	  W25qxx_ReadByte(&buf[1], 26);
//	  W25qxx_ReadByte(&buf[2], 27);
	///////////////////////////////////// ЗАПИСЬ СТРАНИЦЫ ////////////////////////////////
//	  uint8_t w_buf[] = "Hello world!";
//	  W25qxx_WritePage(w_buf, 0, 0, 12);
	///////////////////////////////////// ЧТЕНИЕ СТРАНИЦЫ ////////////////////////////////
//	  uint8_t buf[256] = {0,}; // это буфер в который запишутся данные
//	  W25qxx_ReadPage(buf, 0, 0, 12);
	///////////////////////////////////// ЗАПИСЬ СЕКТОРА ////////////////////////////////
//	  W25qxx_WriteSector(w_buf, 0, 1350, 10);
	///////////////////////////////////// ЧТЕНИЕ СЕКТОРА ////////////////////////////////
//	  memset(buf, 0, 64);
//	  W25qxx_ReadSector(buf, 0, 1350, 10);
	///////////////////////////////////// ЗАПИСЬ БЛОКА ////////////////////////////////
//	  W25qxx_WriteBlock(w_buf, 0, 9350, 10);
	///////////////////////////////////// ЧТЕНИЕ БЛОКА ////////////////////////////////
//	  memset(buf, 0, 64);
//	  W25qxx_ReadBlock(buf, 0, 9350, 10);
	/////////////////////////////////// ЧТЕНИЕ ЛЮБОГО КОЛИЧЕСТВА БАЙТ /////////////////
//	  memset(buf, 0, 64);
//	  W25qxx_ReadBytes(buf, 9350, 10);
	/////////////////////////////////// СТЁРТА ЛИ СТРАНИЦА /////////////////////
//	  uint8_t clear = W25qxx_IsEmptyPage(0, 40);
	/////////////////////////////////// СТЁРТ ЛИ СЕКТОР //////////////////////
//	  clear = W25qxx_IsEmptySector(0, 1360);
	/////////////////////////////////// СТЁРТ ЛИ БЛОК ////////////////////////
//	  clear = W25qxx_IsEmptyBlock(0, 9360);
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_SPI2_Init();
	MX_USART3_UART_Init();
	/* USER CODE BEGIN 2 */

//	char page[SIZE] = { 0, };
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		HAL_UART_Receive(&huart3, (uint8_t*) Rx_data, SIZE, 1000); // Слушаем UART

		if (strlen((char*) Rx_data) != 0) { // Если есть данные то
			memcpy(MainBuf, Rx_data, SIZE); // Записываем Rx_data в переменную MainBuf
			printf("MainBuf - %s\r\n", (char*) MainBuf);
			memset(Rx_data, 0, SIZE); /* To empty a Rx_data array*/
			char w_buf[pagesize] = { 0, }; // Создали буфер в размер страницы
			W25Qxxx(); /* Инициализация W25Qxxx */
			HAL_Delay(10);
///////////////////////////////////// ЗАПИСЬ СТРАНИЦЫ ////////////////////////////////
			//Calculate length of array
			size_t n = strlen(MainBuf);			// Узнаем размер MainBuf
			printf("size of MainBuf - %d\r\n", n);

			// Computes quotient
			int quotient = (int) n / (int) pagesize; // Узнаем целое количесво PAGE's в массиве
			printf("quotient %d\r\n", quotient);
			// Computes remainder
			int remainder = (int) n % (int) pagesize; // Узнаем остаток от деления PAGE's в массиве
			printf("remainder %d\r\n", remainder);

			for (int q = 0; q < quotient; q++) {
				int i = q * pagesize; // При первом проходе это выражение = 0
				n = pagesize + i;
//				printf("i = %d\r\n", i);
//				printf("n = %d\r\n", n);

				for (z = i, a = 0; z < n, a < pagesize; z++, a++) {
					w_buf[a] = MainBuf[z]; // Copy elements of array into page.
				}
				printf("w_buf - %s\r\n", (char*) w_buf);
				W25qxx_WritePage(w_buf, q, 0, sizeof(w_buf));
				for (j = 0; j < pagesize; j++) { // Очистка w_buf
					w_buf[j] = 0;
//					printf("%d", w_buf[j]);
				}
//				printf("w_buf is cleared!\r\n");
//				printf("\r\n The number of elements in 'w_buf' = %d\r\n", j);
				printf("Page %d is DANE! \r\n", q);
			}

//			printf("remainder = %d\r\n", remainder);
			if (remainder != 0) { // Если остаток не равен нулю
				int i = quotient * pagesize; // При первом проходе это выражение = 0
				int s = i + remainder;
//				printf("i = %d\r\n", i);
//				printf("s = %d\r\n", s);
				for (z = i, a = 0; z < s; z++, a++) {
					w_buf[a] = MainBuf[z]; /* Copy elements of array into last page.*/
				}
				printf("This is the last loop and 'w_buf' has only these elements - %s\r\n", (char*) w_buf);
				W25qxx_WritePage(w_buf, quotient, 0, remainder);
				memset(w_buf, 0, pagesize); /* To empty a char array*/
				printf("Page %d (Last one) is DANE! \r\n", quotient);
				flag = 1;
			}

///////////////////////////////////// ЧТЕНИЕ СТРАНИЦЫ ////////////////////////////////
			if (flag == 1) {
				y = quotient;
				for (x = 0; x <= y; x++) {
//					printf("x = %d\r\n", x);
//					printf("y = %d\r\n", y);
					uint8_t buf[pagesize] = { 0, }; // Это буфер в который запишутся данные
				    W25qxx_ReadPage(buf, x, 0, pagesize);
					printf("Page %d read! \r\n", x);
				}
				flag = 0;
			}
//			uint8_t buf2[pagesize] = { 0, }; // это буфер-2 в который запишутся данные
//			W25qxx_ReadPage(buf2, 1, 0, SIZE);
//
//			uint8_t buf3[pagesize] = { 0, }; // это буфер-3 в который запишутся данные
//			W25qxx_ReadPage(buf3, 2, 0, SIZE);
		}
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void) {

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
	if (HAL_SPI_Init(&hspi2) != HAL_OK) {
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
static void MX_USART3_UART_Init(void) {

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
	if (HAL_UART_Init(&huart3) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART3_Init 2 */

	/* USER CODE END USART3_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin : LED_Pin */
	GPIO_InitStruct.Pin = LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
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
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
