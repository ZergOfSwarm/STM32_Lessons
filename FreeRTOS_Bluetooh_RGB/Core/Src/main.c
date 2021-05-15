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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h> // Подключим библиотеку для работы со строками.
#include <stdio.h> /* для printf */
int brightness;
#include <stdlib.h> // Что бы не писало предупреждение в 117 строке (перевод из str в int)!
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
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for my_RGB_Task */
osThreadId_t my_RGB_TaskHandle;
const osThreadAttr_t my_RGB_Task_attributes = {
  .name = "my_RGB_Task",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for my_Bluetooth_Ta */
osThreadId_t my_Bluetooth_TaHandle;
const osThreadAttr_t my_Bluetooth_Ta_attributes = {
  .name = "my_Bluetooth_Ta",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* USER CODE BEGIN PV */
/* -------------------for Bluetooth----------------------------*/
char str1[60] = { 0 };  // Создадим строковую переменную

typedef struct USART_prop { // Создадим структуру
	uint8_t usart_buf[60];
	uint8_t usart_cnt;
	uint8_t is_tcp_connect; //статус попытки создать соединение TCP с сервером
	uint8_t is_text; //статус попытки передать текст серверу
} USART_prop_ptr;
USART_prop_ptr usartprop;
//***Добавим гл. переменную для счётчика выводимых в модуль строк, и массив этих самых строк ***//
uint8_t i = 0;
char *str2[] = { "String1\r\n", "String2\r\n", "String3\r\n", "String4\r\n",
		"String5\r\n" };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
void StartDefaultTask(void *argument);
void Start_RGB_Task(void *argument);
void Start_Bluetooth_Task(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*-----------------------------for prinf--------------------------------------*/
int _write(int file, uint8_t *ptr, int len) {
	for (int DataIdx = 0; DataIdx < len; DataIdx++) {
		ITM_SendChar(*ptr++);
	}
	return len;
}
/*-----------------------------for Bluetooth--------------------------------------*/
/*---------Добавим гл. фун-ю которая передает в USAR все, что приходит!--------*/
void string_parse(char *buf_str) //
{
	HAL_UART_Transmit(&huart2, (uint8_t*) buf_str, strlen(buf_str), 0x1000);
	printf("Get message: %s \n", buf_str);
	brightness = atoi(buf_str); // перевод str в число
	printf("brightness = %d \n", brightness);
}
/*------------Теперь добавим гл-й обработчик приёма символа из USART---------------------------------------------*/
void UART2_RxCpltCallback(void) // тут подробно https://youtu.be/S4P-lAa4iFQ?t=547
{
	uint8_t b; // Можно не создавать эту переменную!
	b = str1[0]; // Принимаем один байт и записываем в переменную "b".
	//если вдруг случайно превысим длину буфера
	if (usartprop.usart_cnt > 59) {
		usartprop.usart_cnt = 0; // то, обнуляем счетчик
		HAL_UART_Receive_IT(&huart2, (uint8_t*) str1, 1); // И снова даем команду получать данные из UART.
		return; // Выход из функции.
	}
	usartprop.usart_buf[usartprop.usart_cnt] = b; // Если не превышена длинна буфера то присваевыем данный байт к буферу UART.
	if (b == 0x0A) { // Если встретился символ перевода строки то, считаем, что строка закончилась и передаем ее на обработку.
		usartprop.usart_buf[usartprop.usart_cnt + 1] = 0; // В конец строки передаем "ноль" тем самым обозначим конец строки.
		string_parse((char*) usartprop.usart_buf); // передаем троку в "string_parse"
		usartprop.usart_cnt = 0; // Обнуляем счетчик
		HAL_UART_Receive_IT(&huart2, (uint8_t*) str1, 1); // И снова запускаем UART на прием.
		return;
	}
	usartprop.usart_cnt++; // Если не получили символ перевода строки то, наращиваем счетчик и продолжаем получать строки.
	HAL_UART_Receive_IT(&huart2, (uint8_t*) str1, 1); // Снова команда на примем байта.
}
/*-----------------------------for RGB--------------------------------------*/
void rgb_set(uint8_t red, uint8_t green, uint8_t blue) {
	htim3.Instance->CCR1 = 255 - red; // Если у нас RGB с общим катодом то приведи к такому виду htim2.Instance->CCR1 = red;
	htim3.Instance->CCR2 = 255 - green; // Если у нас RGB с общим катодом то приведи к такому виду htim2.Instance->CCR1 = green;
	htim3.Instance->CCR3 = 255 - blue; // Если у нас RGB с общим катодом то приведи к такому виду htim2.Instance->CCR1 = blue;
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
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	/*-----------------------------for Bluetooth--------------------------------------*/
	//************* Инициируем приём данных в USART, а также запустим наш таймер ***************//
	HAL_UART_Receive_IT(&huart2, (uint8_t*) str1, 1);
	//HAL_TIM_Base_Start_IT(&htim2);
	  /*-----------------------------for RGB--------------------------------------*/
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of my_RGB_Task */
  my_RGB_TaskHandle = osThreadNew(Start_RGB_Task, NULL, &my_RGB_Task_attributes);

  /* creation of my_Bluetooth_Ta */
  my_Bluetooth_TaHandle = osThreadNew(Start_Bluetooth_Task, NULL, &my_Bluetooth_Ta_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
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
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 1107-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 255-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_Pin|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/*-----------------------------for Bluetooth--------------------------------------*/
/***Добавим оф. обработчик приёма из USART, в котором вызовем наш "самодельный" обработчик***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart2) { // Проверка, точно ли данные со 2-го UART?
		UART2_RxCpltCallback(); // Вызов обработчика.
	}
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Start_RGB_Task */
/**
 * @brief Function implementing the my_RGB_Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_RGB_Task */
void Start_RGB_Task(void *argument)
{
  /* USER CODE BEGIN Start_RGB_Task */
	/* Infinite loop */
	for (;;) {
		rgb_set(brightness, 0, 0);   // red only. Для увеличения/уменьшения яркости LED меняй значения в скобках!
		osDelay(100);
/*
		rgb_set(0, brightness, 0);   // green only
		osDelay(3000);

		rgb_set(0, 0, brightness);   // blue only
		osDelay(3000);
		*/
	}
  /* USER CODE END Start_RGB_Task */
}

/* USER CODE BEGIN Header_Start_Bluetooth_Task */
/**
 * @brief Function implementing the my_Bluetooth_Ta thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_Bluetooth_Task */
void Start_Bluetooth_Task(void *argument)
{
  /* USER CODE BEGIN Start_Bluetooth_Task */
	/* Infinite loop */
	for (;;) {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); // Морагаем светодиодом
		HAL_UART_Transmit(&huart2, (uint8_t*) str2[i], strlen(str2[i]), 0x1000);
		i++;
		if (i > 4) {
			i = 0;
		}
		osDelay(3000);
	}
  /* USER CODE END Start_Bluetooth_Task */
}

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
