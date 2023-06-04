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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "urldecode.h"
#include <stdio.h>/* для printf */
#include <stdlib.h>
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  printf("URL Decoder Test\n");
	  printf("================\n\n");
	  /* My TEST */
	  printf("Test Zerg:  \n");
	  printf("-------------------------------\n");
	  printf("Input:           myhome/groundfloor/24\n");
	  printf("Expected Output: a myhome/groundfloor/24)\n");
	  char *my_test = urlDecode("myhome/groundfloor/24");
	  printf("Actual Output:   %s\n\n\n", my_test);
	  free(my_test);


	  /* TEST 1 */
	  printf("Test 1: Just a space\n");
	  printf("--------------------\n");
	  printf("Input:           %%20\n");
	  printf("Expected Output:  \n");
	  char *test1 = urlDecode("%20");
	  printf("Actual Output:   %s\n\n\n", test1);
	  free(test1);


	  /* TEST 2 */
	  printf("Test 2: A string with a space\n");
	  printf("-----------------------------\n");
	  printf("Input:           ASDF%%20ASDF\n");
	  printf("Expected Output: ASDF ASDF\n");
	  char *test2 = urlDecode("ASDF%20ASDF");
	  printf("Actual Output:   %s\n\n\n", test2);
	  free(test2);


	  /* TEST 3 */
	  printf("Test 3: Just a %% symbol\n");
	  printf("------------------------\n");
	  printf("Input:           %%\n");
	  printf("Expected Output: %%\n");
	  char *test3 = urlDecode("%");
	  printf("Actual Output:   %s\n\n\n", test3);
	  free(test3);


	  /* TEST 4 */
	  printf("Test 4: A string with all escaped characters\n");
	  printf("--------------------------------------------\n");
	  printf("Input:           %%20%%21%%22%%23%%24%%25%%26%%27%%28%%29\n");
	  printf("Expected Output:  !\"#$&'()\n");
	  char *test4 = urlDecode("%20%21%22%23%24%26%27%28%29");
	  printf("Actual Output:   %s\n\n\n", test4);
	  free(test4);


	  /* TEST 5 */
	  printf("Test 5: A string with doubly escaped characters\n");
	  printf("-----------------------------------------------\n");
	  printf("Input:           %%2520%%2524%%2523\n");
	  printf("Expected Output:  $#\n");
	  char *test5 = urlDecode("%2520%2524%2523");
	  printf("Actual Output:   %s\n\n\n", test5);
	  free(test5);


	  /* TEST 6 */
	  printf("Test 6: An empty string\n");
	  printf("-----------------------\n");
	  printf("Input:           \n");
	  printf("Expected Output: \n");
	  char *test6 = urlDecode("");
	  printf("Actual Output:   %s\n\n\n", test6);
	  free(test6);


	  /* TEST 7 */
	  printf("Test 7: A number of percent signs\n");
	  printf("---------------------------------\n");
	  printf("Input:           %%%%%%%%\n");
	  printf("Expected Output: %%%%%%%%\n");
	  char *test7 = urlDecode("%%%%");
	  printf("Actual Output:   %s\n\n\n", test7);
	  free(test7);


	  /* TEST 8 */
	  printf("Test 8: A number of percent signs followed by an encoded symbol\n");
	  printf("---------------------------------------------------------------\n");
	  printf("Input:           %%%%%%%%%%29\n");
	  printf("Expected Output: %%%%%%%%)\n");
	  char *test8 = urlDecode("%%%%%29");
	  printf("Actual Output:   %s\n\n\n", test8);
	  free(test8);


	  /* TEST 9 */
	  printf("Test 9: A mix of +'s for spaces(which don't get decoded)\n");
	  printf("-------------------------------\n");
	  printf("Input:           a%%20b+c+d");
	  printf("Expected Output: a b+c+d)\n");
	  char *test9 = urlDecode("a%20b+c+d");
	  printf("Actual Output:   %s\n\n\n", test9);
	  free(test9);

	  printf("Testing is complete!\n\n\n");

	  HAL_Delay(5000);
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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
