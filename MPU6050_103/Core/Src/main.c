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
#include "math.h"
#include <stdio.h> /* для printf */

#define MPU6050_ADDR 0xD0
#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

#define COMP_FILTER_ALPHA 0.98  // Коэффициент комплиментарного фильтра (от 0 до 1)

int16_t Accel_X_RAW = 0;
int16_t Accel_Y_RAW = 0;
int16_t Accel_Z_RAW = 0;

int16_t Gyro_X_RAW = 0;
int16_t Gyro_Y_RAW = 0;
int16_t Gyro_Z_RAW = 0;

float Ax, Ay, Az, Gx, Gy, Gz;

typedef struct {
	float aROLL;
	float aPitch;
	float aYaw;
} sensorData;

sensorData MPU6050_Data;

float prevPitch = 0.0;
float prevRoll = 0.0;
float prevYaw = 0.0;
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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
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
void MPU6050_Init (void)
{
	uint8_t check;
	uint8_t Data;

	// check device ID WHO_AM_I

	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR,WHO_AM_I_REG,1, &check, 1, 1000);

	if (check == 104)  // 0x68 will be returned by the sensor if everything goes well
	{
		// power management register 0X6B we should write all 0's to wake the sensor up //  Включение / пробудка модуля.
		Data = 0;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, PWR_MGMT_1_REG, 1,&Data, 1, 1000);

		// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		Data = 0x07;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, 1000);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> � 2g
		Data = 0x00;
		//Data = 0x10; // 2 => +-8g
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000);

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> � 250 �/s
		Data = 0x00;
		//Data = 0x8; // 1 => +-500s т.е. в 3-й бит установим 1
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000);
	}

}

void MPU6050_Read_Accel (void)
{
	uint8_t Rec_Data[6];

	// Read 6 BYTES of data starting from ACCEL_XOUT_H register

	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 6, 1000);

	Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	/*** Для преобразования сырых значений акселерометра в ускорение в "g"
	необходимо разделить их на значение, установленное в регистре FS_SEL (полный диапазон шкалы).
	Мы установили FS_SEL равным 0, поэтому мы делите сырые значения на 16384.0.    ****/

	Ax = Accel_X_RAW/16384.0;// Преобразование Accel_X_RAW, в ускорение в единицах "g".
	Ay = Accel_Y_RAW/16384.0;// Преобразование Accel_Y_RAW, в ускорение в единицах "g".
	Az = Accel_Z_RAW/16384.0;// Преобразование Accel_Z_RAW, в ускорение в единицах "g".
	//printf("Ax = %lf   Ay = %lf   Az = %lf\r\n",Ax,Ay,Az);
}

void MPU6050_Read_Gyro (void)
{
	uint8_t Rec_Data[6];

	// Read 6 BYTES of data starting from GYRO_XOUT_H register
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, 6, 1000);
    if (status != HAL_OK) {
        // Обработка ошибки чтения по I2C
        printf("Error reading gyro data via I2C\r\n");
        return;  // или выполните другие необходимые действия в случае ошибки
    }
	Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	/*** Для преобразования сырых значений гироскопа в скорость вращения в "градусах в секунду"
	необходимо разделить их на значение, установленное в регистре FS_SEL.
	Мы установили FS_SEL равным 0, поэтому мы делите сырые значения на 131.0.    ****/

	Gx = Gyro_X_RAW/131.0; // Преобразование Gyro_X_RAW, в скорость вращения в единицах "градусы в секунду".
	Gy = Gyro_Y_RAW/131.0; // Преобразование Gyro_Y_RAW, в скорость вращения в единицах "градусы в секунду".
	Gz = Gyro_Z_RAW/131.0; // Преобразование Gyro_Z_RAW, в скорость вращения в единицах "градусы в секунду".

    // Вычисление угла Roll (Крен) (произведения наклона)
	//MPU6050_Data.aROLL = atan2(Ay, sqrt(Ax * Ax + Az * Az)) / 180.0 * M_PI;
	MPU6050_Data.aROLL = atan2(Ay, Az) * 180.0 / M_PI; // Крен (Roll)

//	float ROLL = mpu6050data[4] - fGY_Cal; // относительно "нуля"
//	ROLL = ROLL / 65.5 / 50; // TODO Укажи правильное количество измерений вместо 50!

	// Вычисление угла PITCH (Тангаж):
	MPU6050_Data.aPitch = atan2(-Ax, sqrt(Ay * Ay + Az * Az)) / 180.0 * M_PI;// Если отбросить деление (180.0 * M_PI) то, результат будет в радианах а не в градусах!
	//printf("Roll = %lf   Pitch = %lf\r\n",MPU6050_Data.aROLL,MPU6050_Data.aPitch);

	//float Yaw = (Gz - sumGz) / 131.0 / 50; // TODO Укажи правильное количество измерений вместо 50!
	float Yaw = (Gz) / 131.0;
	MPU6050_Data.aYaw = Yaw; // РЫСКАНЬЕ
	// Если есть перемещения по крену и тангажу и после мы делаем рыскание то изменяется и крен и тонгаж!
	// Но, датчик будет показывать только рысканье а крен и тангаж будет иметь прежние значения!
	// Нам нужно каждый раз пересчитывать и крен и тангаж относительно рысканья!
	if (Yaw > 0.01) { //Обработка рыскания: TODO сравнение с дельтой
		float _Y = sin(Yaw * 3.1415 / 180);
		MPU6050_Data.aPitch += MPU6050_Data.aROLL * _Y;
		MPU6050_Data.aROLL -= MPU6050_Data.aPitch * _Y;
	}
	//printf("ROLL:%f  Pithch:%f  Yaw:%f\r\n", MPU6050_Data.aROLL, MPU6050_Data.aPitch, MPU6050_Data.aYaw);
}

void ComplementaryFilter(float accelPitch, float gyroPitch, float accelRoll, float gyroRoll, float accelYaw, float gyroYaw) {
    // ... (ваш код без изменений)

    // Применение комплиментарного фильтра для сглаживания измерений акселерометра и гироскопа
    MPU6050_Data.aPitch = COMP_FILTER_ALPHA * (gyroPitch + accelPitch) + (1 - COMP_FILTER_ALPHA) * prevPitch;
    MPU6050_Data.aROLL = COMP_FILTER_ALPHA * (gyroRoll + accelRoll) + (1 - COMP_FILTER_ALPHA) * prevRoll;

    // Применение комплиментарного фильтра для угла Yaw
    MPU6050_Data.aYaw = COMP_FILTER_ALPHA * (gyroYaw + accelYaw) + (1 - COMP_FILTER_ALPHA) * prevYaw;

    printf("f_Pitch: %f, f_ROLL: %f, f_YAW: %f\r\n", MPU6050_Data.aPitch, MPU6050_Data.aROLL, MPU6050_Data.aYaw);
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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  MPU6050_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  MPU6050_Read_Accel();
	  MPU6050_Read_Gyro();
	  // Вычисление углов Pitch, Roll и Yaw из акселерометра
	  MPU6050_Data.aPitch = atan2(-Ax, sqrt(Ay * Ay + Az * Az)) * 180.0 / M_PI;
	  MPU6050_Data.aROLL = atan2(Ay, Az) * 180.0 / M_PI;
	  MPU6050_Data.aYaw = 0;  // Здесь нужно использовать данные магнитометра для полной оценки угла Yaw

	  ComplementaryFilter(MPU6050_Data.aPitch, Gx, MPU6050_Data.aROLL, Gy, MPU6050_Data.aYaw, Gz);

	  prevPitch = MPU6050_Data.aPitch;
	  prevRoll = MPU6050_Data.aROLL;
	  prevYaw = MPU6050_Data.aYaw;
	  HAL_Delay(20);
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
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
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
