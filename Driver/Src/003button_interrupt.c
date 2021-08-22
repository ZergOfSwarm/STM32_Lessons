/*
 * 002led_button.c
 *
 *  Created on: Aug 20, 2021
 *      Author: denis
 */

#include<string.h> // подключил для memset!
#include "stm32f407xx.h"
//#include "stm32f407xx_gpio_driver.h"
#define HIGH 1
#define BTN_PRESSED HIGH


void delay(void)
{	//This will introduce ~200ms delay when systemclock is 16Mhz
	for(uint32_t i=0; i < 500000/2; i++);
}

int main(void)
{
	GPIO_Handel_t GpioLed, GPIOBtn;
	memset(&GpioLed,0,sizeof(GpioLed)); // Установит каждый элимент этой труктуры в ноль!
	memset(&GPIOBtn,0,sizeof(GpioLed)); // Установит каждый элимент этой труктуры в ноль!

/*------------------------------Настройки для LED -------------------------------------
	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_1; // LED на PA-1
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;   // Output режим
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;  // push-pull. Для 001led_toggle.c
	//GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;    // open drain
	//GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD;
	//GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;  // No pull-up no pull-down
*/
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
/*------------------------------Настройки для Button -------------------------------------*/
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;       // Кнопка на PA-0
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;       // Режим Interrupt Falling Adge
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	//GPIOBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;   // push-pull
	//GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	//GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD; // No pull-up no pull-down
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;    // В видео установил GPIO_PIN_PU!

	GPIO_PeriClockControl(GPIOA,ENABLE);
	GPIO_Init(&GPIOBtn);

	// IRQ configuration
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, ENABLE);



	while(1)
	{
		GPIO_ToggleOutputPin(GPIOA,GPIO_PIN_NO_1);
		delay();
	}
	return 0;
}
void EXTI0_IRQHandler(void)  // ISR. Название функции взято из файла startup_stm32f407vgtx.s
{
	delay(); // 200ms wait till button de-bouncing gets ower!
	GPIO_IRQCHandling(GPIO_PIN_NO_0);
	GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_0);
}




















