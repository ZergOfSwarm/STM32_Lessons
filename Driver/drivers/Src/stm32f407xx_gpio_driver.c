/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: Aug 17, 2021
 *      Author: denis
 */


#include "stm32f407xx_gpio_driver.h" // Этот файл должени иметь эту запись!

/***************************************************************************
 * @Function name               GPIO_PeriClockControl
 * @brief description           This function enables or disables clock for the given GPIO port
 * @param[in]                   - base address of the gpio peripheral
 * @param[in]                   - ENABLE or DISABLE macros
 * @param[in]
 *
 * @return                      none
 *
 * @Note                        none
 *
 */

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi) // При помощи этой функции мы вкл/откл перефирию
{
	if (EnorDi == ENABLE) {
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_EN();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_EN();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_EN();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_EN();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_EN();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_EN();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLK_EN();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLK_EN();
		} else if (pGPIOx == GPIOI) {
			GPIOI_PCLK_EN();
		} else {
			if (pGPIOx == GPIOA) {
				GPIOA_PCLK_DI();
			} else if (pGPIOx == GPIOB) {
				GPIOB_PCLK_DI();
			} else if (pGPIOx == GPIOC) {
				GPIOC_PCLK_DI();
			} else if (pGPIOx == GPIOD) {
				GPIOD_PCLK_DI();
			} else if (pGPIOx == GPIOE) {
				GPIOE_PCLK_DI();
			} else if (pGPIOx == GPIOF) {
				GPIOF_PCLK_DI();
			} else if (pGPIOx == GPIOG) {
				GPIOG_PCLK_DI();
			} else if (pGPIOx == GPIOH) {
				GPIOH_PCLK_DI();
			} else if (pGPIOx == GPIOI) {
				GPIOI_PCLK_DI();
			}
		}
	}
}
/***************************************************************************
 * @Function name
 * @brief description
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 *
 * @Note
 */
void GPIO_Init(GPIO_Handel_t *pGPIOHandle) // This is API for initialisation GPIO
{
	uint32_t temp=0;  //Временная переменная
	// 1. configure the mode of gpio pin
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		temp=(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 *pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Clearing пина/ов перед установкой значения
		pGPIOHandle->pGPIOx->MODER |= temp; // Setting значение temp в регистр! Где pGPIOHandle->pGPIOx - это Base address
	}else
	{

	}
	temp = 0; // И снова инициализируем переменную.
	// 2. configure the speed
		temp=(pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed<<(2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Clearing пина/ов перед установкой значения
		pGPIOHandle->pGPIOx->OSPEEDR |= temp;
		temp = 0; // И снова инициализируем переменную.

	//3. configure the pupd settings
		temp=(pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl<<(2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Clearing пина/ов перед установкой значения
		pGPIOHandle->pGPIOx->PUPDR |= temp;
		temp = 0; // И снова инициализируем переменную.

	//4. configure the the optype
		temp=(pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Clearing пина/ов перед установкой значения
		pGPIOHandle->pGPIOx->OTYPER |= temp;
		temp = 0; // И снова инициализируем переменную.
	//5. configure the alt functionality
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
		{
			uint8_t temp1, temp2;

			temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
			temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
			pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4*temp2));  // Clearing пина/ов перед установкой значения
			pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4*temp2));
		}
	}
/***************************************************************************
 * @Function name
 * @brief description
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 *
 * @Note
 *
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx) // Эта фанкция возвращает резистры в начальное состояние!RM -> RCC AHB1 peripheral reset register (RCC_AHB1RSTR)
{
	if (pGPIOx == GPIOA) {
		GPIOA_REG_RESET();
	} else if (pGPIOx == GPIOB) {
		GPIOB_REG_RESET();
	} else if (pGPIOx == GPIOC) {
		GPIOC_REG_RESET();
	} else if (pGPIOx == GPIOD) {
		GPIOD_REG_RESET();
	} else if (pGPIOx == GPIOE) {
		GPIOE_REG_RESET();
	} else if (pGPIOx == GPIOF) {
		GPIOF_REG_RESET();
	} else if (pGPIOx == GPIOG) {
		GPIOG_REG_RESET();
	} else if (pGPIOx == GPIOH) {
		GPIOH_REG_RESET();
	} else if (pGPIOx == GPIOI) {
		GPIOI_REG_RESET();
	}
}
/***************************************************************************
 * @Function name             GPIO_ReadFromInputPin
 * @brief description         Читаем нужный пин из таблици // RM -> GPIO port input data register (GPIOx_IDR) (x = A..I/J/K)
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 *
 * @Note           где 0x00000001 - это маска которая обнуляет все кроме -1го бита и его мы сохраняем в переменную value!
 *
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001); //т.е. нужный бит мы сдвигаем на нулевое значение после его читаем/сохраняем!
	return value;
	}
/***************************************************************************
 * @Function name
 * @brief description
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 *
 * @Note
 *
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
		value = (uint16_t)pGPIOx->IDR;
		return value;
	}
/***************************************************************************
 * @Function name
 * @brief description
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 *
 * @Note
 *
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		//write 1 to the output data register at the field corresponding to the pin number
		pGPIOx->ODR |=(1 << PinNumber);
	}else
	{
		//write 0
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}
/***************************************************************************
 * @Function name
 * @brief description
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 *
 * @Note
 *
 */
void GPIO_PIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
	}
/***************************************************************************
 * @Function name            GPIO_ToggleOutputPin
 * @brief description        Toggle
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 *
 * @Note                     При каждом вызове этой функции будет переключать пин!
 *
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR = pGPIOx->ODR ^(1 << PinNumber); // или pGPIOx->ODR ^= (1 << PinNumber);
	}
/***************************************************************************
 * @Function name
 * @brief description
 * @param[in]
 * @param[in]
 * @param[in]
 *
 *  @return
 *
 * @Note
 *
 */
//IRQ Configuration and ISR handling
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDI)
{
	}
/***************************************************************************
 * @Function name
 * @brief description
 * @param[in]
 * @param[in]
 * @param[in]
 *
 * @return
 *
 * @Note
 *
 */
void GPIO_IRQCHandling(uint8_t PinNumber)
{
	}









