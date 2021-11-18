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
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lwip/apps/httpd.h" // including httpd.h [- HTTPd #1 -]
#include "string.h"  // we include String.h for the strcmp() function [= CGI #1 =]
#include "stdbool.h" // we include this library to be able to use boolean variables for SSI

bool LD1ON = false; // this variable will indicate if the LD1 LED on the board is ON or not
bool LD2ON = false; // this variable will indicate if our LD2 LED on the board is ON or not
bool LD3ON = false; // this variable will indicate if the LD3 LED on the board is ON or not
bool LD4ON = false; // this variable will indicate if the LD4 LED on the board is ON or not
bool LD5ON = false; // this variable will indicate if the LD5 LED on the board is ON or not

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
		HAL_GPIO_WritePin(White_GPIO_Port, White_Pin, GPIO_PIN_RESET);
		// we put this variable to false to indicate that the LD1 LED on the board is not ON
		LD1ON = false; //Когда Led is off LD1ON = false!
		HAL_GPIO_WritePin(Green_GPIO_Port, Green_Pin, GPIO_PIN_RESET);
		// we put this variable to false to indicate that the LD2 LED on the board is not ON
		LD2ON = false; //Когда Led is off LD1ON = false!
		HAL_GPIO_WritePin(Blue_GPIO_Port, Blue_Pin, GPIO_PIN_RESET);
		// we put this variable to false to indicate that the LD3 LED on the board is not ON
		LD3ON = false; //Когда Led is off LD1ON = false!
		HAL_GPIO_WritePin(Red_GPIO_Port, Red_Pin, GPIO_PIN_RESET);
		// we put this variable to false to indicate that the LD2 LED on the board is not ON
		LD4ON = false; //Когда Led is off LD1ON = false!
		HAL_GPIO_WritePin(Yellow_GPIO_Port, Yellow_Pin, GPIO_PIN_RESET);
		// we put this variable to false to indicate that the LD2 LED on the board is not ON
		LD5ON = false; //Когда Led is off LD1ON = false!
	}
	for (i = 0; i < iNumParams; i++) {
		if (strcmp(pcParam[i], "led") == 0) {
			if (strcmp(pcValue[i], "1") == 0) {
				HAL_GPIO_WritePin(White_GPIO_Port, White_Pin, GPIO_PIN_SET);
				// LD3 LED (red) on the board is ON!
				LD1ON = true; //Когда Led is ON LD1ON = true!
			} else if (strcmp(pcValue[i], "2") == 0) {
				HAL_GPIO_WritePin(Green_GPIO_Port, Green_Pin, GPIO_PIN_SET);
				// LD2 LED (blue) on the board is ON!
				LD2ON = true; //Когда Led is ON LD1ON = true!
			} else if (strcmp(pcValue[i], "3") == 0) {
				HAL_GPIO_WritePin(Blue_GPIO_Port, Blue_Pin, GPIO_PIN_SET);
				// LD2 LED (blue) on the board is ON!
				LD3ON = true; //Когда Led is ON LD1ON = true!
			} else if (strcmp(pcValue[i], "4") == 0) {
				HAL_GPIO_WritePin(Red_GPIO_Port, Red_Pin, GPIO_PIN_SET);
				// LD2 LED (blue) on the board is ON!
				LD4ON = true; //Когда Led is ON LD1ON = true!
			} else if (strcmp(pcValue[i], "5") == 0) {
				HAL_GPIO_WritePin(Yellow_GPIO_Port, Yellow_Pin, GPIO_PIN_SET);
				// LD2 LED (blue) on the board is ON!
				LD5ON = true; //Когда Led is ON LD1ON = true!
			}
		}
	}
	// the extension .shtml for SSI to work
	return "/index.shtml";

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
			char myStr1[] ="<input value=\"1\" name=\"led\" type=\"checkbox\" checked>";
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
	}
	else if (iIndex == 2) {
		if (LD3ON == false) {
			char myStr3[] = "<input value=\"3\" name=\"led\" type=\"checkbox\">";
			strcpy(pcInsert, myStr3);
			return strlen(myStr3);
		} else if (LD3ON == true) {
			// since the LD3 red LED on the board is ON we make its checkbox checked!
			char myStr3[] ="<input value=\"3\" name=\"led\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr3);
			return strlen(myStr3);
		}
	}
	else if (iIndex == 3) {
		if (LD4ON == false) {
			char myStr4[] = "<input value=\"4\" name=\"led\" type=\"checkbox\">";
			strcpy(pcInsert, myStr4);
			return strlen(myStr4);
		} else if (LD4ON == true) {
			// since the LD3 red LED on the board is ON we make its checkbox checked!
			char myStr4[] ="<input value=\"4\" name=\"led\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr4);
			return strlen(myStr4);
		}
	}
	else if (iIndex == 4) {
		if (LD5ON == false) {
			char myStr5[] = "<input value=\"5\" name=\"led\" type=\"checkbox\">";
			strcpy(pcInsert, myStr5);
			return strlen(myStr5);
		} else if (LD5ON == true) {
			// since the LD3 red LED on the board is ON we make its checkbox checked!
			char myStr5[] ="<input value=\"5\" name=\"led\" type=\"checkbox\" checked>";
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
  MX_LWIP_Init();
  /* USER CODE BEGIN 2 */
     httpd_init(); // initializing the HTTPd

     myCGIinit(); // initializing CGI  [= CGI #7 =]

     mySSIinit(); // initializing SSI [* SSI #6 *]
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      MX_LWIP_Process(); // starting the LWIP process
/*
		HAL_GPIO_WritePin(White_GPIO_Port, White_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(Green_GPIO_Port, Green_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(Blue_GPIO_Port, Blue_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(Red_GPIO_Port, Red_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(Yellow_GPIO_Port, Yellow_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
*/
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
  HAL_GPIO_WritePin(GPIOD, White_Pin|Green_Pin|Blue_Pin|Red_Pin
                          |Yellow_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : White_Pin Green_Pin Blue_Pin Red_Pin
                           Yellow_Pin */
  GPIO_InitStruct.Pin = White_Pin|Green_Pin|Blue_Pin|Red_Pin
                          |Yellow_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
