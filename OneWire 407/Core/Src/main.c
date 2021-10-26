/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "OneWire.h"
#include "DallasTemperature.h"
//#include <stdio.h> /* для printf */
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
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
OneWire_HandleTypeDef ow;
DallasTemperature_HandleTypeDef dt;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// function to print a device address
void printAddress(CurrentDeviceAddress deviceAddress) {
	for (uint8_t i = 0; i < 6; i++) {  // Указал количество датчиков 7шт.
		printf("0x%02X ", deviceAddress[i]);
		HAL_Delay(50); //Сделал паузу.
	}
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
	MX_USART2_UART_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */

	//printf("[%8lu] Debug UART1 is OK!\r\n", HAL_GetTick());
	char msg[100];
	sprintf(msg, "[%8lu] Debug UART1 is OK!\r\n", HAL_GetTick());
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);

	OW_Begin(&ow, &huart1);
	if (OW_Reset(&ow) == OW_OK) {
		//printf("[%8lu] OneWire devices are present :)\r\n", HAL_GetTick());
		sprintf(msg, "[%8lu] OneWire devices are present :)\r\n",HAL_GetTick());
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
	} else {
		//printf("[%8lu] OneWire no devices :(\r\n", HAL_GetTick());
		sprintf(msg, "[%8lu] OneWire no devices :\r\n", HAL_GetTick());
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
	}
	DT_SetOneWire(&dt, &ow);
	// arrays to hold device address
	CurrentDeviceAddress insideThermometer;
	// locate devices on the bus
	//printf("[%8lu] Locating devices...", HAL_GetTick());
	sprintf(msg, "[%8lu] Locating devices...", HAL_GetTick());
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
	DT_Begin(&dt);
	//printf("[%8lu] Found %d devices.\r\n", HAL_GetTick(), DT_GetDeviceCount(&dt));
	sprintf(msg, "[%8lu] Found %d devices.\r\n", HAL_GetTick(),DT_GetDeviceCount(&dt));
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
	if (!DT_GetAddress(&dt, insideThermometer, 0)) {
		//printf("[%8lu] Unable to find address for Device 0\r\n", HAL_GetTick());
		sprintf(msg, "[%8lu] Unable to find address for Device 0\r\n",HAL_GetTick());
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
		//printf("[%8lu] Device 0 Address: ", HAL_GetTick());
		sprintf(msg, "[%8lu] Device 0 Address: \r\n", HAL_GetTick());
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
		printAddress(insideThermometer);
		printf("\r\n");
	}
	// set the resolution to 12 bit (Each Dallas/Maxim device is capable of several different resolutions)
	DT_SetResolution(&dt, insideThermometer, 12, true);
	//printf("[%8lu] Device 0 Resolution: %d\r\n", HAL_GetTick(), DT_GetResolution(&dt, insideThermometer));
	sprintf(msg, "[%8lu] Device 0 Resolution: %d\r\n", HAL_GetTick(),DT_GetResolution(&dt, insideThermometer));
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);

	if (!DT_GetAddress(&dt, insideThermometer, 1)) {
		//printf("[%8lu] Unable to find address for Device 1\r\n", HAL_GetTick());
		sprintf(msg, "[%8lu] Unable to find address for Device 1\r\n",HAL_GetTick());
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
		//printf("[%8lu] Device 1 Address: ", HAL_GetTick());
		sprintf(msg, "[%8lu] Device 1 Address: \r\n", HAL_GetTick());
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
		printAddress(insideThermometer);
		printf("\r\n");
		// set the resolution to 12 bit (Each Dallas/Maxim device is capable of several different resolutions)
		DT_SetResolution(&dt, insideThermometer, 12, true);
		//printf("[%8lu] Device 1 Resolution: %d\r\n", HAL_GetTick(), DT_GetResolution(&dt, insideThermometer));
		sprintf(msg, "[%8lu] Device 1 Resolution: %d\r\n", HAL_GetTick(),DT_GetResolution(&dt, insideThermometer));
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
	}
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		// call DT_RequestTemperatures(&dt) to issue a global temperature
		// request to all devices on the bus
		//printf("[%8lu] Requesting temperatures...", HAL_GetTick());
		sprintf(msg, "[%8lu] Requesting temperatures...", HAL_GetTick());
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
		DT_RequestTemperatures(&dt); // Send the command to get temperatures
		//printf("\r\n[%8lu] DONE\r\n", HAL_GetTick());
		sprintf(msg, "\r\n[%8lu] DONE\r\n", HAL_GetTick());
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
		// After we got the temperatures, we can print them here.
		// We use the function ByIndex, and as an example get the temperature from the first sensor only.
		//printf("[%8lu] Temperature for the device 1 (index 0) is: %.2f\r\n", HAL_GetTick(), DT_GetTempCByIndex(&dt, 0));
		sprintf(msg,"[%8lu] Temperature for the device 1 (index 0) is: %.2f\r\n",HAL_GetTick(), DT_GetTempCByIndex(&dt, 0));
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
		//printf("[%8lu] Temperature for the device 2 (index 1) is: %.2f\r\n", HAL_GetTick(), DT_GetTempCByIndex(&dt, 1));
		sprintf(msg,"[%8lu] Temperature for the device 2 (index 1) is: %.2f\r\n",HAL_GetTick(), DT_GetTempCByIndex(&dt, 1));
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
		//printf("[%8lu] Temperature for the device 3 (index 2) is: %.2f\r\n", HAL_GetTick(), DT_GetTempCByIndex(&dt, 2));
		sprintf(msg,"[%8lu] Temperature for the device 3 (index 2) is: %.2f\r\n",HAL_GetTick(), DT_GetTempCByIndex(&dt, 2));
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
		//printf("[%8lu] Temperature for the device 4 (index 3) is: %.2f\r\n", HAL_GetTick(), DT_GetTempCByIndex(&dt, 3));
		sprintf(msg,"[%8lu] Temperature for the device 4 (index 3) is: %.2f\r\n",HAL_GetTick(), DT_GetTempCByIndex(&dt, 3));
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
		//printf("[%8lu] Temperature for the device 5 (index 4) is: %.2f\r\n", HAL_GetTick(), DT_GetTempCByIndex(&dt, 4));
		sprintf(msg,"[%8lu] Temperature for the device 5 (index 4) is: %.2f\r\n",HAL_GetTick(), DT_GetTempCByIndex(&dt, 4));
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
		//printf("[%8lu] Temperature for the device 6 (index 5) is: %.2f\r\n", HAL_GetTick(), DT_GetTempCByIndex(&dt, 5));
		sprintf(msg,"[%8lu] Temperature for the device 6 (index 5) is: %.2f\r\n",HAL_GetTick(), DT_GetTempCByIndex(&dt, 5));
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);
		//printf("[%8lu] Temperature for the device 7 (index 6) is: %.2f\r\n", HAL_GetTick(), DT_GetTempCByIndex(&dt, 6));
		sprintf(msg,"[%8lu] Temperature for the device 7 (index 6) is: %.2f\r\n",HAL_GetTick(), DT_GetTempCByIndex(&dt, 6));
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 500);

		HAL_Delay(2000);
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

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_HalfDuplex_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA2_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA2_Stream2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
	/* DMA2_Stream7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
