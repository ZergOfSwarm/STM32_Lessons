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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include <time.h>

/* Global vars for timestamp */
#include "stdio.h"
#include "string.h"
#include <time.h>
#include "stdbool.h" // Для flag
unsigned long Previoustime = 0;
const long Interval = 3;

/* Global vars for timestamp */
RTC_TimeTypeDef currentTime;
RTC_DateTypeDef currentDate;
time_t timestamp;
struct tm currTime;

RTC_TimeTypeDef Time1;
RTC_DateTypeDef Date1;
time_t timestamp1;
struct tm stTime1;

RTC_TimeTypeDef Time2;
RTC_DateTypeDef Date2;
time_t timestamp2;
struct tm stTime2;

#include "stdbool.h"
unsigned long S;
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
RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart6;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for my_RTC_Task */
osThreadId_t my_RTC_TaskHandle;
const osThreadAttr_t my_RTC_Task_attributes = {
  .name = "my_RTC_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for my_Timers_Task */
osThreadId_t my_Timers_TaskHandle;
const osThreadAttr_t my_Timers_Task_attributes = {
  .name = "my_Timers_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* USER CODE BEGIN PV */
RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef DateToUpdate = {0};
RTC_AlarmTypeDef sAlarm = {0}; // структура будильника

char trans_str[64] = {0,};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_USART6_UART_Init(void);
void StartDefaultTask(void *argument);
void Start_RTC_Task(void *argument);
void Start_Timers_Task(void *argument);

/* USER CODE BEGIN PFP */
int _write(int file, char *ptr, int len) {
	int i;
	HAL_UART_Transmit(&huart6, (uint8_t*) ptr, len, 50);
	for (i = 0; i < len; i++) {
		ITM_SendChar(*ptr++);
	}
	return len;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void print_info (void){

	  	  /*---------------------------------- Get timestamp for currentTime ----------------------------------------*/
	  	  	  HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
	  	  	  HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);

	  	  	  currTime.tm_year = currentDate.Year + 100;  // In fact: 2000 + 18 - 1900
	  	  	  currTime.tm_mday = currentDate.Date;
	  	  	  currTime.tm_mon  = currentDate.Month - 1;

	  	  	  currTime.tm_hour = currentTime.Hours;
	  	  	  currTime.tm_min  = currentTime.Minutes;
	  	  	  currTime.tm_sec  = currentTime.Seconds;

	  	  	  timestamp = mktime(&currTime);
	  	  	  //printf ( "Current SECONDS: %f\n", timestamp );
	  	  	  printf ( "Current date and time are: %s\n", ctime (&timestamp) ); // Convert time_t value to string

	  	  /*---------------------------------- Get timestamp for Time1 ----------------------------------------*/
	  	  		HAL_RTC_GetTime(&hrtc, &Time1, RTC_FORMAT_BIN);
	  	  		HAL_RTC_GetDate(&hrtc, &Date1, RTC_FORMAT_BIN);

	  	  		stTime1.tm_year = Date1.Year + 100;  // In fact: 2000 + 18 - 1900
	  	  		stTime1.tm_mday = Date1.Date;
	  	  		stTime1.tm_mon = Date1.Month - 1;

	  	  		stTime1.tm_hour = Time1.Hours;
	  	  		stTime1.tm_min = Time1.Minutes;
	  	  		stTime1.tm_sec = Time1.Seconds;

	  	  		timestamp1 = mktime(&stTime1);
	  	  		printf ( "Time1: %s", ctime (&timestamp1) ); // Convert Time1 value to string

	  	  /*---------------------------------- Get timestamp for Time2 ----------------------------------------*/
	  	  		HAL_RTC_GetTime(&hrtc, &Time2, RTC_FORMAT_BIN);
	  	  		HAL_RTC_GetDate(&hrtc, &Date2, RTC_FORMAT_BIN);

	  	  		stTime2.tm_year = Date2.Year + 100;  // In fact: 2000 + 18 - 1900
	  	  		stTime2.tm_mday = Date2.Date;
	  	  		stTime2.tm_mon = Date1.Month - 1;

	  	  		stTime2.tm_hour = Time2.Hours;
	  	  		stTime2.tm_min = Time2.Minutes;
	  	  		stTime2.tm_sec = 3; //Time2.Seconds;

	  	  		timestamp2 = mktime(&stTime2);
	  	  		printf ( "Time2: %s\n", ctime (&timestamp2) ); // Convert Time2 value to string

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
  MX_RTC_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */

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

  /* creation of my_RTC_Task */
  my_RTC_TaskHandle = osThreadNew(Start_RTC_Task, NULL, &my_RTC_Task_attributes);

  /* creation of my_Timers_Task */
  my_Timers_TaskHandle = osThreadNew(Start_Timers_Task, NULL, &my_Timers_Task_attributes);

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
  while (1)
  {
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
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
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 10;
  sTime.Minutes = 15;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_AUGUST;
  sDate.Date = 8;
  sDate.Year = 22;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 10;
  sAlarm.AlarmTime.Minutes = 15;
  sAlarm.AlarmTime.Seconds = 7;
  sAlarm.AlarmTime.SubSeconds = 0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PD0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
        snprintf(trans_str, 63, "++++++++++ Alarm_A +++++++++\n");
        HAL_UART_Transmit(&huart6, (uint8_t*)trans_str, strlen(trans_str), 1000);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_SET);
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
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Start_RTC_Task */
/**
* @brief Function implementing the my_RTC_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_RTC_Task */
void Start_RTC_Task(void *argument)
{
  /* USER CODE BEGIN Start_RTC_Task */
  /* Infinite loop */
  for(;;)
  {
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); // RTC_FORMAT_BIN , RTC_FORMAT_BCD
      snprintf(trans_str, 63, "Curent time is %d:%d:%d\n", sTime.Hours, sTime.Minutes, sTime.Seconds);
      //snprintf(trans_str, 63, "Time %d:%d:%d:%lu\n", sTime.Hours, sTime.Minutes, sTime.Seconds, sTime.SubSeconds);
      HAL_UART_Transmit(&huart6, (uint8_t*)trans_str, strlen(trans_str), 1000);

      HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
      snprintf(trans_str, 63, "Date %d-%d-20%d\n", DateToUpdate.Date, DateToUpdate.Month, DateToUpdate.Year);
      HAL_UART_Transmit(&huart6, (uint8_t*)trans_str, strlen(trans_str), 1000);

      HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);
      snprintf(trans_str, 63, "Settings of alarm %d:%d:%d\n", sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds);
      HAL_UART_Transmit(&huart6, (uint8_t*)trans_str, strlen(trans_str), 1000);

    osDelay(1000);
  }
  /* USER CODE END Start_RTC_Task */
}

/* USER CODE BEGIN Header_Start_Timers_Task */
/**
* @brief Function implementing the my_Timers_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Timers_Task */
void Start_Timers_Task(void *argument)
{
  /* USER CODE BEGIN Start_Timers_Task */
  /* Infinite loop */

	for (;;) {
/*---------------------------- Get UNIX timestamp from RTC --------------------------------*/
		HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);

		currTime.tm_year = currentDate.Year + 100;  // In fact: 2000 + 18 - 1900
		currTime.tm_mday = currentDate.Date;
		currTime.tm_mon = currentDate.Month - 1;

		currTime.tm_hour = currentTime.Hours;
		currTime.tm_min = currentTime.Minutes;
		currTime.tm_sec = currentTime.Seconds;

		timestamp = mktime(&currTime);
		//printf ( "Current SECONDS: %f\n", timestamp );
		//printf ( "Current date and time are: %s\n", ctime (&timestamp) ); // Convert time_t value to string

/*---------------------------------- Get timestamp for Time1 ----------------------------------------*/
		HAL_RTC_GetTime(&hrtc, &Time1, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &Date1, RTC_FORMAT_BIN);

		stTime1.tm_year = Date1.Year + 100;  // In fact: 2000 + 18 - 1900
		stTime1.tm_mday = Date1.Date;
		stTime1.tm_mon = Date1.Month - 1;

		stTime1.tm_hour = Time1.Hours;
		stTime1.tm_min = Time1.Minutes;
		stTime1.tm_sec = Time1.Seconds;

		timestamp1 = mktime(&stTime1);
		//printf("Time1: %s", ctime(&timestamp1)); // Convert Time1 value to string

/*---------------------------------- Get timestamp for Time2 ----------------------------------------*/
		HAL_RTC_GetTime(&hrtc, &Time2, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &Date2, RTC_FORMAT_BIN);

		stTime2.tm_year = Date2.Year + 100;  // In fact: 2000 + 18 - 1900
		stTime2.tm_mday = Date2.Date;
		stTime2.tm_mon = Date1.Month - 1;

		stTime2.tm_hour = Time2.Hours;
		stTime2.tm_min = Time2.Minutes;
		stTime2.tm_sec = 10; //Time2.Seconds;

		timestamp2 = mktime(&stTime2);
		//printf("Time2: %s\n", ctime(&timestamp2)); // Convert Time2 value to string

		double interval = difftime(timestamp, timestamp2);

		if(interval == 0.00){
		printf("Srabotal timer! \n");
		}
/*---------------------------------- Toggle timer ----------------------------------------*/
		unsigned long currentTime = timestamp;

		if (currentTime - Previoustime >= Interval) {  // Переключаем согласно "Interval".
			Previoustime = currentTime;
			GPIOA->ODR ^= GPIO_PIN_1; // toggle PA1
			GPIOD->ODR ^= GPIO_PIN_0; // toggle PD0
		}

//		osDelay(1000);
	}
	/* USER CODE END Start_Timers_Task */
}

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
