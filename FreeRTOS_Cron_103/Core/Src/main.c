/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
/*------------- Crone ---------------*/
#include <time.h>
#include <stdlib.h>/* для atoi */
#include <stdio.h> /* для printf */
#include "lwdtc.h"
#include "string.h"
struct tm *timez;
time_t cronetime;
time_t cronetime_old;
lwdtc_cron_ctx_t cron_ctx = { 0 };
int i = 0;
int8_t k = -1; // номер елемента в котором находиться "р" - пауза
extern struct dcCron dcCrontxt[MAXSIZE];
char str[20] = {'\0',};
int myNumbers = -1;
uint32_t timestamp;
uint32_t timestampold;
/*--------- End crone -------------*/
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

UART_HandleTypeDef huart1;

osThreadId my_Cron_TaskHandle;
osThreadId my_Action_TaskHandle;
osMessageQId myQueueHandle;
uint8_t myQueueBuffer[ 32 * sizeof( uint8_t ) ];
osStaticMessageQDef_t myQueueControlBlock;
/* USER CODE BEGIN PV */
RTC_TimeTypeDef sTime = { 0 };
RTC_DateTypeDef sDate = { 0 };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
void Start_Cron_Task(void const * argument);
void Start_Action_Task(void const * argument);

/* USER CODE BEGIN PFP */
int _write(int file, char *ptr, int len) {
	int i;
	HAL_UART_Transmit(&huart1, (uint8_t*) ptr, len, 50);
	for (i = 0; i < len; i++) {
		ITM_SendChar(*ptr++);
	}
	return len;
}
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
  MX_RTC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  printf("Hello world! \r\n");
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of myQueue */
  osMessageQStaticDef(myQueue, 32, uint8_t, myQueueBuffer, &myQueueControlBlock);
  myQueueHandle = osMessageCreate(osMessageQ(myQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of my_Cron_Task */
  osThreadDef(my_Cron_Task, Start_Cron_Task, osPriorityNormal, 0, 256);
  my_Cron_TaskHandle = osThreadCreate(osThread(my_Cron_Task), NULL);

  /* definition and creation of my_Action_Task */
  osThreadDef(my_Action_Task, Start_Action_Task, osPriorityNormal, 0, 256);
  my_Action_TaskHandle = osThreadCreate(osThread(my_Action_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x15;
  sTime.Minutes = 0x10;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  DateToUpdate.Month = RTC_MONTH_APRIL;
  DateToUpdate.Date = 0x5;
  DateToUpdate.Year = 0x23;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

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
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_Start_Cron_Task */
/**
  * @brief  Function implementing the my_Cron_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Start_Cron_Task */
void Start_Cron_Task(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
	/* Set array of context objects */
	static lwdtc_cron_ctx_t cron_ctxs[MAXSIZE];

	/* Define context for CRON, used to parse data to */
	size_t fail_index;
	printf("Count task %d\r\n", LWDTC_ARRAYSIZE(dcCrontxt));
	/* Parse all cron strings */
	if (lwdtc_cron_parse_multi(cron_ctxs, dcCrontxt, MAXSIZE, &fail_index)
			!= lwdtcOK) {
		printf("Failed to parse cron at index %d\r\n", (int) fail_index);
	}
	printf("CRONs parsed and ready to go\r\n");
	/**************************************************/
	struct tm stm;

  for(;;)
  {
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

		stm.tm_year = sDate.Year + 100; //RTC_Year rang 0-99,but tm_year since 1900
		stm.tm_mon = sDate.Month - 1; //RTC_Month rang 1-12,but tm_mon rang 0-11
		stm.tm_mday = sDate.Date; //RTC_Date rang 1-31 and tm_mday rang 1-31
		stm.tm_hour = sTime.Hours;   //RTC_Hours rang 0-23 and tm_hour rang 0-23
		stm.tm_min = sTime.Minutes; //RTC_Minutes rang 0-59 and tm_min rang 0-59
		stm.tm_sec = sTime.Seconds;
		cronetime = mktime(&stm);
		timestamp= mktime(&stm); //1680707405

		if (cronetime != cronetime_old) {
			cronetime_old = cronetime;
			timez = localtime(&cronetime);
			i = 0;

			/* Check if CRON should execute */
			while (i < LWDTC_ARRAYSIZE(cron_ctxs)) {
				if (lwdtc_cron_is_valid_for_time(timez, cron_ctxs, &i)== lwdtcOK) {
					//printf("Executing CRON task %s ACTIVE %s ID %d \r\n",dcCrontxt[i].cron, dcCrontxt[i].activ, i);
					strcpy(str, dcCrontxt[i].activ);
//					if(i==0){
//					//HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//					GPIOC->ODR ^= GPIO_PIN_13; // toggle PA1
//					}
//					elmnt[i] = i;
					xQueueSend(myQueueHandle, &i, 0);
				}
				i++;
//			    printf("Get UNIX timestamp = %ld \r\n",timestamp); // timestamp = 1680707406
//				/*++++++++++++++++++++++++++++++++++*/
			}
		}
		//    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Start_Action_Task */
/**
* @brief Function implementing the my_Action_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Action_Task */
void Start_Action_Task(void const *argument) {
	/* USER CODE BEGIN Start_Action_Task */
	/* Infinite loop */
	uint8_t element; // Строка в cron массиве.
//	uint8_t messagesWaiting = uxQueueMessagesWaiting(myQueueHandle);
//	uint8_t emptySpaces = uxQueueSpacesAvailable(myQueueHandle);
	char str[20];
	int i = 0;
	for (;;) {

		if (xQueueReceive(myQueueHandle, &element, portMAX_DELAY) == pdTRUE) {/* portMAX_DELAY - Если очередь пустая то, Task будет ждать пока не появятся данные!  == pdTRUE - проверка на ошибки при получении!*/
//			printf("Messages waiting: %d \n", messagesWaiting);
//			printf("Empty spaces: %d \n", emptySpaces);
//			printf("Queue value is: %d \n", element);  /*где “%d” смотри Команды форматирования для printf()для с*/
			if (element < 5) {
				printf("element %d \n\r", element);
				strcpy(str, dcCrontxt[element].activ);
//				printf("++++++++ %s ------ %s \n\r", dcCrontxt[element].activ,str);
				// парсинг сценария
				k = -1;
				char *pch = strtok(str, ";");
				while (pch != NULL) {
					k ++;
					// если в строке "p" то это пауза
					// делаем паузу
					if (pch[0] == 'p') {
						char *newstring = pch + 1;
//						printf("Pause %d seconds & ++ k ++ = %d \r\n", atoi(newstring),k);
						printf("?????????? Start pause... %d\n\r",element);
//						HAL_Delay((atoi(newstring) * 1000));
						dcCrontxt[element].ptime = timestamp + atoi(newstring);
						dcCrontxt[element].elmnt = k;
//						printf("ptim - %ld , elmnt = %d\n\r", dcCrontxt[element].ptime,dcCrontxt[element].elmnt);
						printf("dcCrontxt[element].ptime = %ld \n\r", dcCrontxt[element].ptime);
						break;
					} else {
						// управляем пинами
						printf("Actions in pins %s \n", pch);
					}
					pch = strtok(NULL, ";");
				}
			}

		}
		if (timestamp != timestampold) {
			timestampold = timestamp;
			while (i < LWDTC_ARRAYSIZE(dcCrontxt)) {
//				printf("i %d \n", i);
//				printf("timestamp %ld &&&&& dcCrontxt[element].ptime ========= %ld \n\r",timestamp, dcCrontxt[i].ptime);
				if(timestampold >= dcCrontxt[i].ptime && dcCrontxt[i].ptime != 0){
					printf("dcCrontxt[i].elmnt %d \n",dcCrontxt[i].elmnt);
					printf("?????????? END pause! %d\n\r",i);
					dcCrontxt[i].ptime = 0;
				}
				i++;
			}
			i=0;
		}
//		osDelay(1);
	}
	/* USER CODE END Start_Action_Task */
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
