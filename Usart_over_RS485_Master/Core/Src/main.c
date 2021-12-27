/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include <stdio.h> /* для printf */
#include <string.h>
#define rx_buffer_size 64 // Размер буфера для входных данных, установи нужное значение!
uint8_t rx_buffer[rx_buffer_size] = { 0, }; // Буфер для входных данных.
uint16_t rx_buffer_len; // Переменная для разных длинн данных.
int flag = 0;
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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET); // По дефолту откоючаем LED
  HAL_GPIO_WritePin(Enable_GPIO_Port, Enable_Pin, GPIO_PIN_SET); // Enable pin always HIGH as Master writes data to Slave
  printf("Aplication is runing! \n\r");

	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE); // Флаг IDLE
	HAL_UART_Receive_IT(&huart2, rx_buffer, rx_buffer_size); // Запуск према данных
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) { // Усли нажали на кнопку то LED is ON.
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
			if (huart2.gState != HAL_UART_STATE_BUSY_TX) {
				memset(rx_buffer, '\0', sizeof(rx_buffer)); // Очистка массива в сокете т.е. 'rx_buffer'
				*rx_buffer = '7';
				rx_buffer_len = sizeof(*rx_buffer);
				HAL_UART_Transmit_IT(&huart2, rx_buffer, rx_buffer_len);
				flag = 1;
			}
		}

		if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) && flag == 1) {// Усли нажали на кнопку то LED is ON.
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
			if (huart2.gState != HAL_UART_STATE_BUSY_TX) {
				memset(rx_buffer, '\0', sizeof(rx_buffer)); // Очистка массива в сокете т.е. 'rx_buffer'
				*rx_buffer = '8';
				rx_buffer_len = sizeof(*rx_buffer);
				HAL_UART_Transmit_IT(&huart2, rx_buffer, rx_buffer_len);
				flag = 0;
			}
		}
		HAL_Delay(100);
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Enable_GPIO_Port, Enable_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BTN_Pin */
  GPIO_InitStruct.Pin = BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(BTN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Enable_Pin */
  GPIO_InitStruct.Pin = Enable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Enable_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/*------------------------Callback для флага IDLE---------------------------------------*/
void HAL_UART_IDLE_Callback(UART_HandleTypeDef *huart) { // Эта функция отслеживает окончание передачи!
	if (huart == &huart2) { //проверка, из нужного ли uart пришли данные
		printf("IDLE = OK\n\r");
		__HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE); // Что бы писало один раз "IDLE = OK"
		rx_buffer_len = rx_buffer_size - huart->RxXferCount;

		/*----------Действия с входящими данными, что получили то и отправляем!-----------*/
		if (huart2.gState != HAL_UART_STATE_BUSY_TX) {
			HAL_UART_Transmit_IT(&huart2, rx_buffer, rx_buffer_len);
			if (*rx_buffer == '1') { // Если прилитела 1-ка то включаем светодиод!
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			} else if (*rx_buffer == '0') { // Если прилитела 0-к то отключаем светодиод!
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			}
		} else {
		}
		/*----------основные действия с входящими данными-----------*/
		HAL_UART_AbortReceive_IT(&huart2);      // Без этих 2-х строк,
		__HAL_UART_CLEAR_IDLEFLAG(&huart2); // работать будет только один раз!

		__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
		HAL_UART_Receive_IT(&huart2, rx_buffer, rx_buffer_size);
	}
}

/*------------------------UART RxCpltCallback---------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart2) { //проверка, из нужного ли uart пришли данные
		printf("RxCpltCallback = OK\n\r");
		__HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);

		printf("rx_buffer is overflow! \n\r"); // Если объем данных превышает размер буфера для входных данных будет сообщение!

		HAL_UART_AbortReceive_IT(&huart2); // Нужно, что бы не суммировало сообщения! Без этих 3-х строк,
		__HAL_UART_CLEAR_IDLEFLAG(&huart2); // работать будет только один раз!
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE); // Сбрасываем флаг IDLE

		HAL_UART_Receive_IT(&huart2, rx_buffer, rx_buffer_size);// Снова в режим приёма.
	}
}
/*------------------------RxCpltCallback---------------------------------------*/
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM14) {
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

