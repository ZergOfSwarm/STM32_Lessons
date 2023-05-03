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
#include <stdio.h>/* для printf */
#include <string.h>
/************** PWM *******************/
uint16_t pwmData[1];
/************** ADC *******************/
#define ADC_BUF_LED 100 //4096
uint16_t adc_buf[ADC_BUF_LED];

/************** Tahometr *******************/
volatile uint8_t count = 0;
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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim1_ch1;

UART_HandleTypeDef huart1;

osThreadId defaultTaskHandle;
uint32_t defaultTaskBuffer[ 256 ];
osStaticThreadDef_t defaultTaskControlBlock;
osThreadId my_PWM_TaskHandle;
uint32_t my_PWM_TaskBuffer[ 256 ];
osStaticThreadDef_t my_PWM_TaskControlBlock;
osThreadId my_ADC_TaskHandle;
uint32_t my_ADC_TaskBuffer[ 256 ];
osStaticThreadDef_t my_ADC_TaskControlBlock;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void const * argument);
void Start_PWM_Task(void const * argument);
void Start_ADC_Task(void const * argument);

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
/************** Медианный фильтр *******************/
float median(float newVal) {
	static float buf[3];
	static int count = 0;
	buf[count] = newVal;
	if (++count > 2)
		count = 0;

	float a = buf[0];
	float b = buf[1];
	float c = buf[2];

	float middle;
	if ((a <= b) && (a <= c)) {
		middle = (b <= c) ? b : c;
	} else {
		if ((b <= a) && (b <= c)) {
			middle = (a <= c) ? a : c;
		} else {
			middle = (a <= b) ? a : b;
		}
	}
	return middle;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim3) {
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
			count++;

			if (count == 1) {
				__HAL_TIM_SET_COUNTER(&htim3, 0x0000);
				__HAL_TIM_SET_COUNTER(&htim2, 0x0000);
			}

			else if (count == 2) {
				HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);

				count = 0;

				uint16_t count_main = HAL_TIM_ReadCapturedValue(&htim3,
						TIM_CHANNEL_1);
				uint16_t count_secondary = __HAL_TIM_GET_COUNTER(&htim2);
				uint16_t arr = __HAL_TIM_GET_AUTORELOAD(&htim2);
				uint32_t res = count_main + (count_secondary * arr)
						+ count_secondary;

				float freq = 1 / (res / 1000000.0);

				char str[96] = { 0, };
				//snprintf(str, 96, "Capture Freq: %.3f Hz | res: %lu | cm: %d | cs: %d\n", (float)freq, res, count_main, count_secondary);
				freq = median(freq);

				snprintf(str, 96, "Capture Freq: %.3f Hz\n", (float) freq);
				HAL_UART_Transmit(&huart1, (uint8_t*) str, strlen(str), 1000);

				HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
			}
		}
	}
}
/************** Button *******************/
MenuButton_t hMenuButton;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	/************** Button *******************/
	hMenuButton.buttonFlag.data = RESET; //Flag reset
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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

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

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadStaticDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256, defaultTaskBuffer, &defaultTaskControlBlock);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of my_PWM_Task */
  osThreadStaticDef(my_PWM_Task, Start_PWM_Task, osPriorityNormal, 0, 256, my_PWM_TaskBuffer, &my_PWM_TaskControlBlock);
  my_PWM_TaskHandle = osThreadCreate(osThread(my_PWM_Task), NULL);

  /* definition and creation of my_ADC_Task */
  osThreadStaticDef(my_ADC_Task, Start_ADC_Task, osPriorityNormal, 0, 256, my_ADC_TaskBuffer, &my_ADC_TaskControlBlock);
  my_ADC_TaskHandle = osThreadCreate(osThread(my_ADC_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	printf("Hello world! \r\n");
  /* USER CODE END RTOS_THREADS */

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 100-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
  sSlaveConfig.InputTrigger = TIM_TS_ITR2;
  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7199;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : menu_button_Pin */
  GPIO_InitStruct.Pin = menu_button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(menu_button_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
uint8_t MenuButton_Debounce(void) //Menu Button debounce function
{
	uint8_t ret = DISABLE;
	static uint16_t Level = Restart_Level;
	if (!HAL_GPIO_ReadPin(menu_button_GPIO_Port, menu_button_Pin)) {
		++Level;
	} else {
		--Level;
		if (Level <= Restart_Level) {
			Level = Restart_Level;
		}
	}
	if (Level >= 4000) {
		Level = Acceptance_Level;
		ret = ENABLE;
	}
	return ret;
}

void setTimer(uint32_t *timer) {
	*timer = HAL_GetTick();
}
uint8_t checkTimer(uint32_t *timer, uint32_t msTime) {
	uint8_t ret = RESET;
	ret = ((HAL_GetTick() - *timer) > msTime) ? ENABLE : DISABLE;
	return ret;
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
	/* Infinite loop */
	for (;;) {
		if (hMenuButton.buttonFlag.bit.B0) {
			if (MenuButton_Debounce())//Apply de-bounce algorithm before taking any action
			{
				hMenuButton.buttonFlag.bit.B0 = RESET;	//Reset interrupt flag

				if (hMenuButton.buttonTimerEnable != SET)//Start timer when the menu button gets pressed for the first time
						{
					hMenuButton.buttonTimerEnable = SET;//Enable software timer
					hMenuButton.buttonStatus = MenuButtonStatus_oneClick;//The menu button has been pressed for one time
					setTimer(&hMenuButton.buttonTimer);	//Start software timer
				} else {
					hMenuButton.buttonStatus = MenuButtonStatus_doubleClick;//If the timer is already started this means that the button is pressed for the second time
				}
			}
		}
		if ((checkTimer(&hMenuButton.buttonTimer,
				5 * hMenuButton.buttonHeldPressedCounter))
				&& (hMenuButton.buttonStatus == MenuButtonStatus_oneClick)) {//If the menu button has been pressed, take logic sample every 10 ms from the button read pin
			if (!HAL_GPIO_ReadPin(menu_button_GPIO_Port, menu_button_Pin))//To check if the button is still pressed
					{
				++hMenuButton.buttonHeldPressedCounter;	//Increment samples number
			}
			hMenuButton.buttonStatus =
					(hMenuButton.buttonHeldPressedCounter >= 65) ?
							MenuButtonStatus_heldPressed :
							hMenuButton.buttonStatus;
		}					//If the button is held pressed change button status
		if ((checkTimer(&hMenuButton.buttonTimer, 350))
				&& hMenuButton.buttonTimerEnable) {	//Take decision after 300 ms whether one click, double click or held pressed event has occured
			switch (hMenuButton.buttonStatus) {
			case MenuButtonStatus_oneClick:
				printf("oneClick \r\n");
				pwmData[0] += 1; // Уменьшаем значение pwmData[0] на -1
				break;
			case MenuButtonStatus_doubleClick:
				printf("doubleClick \r\n");
				if (pwmData[0] >= 100) {
					pwmData[0] = 100;
				} else {
					pwmData[0] += 100; // Увеличиваем значение pwmData[0] на +3
					if (pwmData[0] >= 100) {
						pwmData[0] = 100;
					}
				}
				break;
			case MenuButtonStatus_heldPressed:
				printf("heldPressed & pwmData[0] = 0\r\n");
				pwmData[0] = 0;
				break;
			case MenuButtonStatus_notPressed:
				break;

			}
			hMenuButton.buttonTimerEnable = RESET;		//Disable software timer
			hMenuButton.buttonHeldPressedCounter = RESET;		//Reset counter
			hMenuButton.buttonStatus = MenuButtonStatus_notPressed;
			//button status is set to no press to avoid misbehavior of the button functionality
		}
//    osDelay(1);
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Start_PWM_Task */
/**
 * @brief Function implementing the my_PWM_Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_PWM_Task */
void Start_PWM_Task(void const * argument)
{
  /* USER CODE BEGIN Start_PWM_Task */
	/************** PWM *******************/
	//	  pwmData[0] = 70;
	//	  pwmData[1] = 20;
	//	  pwmData[2] = 30;
	//	  pwmData[3] = 40;
	//	  pwmData[4] = 50;
	//	  pwmData[5] = 60;
	//	  pwmData[6] = 70;
	//	  pwmData[7] = 80;
	//	  pwmData[8] = 90;
	//	  pwmData[9] = 100;
	//	  HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
	/************** Map *******************/
//	      int16_t value = 0; // For function "map" from analogRead(0);
//	      int16_t in_max = 4050; // было 4095
//		    int8_t in_min = 0;  // было 0
//		    unsigned char out_max = 100;
//		    int8_t out_min = 0;
	/* Infinite loop */
	for (;;) {
//	    value = (adc_buf[0] - in_min) * (out_max - out_min) / (in_max - in_min) + out_min; // Function map
//	    pwmData[0] = value;
//	    pwmData[0] = value;
		HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*) pwmData, 1);
		printf("pwmData[0] = %d\r\n", pwmData[0]);
		osDelay(1000);
	}
  /* USER CODE END Start_PWM_Task */
}

/* USER CODE BEGIN Header_Start_ADC_Task */
/**
 * @brief Function implementing the my_ADC_Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_ADC_Task */
void Start_ADC_Task(void const * argument)
{
  /* USER CODE BEGIN Start_ADC_Task */
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_buf, ADC_BUF_LED);
	/* Infinite loop */
	for (;;) {
//	  printf("adc_buf = %u\r\n",(unsigned int)adc_buf[0]);
		osDelay(1000);
	}
  /* USER CODE END Start_ADC_Task */
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
