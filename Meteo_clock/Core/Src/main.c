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
#include <stdlib.h>  /* For BME280 */
#include <string.h>  /* For BME280 */
#include "bme280.h"  /* For BME280 */
#include <stdio.h>   /* For printf */
#include "i2c-lcd.h" /* For LCD */
/*-------------------------------для MH-Z19B------------------------------------*/
#define rx_buffer_size 64
uint8_t rx_buffer[rx_buffer_size] = { 0, };
uint16_t rx_buffer_len; // Переменная для длинны данных.
uint16_t CO2_concentration;
uint8_t tx_buffer[9] = { 0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 };
unsigned long T;
uint8_t crc_check;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = { .name = "defaultTask",
		.priority = (osPriority_t) osPriorityNormal, .stack_size = 128 * 4 };
/* Definitions for my_BME280_Task */
osThreadId_t my_BME280_TaskHandle;
uint32_t my_BME280_TaskBuffer[200];
osStaticThreadDef_t my_BME280_TaskControlBlock;
const osThreadAttr_t my_BME280_Task_attributes = { .name = "my_BME280_Task",
		.stack_mem = &my_BME280_TaskBuffer[0], .stack_size =
				sizeof(my_BME280_TaskBuffer), .cb_mem =
				&my_BME280_TaskControlBlock, .cb_size =
				sizeof(my_BME280_TaskControlBlock), .priority =
				(osPriority_t) osPriorityLow, };
/* Definitions for my_MHZ19B_Task */
osThreadId_t my_MHZ19B_TaskHandle;
uint32_t my_MHZ19B_TaskBuffer[300];
osStaticThreadDef_t my_MHZ19B_TaskControlBlock;
const osThreadAttr_t my_MHZ19B_Task_attributes = { .name = "my_MHZ19B_Task",
		.stack_mem = &my_MHZ19B_TaskBuffer[0], .stack_size =
				sizeof(my_MHZ19B_TaskBuffer), .cb_mem =
				&my_MHZ19B_TaskControlBlock, .cb_size =
				sizeof(my_MHZ19B_TaskControlBlock), .priority =
				(osPriority_t) osPriorityLow, };
/* Definitions for myQueue */
osMessageQueueId_t myQueueHandle;
uint8_t myQueueBuffer[5 * sizeof(uint16_t)];
osStaticMessageQDef_t myQueueControlBlock;
const osMessageQueueAttr_t myQueue_attributes = { .name = "myQueue", .cb_mem =
		&myQueueControlBlock, .cb_size = sizeof(myQueueControlBlock), .mq_mem =
		&myQueueBuffer, .mq_size = sizeof(myQueueBuffer) };
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void *argument);
void Start_BME280_Task(void *argument);
void Start_MHZ19B_Task(void *argument);

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
/*-----------------------------For BME280--------------------------------------*/

float temperature;
float humidity;
float pressure;

struct bme280_dev dev;
struct bme280_data comp_data;
int8_t rslt;

char line1[16];
char line2[16];

int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
	if (HAL_I2C_Master_Transmit(&hi2c1, (id << 1), &reg_addr, 1, 10) != HAL_OK)
		return -1;
	if (HAL_I2C_Master_Receive(&hi2c1, (id << 1) | 0x01, data, len, 10)
			!= HAL_OK)
		return -1;

	return 0;
}

void user_delay_ms(uint32_t period) {
	HAL_Delay(period);
}

int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
	int8_t *buf;
	buf = malloc(len + 1);
	buf[0] = reg_addr;
	memcpy(buf + 1, data, len);

	if (HAL_I2C_Master_Transmit(&hi2c1, (id << 1), (uint8_t*) buf, len + 1,
	HAL_MAX_DELAY) != HAL_OK)
		return -1;

	free(buf);
	return 0;
}
/*-----------------------------для MH-Z19Bf--------------------------------------*/

/*------------------------IDLE_Callback---------------------------------------*/
void HAL_UART_IDLE_Callback(UART_HandleTypeDef *huart) {
	char str_co2[20] = {0};// for LCD
	if (huart == &huart1) { //проверка, из нужного ли uart пришли данные
		//printf("IDLE = OK\n\r");
		__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
		rx_buffer_len = rx_buffer_size - huart->RxXferCount;

		/*----------основные действия с входящими данными-----------*/
/*		printf("rx_buffer = %X", rx_buffer[0]);
		printf(" %X", rx_buffer[1]);
		printf(" %X", rx_buffer[2]);
		printf(" %X", rx_buffer[3]);
		printf(" %X", rx_buffer[4]);
		printf(" %X", rx_buffer[5]);
		printf(" %X", rx_buffer[6]);
		printf(" %X", rx_buffer[7]);
		printf(" %X\r\n", rx_buffer[8]);
*/
		crc_check = (~(rx_buffer[1] + rx_buffer[2] + rx_buffer[3] + rx_buffer[4]
				+ rx_buffer[5] + rx_buffer[6] + rx_buffer[7]) + 1);
		if (crc_check == rx_buffer[8]) {
			printf("CRC = OK\r\n");
			CO2_concentration = rx_buffer[2] * 256 + rx_buffer[3];
			printf("CO2 = %d\r\n", CO2_concentration);
			sprintf(str_co2, "CO2:%d", CO2_concentration);
			lcd_put_cur(0, 8); // To LCD
			lcd_send_string(str_co2); // To LCD
			//lcd_send_string("mm.Hg");
		} else {
			printf("CRC != OK\r\n");
		}

		/*----------основные действия с входящими данными-----------*/

		HAL_UART_AbortReceive_IT(&huart1);
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
		HAL_UART_Receive_IT(&huart1, rx_buffer, rx_buffer_size);
	}
}
/*------------------------IDLE_Callback---------------------------------------*/

/*------------------------RxCpltCallback---------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart1) { //проверка, из нужного ли uart пришли данные
		printf("RxCpltCallback = OK\n\r");
		__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);

		printf("rx_buffer is overflow\n\r");

		HAL_UART_AbortReceive_IT(&huart1);
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
		HAL_UART_Receive_IT(&huart1, rx_buffer, rx_buffer_size);

	}
}
/*------------------------RxCpltCallback---------------------------------------*/
/*------------------------End for MH-Z19Bf-------------------------------------*/
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
	MX_I2C1_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	/* BME280 */
	dev.dev_id = BME280_I2C_ADDR_PRIM;
	dev.intf = BME280_I2C_INTF;
	dev.read = user_i2c_read;
	dev.write = user_i2c_write;
	dev.delay_ms = user_delay_ms;

	rslt = bme280_init(&dev);

	/* BME280 */
	dev.settings.osr_h = BME280_OVERSAMPLING_1X;
	dev.settings.osr_p = BME280_OVERSAMPLING_16X;
	dev.settings.osr_t = BME280_OVERSAMPLING_2X;
	dev.settings.filter = BME280_FILTER_COEFF_16;
	rslt = bme280_set_sensor_settings(
			BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL
					| BME280_FILTER_SEL, &dev);

	/*-----------------------------For LCD-------------------------------------*/
	lcd_init();
	lcd_clear(); // Очищаем LCD
	lcd_put_cur(0, 0); // Устанавливаем курсор в нужное место
	lcd_send_string("Hello World!");
	HAL_Delay(5000);
	lcd_clear(); // Очищаем LCD

	lcd_put_cur(1, 0); // Устанавливаем курсор в нужное место
	lcd_send_string("Cool, It works!");
	/*------------------------For MH-Z19Bf---------------------------------------*/
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UART_Receive_IT(&huart1, rx_buffer, rx_buffer_size); // Прием данных
	T = HAL_GetTick();
	/*------------------------End for MH-Z19Bf---------------------------------------*/
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

	/* Create the queue(s) */
	/* creation of myQueue */
	myQueueHandle = osMessageQueueNew(5, sizeof(uint16_t), &myQueue_attributes);

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL,
			&defaultTask_attributes);

	/* creation of my_BME280_Task */
	my_BME280_TaskHandle = osThreadNew(Start_BME280_Task, NULL,
			&my_BME280_Task_attributes);

	/* creation of my_MHZ19B_Task */
	my_MHZ19B_TaskHandle = osThreadNew(Start_MHZ19B_Task, NULL,
			&my_MHZ19B_Task_attributes);

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
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

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
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
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
static void MX_GPIO_Init(void) {

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument) {
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Start_BME280_Task */
/**
 * @brief Function implementing the my_BME280_Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_BME280_Task */
void Start_BME280_Task(void *argument) {
	/* USER CODE BEGIN Start_BME280_Task */
	/* Infinite loop */
	char str_t[20] = { 0 };
	char str_h[20] = { 0 };
	char str_p[20] = { 0 };
	for (;;) {
		rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &dev);
		dev.delay_ms(40);
		/* BME280 getrslt */
		rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);
		if (rslt == BME280_OK) {
			temperature = comp_data.temperature / 100.0; /* °C  */
			humidity = comp_data.humidity / 1024.0; /* %   */
			pressure = comp_data.pressure / 10000.0; /* hPa */

			//printf("TEMP: %03.1f \n", temperature);
			//printf("HUMID: %03.1f \n", humidity);
			//printf("PRESSURE: %03.1f \n", pressure);

			sprintf(str_t, "T-%.1f", temperature);
			lcd_put_cur(0, 0);
			lcd_send_string(str_t);
			lcd_send_string("C");

			sprintf(str_h, "H-%.1f", humidity);
			lcd_put_cur(1, 9);
			lcd_send_string(str_h);
			lcd_send_string("%");

			sprintf(str_p, "P:%.1f", pressure);
			lcd_put_cur(1, 0);
			lcd_send_string(str_p);
			//lcd_send_string("mm.Hg");

		}
	}
	/* USER CODE END Start_BME280_Task */
}

/* USER CODE BEGIN Header_Start_MHZ19B_Task */
/**
 * @brief Function implementing the my_MHZ19B_Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_MHZ19B_Task */
void Start_MHZ19B_Task(void *argument) {
	/* USER CODE BEGIN Start_MHZ19B_Task */
	/* Infinite loop */
	for (;;) {
		if (HAL_GetTick() - T >= 1000) {
			T = HAL_GetTick();
			HAL_UART_Transmit_IT(&huart1, tx_buffer, 9);
		}
	}
	osDelay(1000);
	/* USER CODE END Start_MHZ19B_Task */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM4 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
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
