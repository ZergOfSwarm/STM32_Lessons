/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: Aug 17, 2021
 *      Author: denis
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h" // Этот файл должени иметь эту запись!



// This is a Configuration structure for a GPIO pin
typedef struct
{
	uint8_t GPIO_PinNumber;      // uint8_t т.к. тут будет храниться данные от 0-15 т.е. номера пинов.
	uint8_t GPIO_PinMode;        // Возможные значения из @GPIO_PIN_MODES
	uint8_t GPIO_PinSpeed;       // Возможные значения из @GPIO_PIN_SPEED
	uint8_t GPIO_PinPuPdControl; //
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;


// This is a Handle structure for a GPIO pin

typedef struct
{
	GPIO_RegDef_t *pGPIOx; // This holds the base address of the GPIO port to wich the pin belongs.
	GPIO_PinConfig_t GPIO_PinConfig; // This holds GPIO pin configuration settings
}GPIO_Handel_t;

// Пишем макросы для всех возможных вариантов pin numbers RM ->
#define GPIO_PIN_NO_0        0
#define GPIO_PIN_NO_1        1
#define GPIO_PIN_NO_2        2
#define GPIO_PIN_NO_3        3
#define GPIO_PIN_NO_4        4
#define GPIO_PIN_NO_5        5
#define GPIO_PIN_NO_6        6
#define GPIO_PIN_NO_7        7
#define GPIO_PIN_NO_8        8
#define GPIO_PIN_NO_9        9
#define GPIO_PIN_NO_10       10
#define GPIO_PIN_NO_11       11
#define GPIO_PIN_NO_12       12
#define GPIO_PIN_NO_13       13
#define GPIO_PIN_NO_14       14
#define GPIO_PIN_NO_15       15

// Пишем макросы для всех возможных вариантов modes пинов RM -> GPIO port mode register (GPIOx_MODER) (x = A..I/J/K)
// @GPIO_PIN_MODES
#define GPIO_MODE_IN       0
#define GPIO_MODE_OUT      1
#define GPIO_MODE_ALTFN    2
#define GPIO_MODE_ANALOG   3
#define GPIO_MODE_IT_FT    4 // input falling age
#define GPIO_MODE_IT_RT    5 // input rising age
#define GPIO_MODE_IT_RFT   6 // input rising and falling age triger

// Пишем макросы для всех возможных вариантов output types RM -> GPIO port output type register (GPIOx_OTYPER)
#define GPIO_OP_TYPE_PP    0 // Push-pull
#define GPIO_OP_TYPE_OD    1 // Open-drain

// Пишем макросы для всех возможных вариантов output speeds RM -> GPIO port output speed register (GPIOx_OSPEEDR)(x = A..I/J/K)
#define GPIO_SPEED_LOW     0
#define GPIO_SPEED_MEDIUM  1
#define GPIO_SPEED_FAST    2
#define GPIO_SPEED_HIGH    3

// Пишем макросы для всех возможных вариантов output pull-up/pull-down register RM -> GPIO port pull-up/pull-down register (GPIOx_PUPDR)
#define GPIO_NO_PUPD       0
#define GPIO_PIN_PU        1
#define GPIO_PIN_PD        2

// протоколы API's

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi); // При помощи этой функции мы вкл/откл перефирию
// Init and de init
void GPIO_Init(GPIO_Handel_t *pGPIOHandle); // This is API for initialisation GPIO
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx); // Эта фанкция возвращает резистры в начальное состояние!RM -> RCC AHB1 peripheral reset register (RCC_AHB1RSTR)
// Data read and write
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_PIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
//IRQ Configuration and ISR handling
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDI);
void GPIO_IRQCHandling(uint8_t PinNumber);








#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */









