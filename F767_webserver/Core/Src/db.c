/*
 * db.c
 *
 *  Created on: Dec 14, 2022
 *      Author: anton
 */

#include <db.h>
#include "stdio.h"
#include "stm32f7xx_hal.h"


struct dbPinsConf PinsConf[NUMPIN];

struct dbPinsInfo PinsInfo[NUMPIN] = {
		// Total 89 pins
		{"PA0", "A", 0, GPIO_PIN_0, GPIOA},
		{"PA3", "A", 3, GPIO_PIN_3, GPIOA},
		{"PA4", "A", 4, GPIO_PIN_4, GPIOA},
		{"PA5", "A", 5, GPIO_PIN_5, GPIOA},
		{"PA6", "A", 6, GPIO_PIN_6, GPIOA},
		{"PA15", "A", 15, GPIO_PIN_15, GPIOA},

		{"PB0", "B", 0, GPIO_PIN_0, GPIOB}, // LD1 [Green]
		{"PB1", "B", 1, GPIO_PIN_1, GPIOB},
		{"PB2", "B", 2, GPIO_PIN_2, GPIOB},
		{"PB4", "B", 4, GPIO_PIN_4, GPIOB},
		{"PB5", "B", 5, GPIO_PIN_5, GPIOB},
		{"PB6", "B", 6, GPIO_PIN_6, GPIOB},
		{"PB7", "B", 7, GPIO_PIN_7, GPIOB}, // LD2 [Blue]
		{"PB8", "B", 8, GPIO_PIN_8, GPIOB},
		{"PB9", "B", 9, GPIO_PIN_9, GPIOB},
		{"PB10", "B", 10, GPIO_PIN_10, GPIOB},
		{"PB11", "B", 11, GPIO_PIN_11, GPIOB},
		{"PB12", "B", 12, GPIO_PIN_12, GPIOB},
		{"PB14", "B", 14, GPIO_PIN_14, GPIOB}, // LD3 [Red]
		{"PB15", "B", 15, GPIO_PIN_15, GPIOB},

		{"PC0", "C", 0, GPIO_PIN_0, GPIOC},
		{"PC2", "C", 2, GPIO_PIN_2, GPIOC},
		{"PC3", "C", 3, GPIO_PIN_3, GPIOC},
		{"PC6", "C", 6, GPIO_PIN_6, GPIOC},
		{"PC7", "C", 7, GPIO_PIN_7, GPIOC},
		{"PC8", "C", 8, GPIO_PIN_8, GPIOC},
		{"PC9", "C", 9, GPIO_PIN_9, GPIOC},
		{"PC10", "C", 10, GPIO_PIN_10, GPIOC},
		{"PC11", "C", 11, GPIO_PIN_11, GPIOC},
		{"PC12", "C", 12, GPIO_PIN_12, GPIOC},
		{"PC13", "C", 13, GPIO_PIN_13, GPIOC}, // USER_Btn [B1]

		{"PD0", "D", 0, GPIO_PIN_0, GPIOD},
		{"PD1", "D", 1, GPIO_PIN_1, GPIOD},
		{"PD2", "D", 2, GPIO_PIN_2, GPIOD},
		{"PD3", "D", 3, GPIO_PIN_3, GPIOD},
		{"PD4", "D", 4, GPIO_PIN_4, GPIOD},
		{"PD5", "D", 5, GPIO_PIN_5, GPIOD},
		{"PD6", "D", 6, GPIO_PIN_6, GPIOD},
		{"PD7", "D", 7, GPIO_PIN_7, GPIOD},
		//{"PD8", "D", 8, GPIO_PIN_8, GPIOD}, //STLK_RX [STM32F103CBT6_PA3] (usart3)
		//{"PD9", "D", 9, GPIO_PIN_9, GPIOD}, //STLK_TX [STM32F103CBT6_PA2] (usart3)
		{"PD10", "D", 10, GPIO_PIN_10, GPIOD},
		{"PD11", "D", 11, GPIO_PIN_11, GPIOD},
		{"PD12", "D", 12, GPIO_PIN_12, GPIOD},
		{"PD13", "D", 13, GPIO_PIN_13, GPIOD},
		{"PD14", "D", 14, GPIO_PIN_14, GPIOD},
		{"PD15", "D", 15, GPIO_PIN_15, GPIOD},

		{"PE0", "E", 0, GPIO_PIN_0, GPIOE},
		{"PE1", "E", 1, GPIO_PIN_1, GPIOE},
		{"PE2", "E", 2, GPIO_PIN_2, GPIOE},
		{"PE3", "E", 3, GPIO_PIN_3, GPIOE},
		{"PE4", "E", 4, GPIO_PIN_4, GPIOE},
		{"PE5", "E", 5, GPIO_PIN_5, GPIOE},
		{"PE6", "E", 6, GPIO_PIN_6, GPIOE},
		{"PE7", "E", 7, GPIO_PIN_7, GPIOE},
		{"PE8", "E", 8, GPIO_PIN_8, GPIOE},
		{"PE9", "E", 9, GPIO_PIN_9, GPIOE},
		{"PE10", "E", 10, GPIO_PIN_10, GPIOE},
		{"PE11", "E", 11, GPIO_PIN_11, GPIOE},
		{"PE12", "E", 12, GPIO_PIN_12, GPIOE},
		{"PE13", "E", 13, GPIO_PIN_13, GPIOE},
		{"PE14", "E", 14, GPIO_PIN_14, GPIOE},
		{"PE15", "E", 15, GPIO_PIN_15, GPIOE},

		{"PF0", "F", 0, GPIO_PIN_0, GPIOF},
		{"PF1", "F", 1, GPIO_PIN_1, GPIOF},
		{"PF2", "F", 2, GPIO_PIN_2, GPIOF},
		{"PF3", "F", 3, GPIO_PIN_3, GPIOF},
		{"PF4", "F", 4, GPIO_PIN_4, GPIOF},
		{"PF5", "F", 5, GPIO_PIN_5, GPIOF},
		{"PF6", "F", 6, GPIO_PIN_6, GPIOF},
		{"PF7", "F", 7, GPIO_PIN_7, GPIOF},
		{"PF8", "F", 8, GPIO_PIN_8, GPIOF},
		{"PF9", "F", 9, GPIO_PIN_9, GPIOF},
		{"PF10", "F", 10, GPIO_PIN_10, GPIOF},
		{"PF11", "F", 11, GPIO_PIN_11, GPIOF},
		{"PF12", "F", 12, GPIO_PIN_12, GPIOF},
		{"PF13", "F", 13, GPIO_PIN_13, GPIOF},
		{"PF14", "F", 14, GPIO_PIN_14, GPIOF},
		{"PF15", "F", 15, GPIO_PIN_15, GPIOF},

		{"PG0", "G", 0, GPIO_PIN_0, GPIOG},
		{"PG1", "G", 1, GPIO_PIN_1, GPIOG},
		{"PG2", "G", 2, GPIO_PIN_2, GPIOG},
		{"PG3", "G", 3, GPIO_PIN_3, GPIOG},
		{"PG4", "G", 4, GPIO_PIN_4, GPIOG},
		{"PG5", "G", 5, GPIO_PIN_5, GPIOG},
		{"PG8", "G", 8, GPIO_PIN_8, GPIOG},
		{"PG9", "G", 9, GPIO_PIN_9, GPIOG},
		{"PG10", "G", 10, GPIO_PIN_10, GPIOG},
		{"PG12", "G", 12, GPIO_PIN_12, GPIOG},
		{"PG14", "G", 14, GPIO_PIN_14, GPIOG},
		{"PG15", "G", 15, GPIO_PIN_15, GPIOG}
};
