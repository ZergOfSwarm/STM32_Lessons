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
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lwip/apps/httpd.h" // including httpd.h [- HTTPd #1 -]
#include "string.h"  // we include String.h for the strcmp() function [= CGI #1 =]
#include "stdbool.h" // we include this library to be able to use boolean variables for SSI

bool LD1ON = true; // this variable will indicate if the LD1 LED on the board is ON or not
bool LD2ON = true; // this variable will indicate if our LD2 LED on the board is ON or not
bool LD3ON = true; // this variable will indicate if the LD3 LED on the board is ON or not
bool LD4ON = true; // this variable will indicate if the LD4 LED on the board is ON or not
bool LD5ON = true; // this variable will indicate if the LD5 LED on the board is ON or not

/*------------------------------------MQTT---------------------------------------------*/
#include "lwip_mqtt.h"
mqtt_client_t *client;
uint32_t cont = 0, blink = 0;
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
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */
// just declaring the function for the compiler [= CGI #2 =]
const char* LedCGIhandler(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]);
// in our SHTML file <form method="get" action="/leds.cgi"> [= CGI #3 =]
const tCGI LedCGI = { "/leds.cgi", LedCGIhandler };

// [= CGI #4 =]
tCGI theCGItable[1];
// just declaring SSI handler function [* SSI #1 *]
u16_t mySSIHandler(int iIndex, char *pcInsert, int iInsertLen);
// [* SSI #2 *]
#define numSSItags 5 // Указываем количество тэгов
// [* SSI #3 *]
char const *theSSItags[numSSItags] = { "tag1", "tag2", "tag3", "tag4", "tag5" };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_UART4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//turning the LED lights off
// the actual function for handling CGI [= CGI #5 =]
const char* LedCGIhandler(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]) {
	uint32_t i = 0;
	if (iIndex == 0) {
		//turning the White_Pin lights off
		HAL_GPIO_WritePin(White_GPIO_Port, White_Pin, GPIO_PIN_SET);
		// we put this variable to false to indicate that the LD1 LED on the board is not ON
		LD1ON = false; //Когда Led is off LD1ON = false!
		HAL_GPIO_WritePin(Green_GPIO_Port, Green_Pin, GPIO_PIN_SET);
		// we put this variable to false to indicate that the LD2 LED on the board is not ON
		LD2ON = false; //Когда Led is off LD1ON = false!
		HAL_GPIO_WritePin(Blue_GPIO_Port, Blue_Pin, GPIO_PIN_SET);
		// we put this variable to false to indicate that the LD3 LED on the board is not ON
		LD3ON = false; //Когда Led is off LD1ON = false!
		HAL_GPIO_WritePin(Red_GPIO_Port, Red_Pin, GPIO_PIN_SET);
		// we put this variable to false to indicate that the LD2 LED on the board is not ON
		LD4ON = false; //Когда Led is off LD1ON = false!
		HAL_GPIO_WritePin(Yellow_GPIO_Port, Yellow_Pin, GPIO_PIN_SET);
		// we put this variable to false to indicate that the LD2 LED on the board is not ON
		LD5ON = false; //Когда Led is off LD1ON = false!
	}
	for (i = 0; i < iNumParams; i++) {
		if (strcmp(pcParam[i], "led") == 0) {
			if (strcmp(pcValue[i], "1") == 0) {
				HAL_GPIO_WritePin(White_GPIO_Port, White_Pin, GPIO_PIN_RESET);
				// LD3 LED (red) on the board is ON!
				LD1ON = true; //Когда Led is ON LD1ON = true!
			} else if (strcmp(pcValue[i], "2") == 0) {
				HAL_GPIO_WritePin(Green_GPIO_Port, Green_Pin, GPIO_PIN_RESET);
				// LD2 LED (blue) on the board is ON!
				LD2ON = true; //Когда Led is ON LD1ON = true!
			} else if (strcmp(pcValue[i], "3") == 0) {
				HAL_GPIO_WritePin(Blue_GPIO_Port, Blue_Pin, GPIO_PIN_RESET);
				// LD2 LED (blue) on the board is ON!
				LD3ON = true; //Когда Led is ON LD1ON = true!
			} else if (strcmp(pcValue[i], "4") == 0) {
				HAL_GPIO_WritePin(Red_GPIO_Port, Red_Pin, GPIO_PIN_RESET);
				// LD2 LED (blue) on the board is ON!
				LD4ON = true; //Когда Led is ON LD1ON = true!
			} else if (strcmp(pcValue[i], "5") == 0) {
				HAL_GPIO_WritePin(Yellow_GPIO_Port, Yellow_Pin, GPIO_PIN_RESET);
				// LD2 LED (blue) on the board is ON!
				LD5ON = true; //Когда Led is ON LD1ON = true!
			}
		}
	}
	// the extension .shtml for SSI to work
	//return "/index.shtml"; // Было но, Антон переделал!
	return "/redirect.html";

} // END [= CGI #5 =]

// function to initialize CGI [= CGI #6 =]
void myCGIinit(void) {
	//add LED control CGI to the table
	theCGItable[0] = LedCGI;
	//give the table to the HTTP server
	http_set_cgi_handlers(theCGItable, 1);
} // END [= CGI #6 =]

// the actual function for SSI [* SSI #4 *] Прописыаем галочку в боксе!
u16_t mySSIHandler(int iIndex, char *pcInsert, int iInsertLen) {
	if (iIndex == 0) {
		if (LD1ON == false) {
			char myStr1[] = "<input value=\"1\" name=\"led\" type=\"checkbox\">";
			strcpy(pcInsert, myStr1);
			return strlen(myStr1);
		} else if (LD1ON == true) {
			// since the LD3 red LED on the board is ON we make its checkbox checked!
			char myStr1[] =
					"<input value=\"1\" name=\"led\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr1);
			return strlen(myStr1);
		}
	} else if (iIndex == 1) {
		if (LD2ON == false) {
			char myStr2[] = "<input value=\"2\" name=\"led\" type=\"checkbox\">";
			strcpy(pcInsert, myStr2);
			return strlen(myStr2);
		} else if (LD2ON == true) {
			// since the LD2 blue LED on the board is ON we make its checkbox checked!
			char myStr2[] =
					"<input value=\"2\" name=\"led\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr2);
			return strlen(myStr2);
		}
	} else if (iIndex == 2) {
		if (LD3ON == false) {
			char myStr3[] = "<input value=\"3\" name=\"led\" type=\"checkbox\">";
			strcpy(pcInsert, myStr3);
			return strlen(myStr3);
		} else if (LD3ON == true) {
			// since the LD3 red LED on the board is ON we make its checkbox checked!
			char myStr3[] =
					"<input value=\"3\" name=\"led\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr3);
			return strlen(myStr3);
		}
	} else if (iIndex == 3) {
		if (LD4ON == false) {
			char myStr4[] = "<input value=\"4\" name=\"led\" type=\"checkbox\">";
			strcpy(pcInsert, myStr4);
			return strlen(myStr4);
		} else if (LD4ON == true) {
			// since the LD3 red LED on the board is ON we make its checkbox checked!
			char myStr4[] =
					"<input value=\"4\" name=\"led\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr4);
			return strlen(myStr4);
		}
	} else if (iIndex == 4) {
		if (LD5ON == false) {
			char myStr5[] = "<input value=\"5\" name=\"led\" type=\"checkbox\">";
			strcpy(pcInsert, myStr5);
			return strlen(myStr5);
		} else if (LD5ON == true) {
			// since the LD3 red LED on the board is ON we make its checkbox checked!
			char myStr5[] =
					"<input value=\"5\" name=\"led\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr5);
			return strlen(myStr5);
		}
	}
	return 0;
}
// function to initialize SSI [* SSI #5 *]
void mySSIinit(void) {
	http_set_ssi_handler(mySSIHandler, (char const**) theSSItags,
	numSSItags);
}
/*------------------------------------MQTT---------------------------------------------*/
char pacote[1000];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
#include "lwip/apps/httpd.h"
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
  MX_TIM2_Init();
  MX_UART4_Init();
  MX_LWIP_Init();
  /* USER CODE BEGIN 2 */
  httpd_init(); // initializing the HTTPd
  	myCGIinit(); // initializing CGI  [= CGI #7 =]
  	mySSIinit(); // initializing SSI [* SSI #6 *]

  	HAL_TIM_Base_Start_IT(&htim2);
  	/*------------------------------------MQTT---------------------------------------------*/
  	client = mqtt_client_new();

  	if (client != NULL) {
  		sprintf(pacote, "Hi, Zerg! MQTT on STM32 working!"); // Сообщение!
  		example_do_connect(client, "helo_world"); // Топик
  		example_publish(client, pacote); // Публикуем сообщение.
  	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  MX_LWIP_Process(); // starting the LWIP process
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 8400-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
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
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, Green_Pin|Yellow_Pin|White_Pin|Red_Pin
                          |Blue_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Green_Pin Yellow_Pin White_Pin Red_Pin
                           Blue_Pin */
  GPIO_InitStruct.Pin = Green_Pin|Yellow_Pin|White_Pin|Red_Pin
                          |Blue_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
	if (htim->Instance == TIM2) {
		cont++;
		if (cont > 3) { // Once in 3 sec.
			blink++;
			sprintf(pacote, "we blink green led %d times", (int) blink); // Сообщение
			cont = 0;
			HAL_GPIO_TogglePin(GPIOD, Blue_Pin);

			example_do_connect(client, "hello_world"); // Где 'hello_world' - это топик
			example_publish(client, pacote);
			//mqtt_disconnect(client);
		}
	}
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

