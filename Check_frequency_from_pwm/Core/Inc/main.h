/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef union		// module send to Lop
{	uint8_t data;
	struct
	{
		uint8_t B0				:1;
//		uint8_t B1				:1;
//		uint8_t B2				:1;
//		uint8_t B3				:1;
	}bit;

}Flag8_t;

typedef enum
{
	MenuButtonStatus_oneClick = 0,
	MenuButtonStatus_doubleClick,
	MenuButtonStatus_heldPressed,
	MenuButtonStatus_notPressed
}MenuButtonStatus_e;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define Acceptance_Level	3000
#define Restart_Level		500

typedef struct
{
	uint32_t 			buttonTimer;
	bool 				buttonTimerEnable;
	MenuButtonStatus_e	buttonStatus;
	Flag8_t 			buttonFlag;
	uint8_t 			buttonHeldPressedCounter;
}MenuButton_t;

extern MenuButton_t hMenuButton;
/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
uint8_t Button_DeBounce		(uint16_t* ADC_Buffer);
//uint8_t	Button1_DeBounce	(uint16_t* ADC_Buffer);
//uint8_t	Button2_DeBounce	(uint16_t* ADC_Buffer);
//uint8_t	Button3_DeBounce	(uint16_t* ADC_Buffer);
uint8_t MenuButton_Debounce	(void);

uint8_t checkTimer			(uint32_t* timer, uint32_t msTime);
void 	setTimer			(uint32_t* timer);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define menu_button_Pin GPIO_PIN_7
#define menu_button_GPIO_Port GPIOB
#define menu_button_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
