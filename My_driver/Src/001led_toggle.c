/*
 * 001led_toggle.c
 *
 *  Created on: 18 авг. 2021 г.
 *      Author: denis
 */

#include "stm32f407xx.h"
//#include "stm32f407xx_gpio_driver.h"

void delay(void)
{
	for(uint32_t i=0; i < 500000/3; i++);
}

int main(void)
{
	GPIO_Handel_t GpioLed;

	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_1;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	//GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;  //push-pull
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;    //open drain
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	//GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;  // No pull-up no pull-down

	GPIO_PeriClockControl(GPIOA,ENABLE);

	GPIO_Init(&GpioLed);

	while(1)
	{
		GPIO_ToggleOutputPin(GPIOA,GPIO_PIN_NO_1);
		delay();
	}
	return 0;

}










