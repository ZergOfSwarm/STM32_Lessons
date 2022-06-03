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
#include <stdio.h>/* для printf */
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
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART6_UART_Init(void);
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
MenuButton_t hMenuButton;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
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
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(hMenuButton.buttonFlag.bit.B0)
		  {
			  if(MenuButton_Debounce())											//Apply de-bounce algorithm before taking any action
			  {
				  hMenuButton.buttonFlag.bit.B0 = RESET;						//Reset interrupt flag

				  if(hMenuButton.buttonTimerEnable != SET)						//Start timer when the menu button gets pressed for the first time
				  {
					  hMenuButton.buttonTimerEnable = SET;						//Enable software timer
					  hMenuButton.buttonStatus = MenuButtonStatus_oneClick;		//The menu button has been pressed for one time
					  setTimer(&hMenuButton.buttonTimer);						//Start software timer
				  }
				  else
				  {
					  hMenuButton.buttonStatus = MenuButtonStatus_doubleClick;	//If the timer is already started this means that the button is pressed for the second time
				  }
			  }
		  }
		  if((checkTimer(&hMenuButton.buttonTimer, 5 * hMenuButton.buttonHeldPressedCounter)) && ( hMenuButton.buttonStatus == MenuButtonStatus_oneClick))
		  {																		//If the menu button has been pressed, take logic sample every 10 ms from the button read pin
			  if(!HAL_GPIO_ReadPin(menu_button_GPIO_Port, menu_button_Pin))		//To check if the button is still pressed
			  {
				  ++hMenuButton.buttonHeldPressedCounter;						//Increment samples number
			  }
			  hMenuButton.buttonStatus = (hMenuButton.buttonHeldPressedCounter >= 65)  ? MenuButtonStatus_heldPressed : hMenuButton.buttonStatus;
		  }																		//If the button is held pressed change button status
		  if((checkTimer(&hMenuButton.buttonTimer, 350)) && hMenuButton.buttonTimerEnable)
		  {																		//Take decision after 300 ms whether one click, double click or held pressed event has occured
			  switch(hMenuButton.buttonStatus)
			  {
			  	  case	MenuButtonStatus_oneClick:
			  		printf("oneClick \r\n");
			  		  break;
			  	  case 	MenuButtonStatus_doubleClick:
			  		printf("doubleClick \r\n");
			  		 break;
			  	  case MenuButtonStatus_heldPressed:
			  		printf("heldPressed \r\n");
			  		 break;
			  	case MenuButtonStatus_notPressed:
			  		 break;

			  }
			  hMenuButton.buttonTimerEnable 		= RESET;					//Disable software timer
			  hMenuButton.buttonHeldPressedCounter 	= RESET;					//Reset counter
			  hMenuButton.buttonStatus 				= MenuButtonStatus_notPressed;
			  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	//button status is set to no press to avoid misbehavior of the button functionality
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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin : test_pin_Pin */
  GPIO_InitStruct.Pin = test_pin_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(test_pin_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : menu_button_Pin */
  GPIO_InitStruct.Pin = menu_button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(menu_button_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 4 */
uint8_t MenuButton_Debounce(void) //Menu Button debounce function
{
	uint8_t 		ret = DISABLE;
	static uint16_t Level = Restart_Level;
	if(!HAL_GPIO_ReadPin(menu_button_GPIO_Port, menu_button_Pin))
	{
		++Level;
	}
	else
	{
		--Level;
		if(Level <= Restart_Level)
		{
			Level = Restart_Level;
		}
	}
	if(Level >= 4000)
	{
		Level = Acceptance_Level;
		ret = ENABLE;
	}
	return ret;
}

void setTimer(uint32_t* timer)
{
	*timer = HAL_GetTick();
}
uint8_t checkTimer(uint32_t* timer, uint32_t msTime)
{
	uint8_t ret = RESET;
	ret = ((HAL_GetTick() - *timer) > msTime)  ? ENABLE : DISABLE;
	return ret;
}
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

