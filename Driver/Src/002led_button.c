/*
 * 002led_button.c
 *
 *  Created on: Aug 20, 2021
 *      Author: denis
 */


#include "stm32f407xx.h"
//#include "stm32f407xx_gpio_driver.h"
#define HIGH 1
#define BTN_PRESSED HIGH


void delay(void)
{
	for(uint32_t i=0; i < 500000/3; i++);
}

int main(void)
{
	GPIO_Handel_t GpioLed, GPIOBtn;
/*------------------------------Настройки для LED -------------------------------------*/
	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_1; // LED на PA-1
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;   // Output режим
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	//GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;  // push-pull. Для 001led_toggle.c
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;    // open drain
	//GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;  // No pull-up no pull-down

	GPIO_PeriClockControl(GPIOA,ENABLE);
	GPIO_Init(&GpioLed);
/*------------------------------Настройки для Button -------------------------------------*/
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;      // Кнопка на PA-0
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;         // Input режим
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	//GPIOBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;  // push-pull
	//GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	//GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;  // No pull-up no pull-down
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD;

	GPIO_PeriClockControl(GPIOA,ENABLE);
	GPIO_Init(&GPIOBtn);
	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) == BTN_PRESSED)
		{
			delay();
			GPIO_ToggleOutputPin(GPIOA,GPIO_PIN_NO_1);
		}
	}
	return 0;

}





















