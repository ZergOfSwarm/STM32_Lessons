/*
 * stm32f407xx.h
 *
 *  Created on: Aug 17, 2021
 *      Author: denis
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include<stdint.h>  // Без него будет ругаться на uint32_t и т.п.
#define __vo volatile

/******************************START:Processor Specific Details**************************/
/*
 * ARM Cortex Mx Processor NVIC ISERx register Addresses (Interrupt Set-enable Registers)
 */
#define NVIC_ISER0         ((__vo uint32_t*)0xE000E100)       // Cortex -M4 Devices TM Generic User Guide -> Table 4-2 NVIC register summary
#define NVIC_ISER1         ((__vo uint32_t*)0xE000E104)
#define NVIC_ISER2         ((__vo uint32_t*)0xE000E108)
#define NVIC_ISER3         ((__vo uint32_t*)0xE000E10c)
//#define NVIC_ISER4         ((__vo uint32_t)xxxxxxxxx)
//#define NVIC_ISER5         ((__vo uint32_t)xxxxxxxxx)
//#define NVIC_ISER6         ((__vo uint32_t)xxxxxxxxx)
/*
 * ARM Cortex Mx Processor NVIC ICERx register Addresses (Interrupt Clear-enable Registers)
 */
#define NVIC_ICER0         ((__vo uint32_t*)0XE000E180)       // Cortex -M4 Devices TM Generic User Guide -> Table 4-2 NVIC register summary
#define NVIC_ICER1         ((__vo uint32_t*)0XE000E184)
#define NVIC_ICER2         ((__vo uint32_t*)0XE000E188)
#define NVIC_ICER3         ((__vo uint32_t*)0XE000E18C)

/*
 * ARM Cortex Mx Processor NVIC Interrupt Priority Registers Address Calculation
 */
#define NVIC_PR_BASE_ADDR ((__vo uint32_t*)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED  4

// 1. Defining base address of Flash, Sram's, ROM, etc.
#define FLASH_BASEADDR 0x08000000U           // Main memory. RM Table 5. -> Table 3. Memory mapping vs. Boot mode/physical remap in STM32F405xx/07xx and STM32F415xx/17xx
#define SRAM1_BASEADDR 0x20000000U           // 112 KB RM -> Table 3. Memory mapping vs. Boot mode/physical remap in STM32F405xx/07xx and STM32F415xx/17xx
#define SRAM2_BASEADDR 0x2001C000U           // 16 KB RM -> Memory and bus architecture. Как получил Base addres SRAM2 см в cherrytree Lesson 8!
#define ROM_BASEADDR   0x1FFF0000U           // System memory. RM-> Table 3. Memory mapping vs. Boot mode/physical remap in STM32F405xx/07xx and STM32F415xx/17xx
#define SRAM_BASEADDR  SRAM1_BASEADDR
//2. Defining base address of varius bas domains such as (AHBx domain, APBx domain)
#define PERIPH_BASEADDR     0x40000000U           // RM -> Table 1. STM32F4xx register boundary addresses (continued)
#define APB1PERIPH_BASEADDR PERIPH_BASEADDR       // RM -> Table 1. STM32F4xx register boundary addresses (continued)
#define APB2PERIPH_BASEADDR 0x40010000U           // RM -> Table 1. STM32F4xx register boundary addresses (continued)
#define AHB1PERIPH_BASEADDR 0x40020000U           // RM -> Table 1. STM32F4xx register boundary addresses (continued)
#define AHB2PERIPH_BASEADDR 0x50000000U           // RM -> Table 1. STM32F4xx register boundary addresses (continued)
//3.Defining base address of peripherals which are hanging on AHB1 bus
#define GPIOA_BASEADDR (AHB1PERIPH_BASEADDR + 0x0000)
#define GPIOB_BASEADDR (AHB1PERIPH_BASEADDR + 0x0400)
#define GPIOC_BASEADDR (AHB1PERIPH_BASEADDR + 0x0800)
#define GPIOD_BASEADDR (AHB1PERIPH_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR (AHB1PERIPH_BASEADDR + 0x1000)
#define GPIOF_BASEADDR (AHB1PERIPH_BASEADDR + 0x1400)
#define GPIOG_BASEADDR (AHB1PERIPH_BASEADDR + 0x1800)
#define GPIOH_BASEADDR (AHB1PERIPH_BASEADDR + 0x1C00)
#define GPIOI_BASEADDR (AHB1PERIPH_BASEADDR + 0x2000)
#define GPIOJ_BASEADDR (AHB1PERIPH_BASEADDR + 0x2400)
#define GPIOK_BASEADDR (AHB1PERIPH_BASEADDR + 0x2800)

#define RCC_BASEADDR (AHB1PERIPH_BASEADDR + 0x3800)
//4.Defining base address of peripherals which are hanging on APB1 bus
#define I2C1_BASEADDR   (APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR   (APB1PERIPH_BASEADDR + 0x5800)
#define I2C3_BASEADDR   (APB1PERIPH_BASEADDR + 0x5C00)

#define SPI2_BASEADDR   (APB1PERIPH_BASEADDR + 0x3800)
#define SPI3_BASEADDR   (APB1PERIPH_BASEADDR + 0x3C00)

#define USART2_BASEADDR (APB1PERIPH_BASEADDR + 0x4400)
#define USART3_BASEADDR (APB1PERIPH_BASEADDR + 0x4800)
#define UART4_BASEADDR (APB1PERIPH_BASEADDR + 0x4C00)
#define UART5_BASEADDR (APB1PERIPH_BASEADDR + 0x5000)
//5.Defining base address of peripherals which are hanging on APB2 bus
#define SPI1_BASEADDR   (APB2PERIPH_BASEADDR + 0x3000)
#define USART1_BASEADDR (APB2PERIPH_BASEADDR + 0x1000)
#define USART6_BASEADDR (APB2PERIPH_BASEADDR + 0x1400)
#define EXTI_BASEADDR   (APB2PERIPH_BASEADDR + 0x3C00) // RM -> Table 1. STM32F4xx register boundary addresses (continued)
#define SYSCFG_BASEADDR (APB2PERIPH_BASEADDR + 0x3800)


/*------------------PERIPHERAL REGISTER DEFINITION STRUCTURS-------------------------------------------------*/
// RM -> Table 39. GPIO register map and reset values / Структура для определения переферийного регистра для GPIO!
typedef struct {
	volatile uint32_t MODER;   /*!< Give a short description,     Address offset: 0x00  */
	volatile uint32_t OTYPER;  /*!< TODO,                         Address offset: 0x04  */
	volatile uint32_t OSPEEDR; /*!< TODO,                         Address offset: 0x08  */
	volatile uint32_t PUPDR;   /*!< TODO,                         Address offset: 0x0c  */
	volatile uint32_t IDR;     /*!< TODO,                         Address offset: 0x10  */
	volatile uint32_t ODR;     /*!< TODO,                         Address offset: 0x14  */
	volatile uint32_t BSRR;    /*!< TODO,                         Address offset: 0x18  */
	volatile uint32_t LCKR;    /*!< TODO,                         Address offset: 0x1c  */
	volatile uint32_t AFR[2];  /*!< TODO,                         Address offset: 0x20-0x24 */
} GPIO_RegDef_t;

typedef struct        // RM -> Table 33. RCC register map and reset values for STM32F42xxx and STM32F43xxx
{
	__vo uint32_t CR;        /*!< TODO, Address offset: 0x00 */
	__vo uint32_t PLLCFGR;   /*!< TODO, Address offset: 0x04 */
	__vo uint32_t CFGR;      /*!< TODO, Address offset: 0x08 */
	__vo uint32_t CIR;       /*!< TODO, Address offset: 0x0c */
	__vo uint32_t AHB1RSTR;  /*!< TODO, Address offset: 0x10 */
	__vo uint32_t AHB2RSTR;  /*!< TODO, Address offset: 0x14 */
	__vo uint32_t AHB3RSTR;  /*!< TODO, Address offset: 0x18 */
	     uint32_t RESERVED0; /*!< Reserved, 0x1C             */
	__vo uint32_t APB1RSTR;  /*!< TODO, Address offset: 0x20 */
	__vo uint32_t APB2RSTR;  /*!< TODO, Address offset: 0x24 */
	  uint32_t RESERVED1[2]; /*!< Reserved, 0x28-0x2c        */
	__vo uint32_t AHB1ENR;   /*!< TODO, Address offset: 0x30 */
	__vo uint32_t AHB2ENR;   /*!< TODO, Address offset: 0x34 */
	__vo uint32_t AHB3ENR;   /*!< TODO, Address offset: 0x38 */
	   uint32_t RESERVED2;   /*!< Reserved, 0x3c             */
	__vo uint32_t APB1ENR;   /*!< TODO, Address offset: 0x40 */
	__vo uint32_t APB2ENR;   /*!< TODO, Address offset: 0x44 */
	uint32_t RESERVED3[2];   /*!< Reserved, 0x48-0x4c        */
	__vo uint32_t AHB1LPENR; /*!< TODO, Address offset: 0x50 */
	__vo uint32_t AHB2LPENR; /*!< TODO, Address offset: 0x54 */
	__vo uint32_t AHB3LPENR; /*!< TODO, Address offset: 0x58 */
	uint32_t RESERVED4;      /*!< Reserved, 0x5c             */
	__vo uint32_t APB1LPENR; /*!< TODO, Address offset: 0x60 */
	__vo uint32_t APB2LPENR; /*!< TODO, Address offset: 0x64 */
	uint32_t RESERVED5[2];   /*!< Reserved, 0x68-0x6c        */
	__vo uint32_t BDCR;      /*!< TODO, Address offset: 0x70 */
	__vo uint32_t CSR;       /*!< TODO, Address offset: 0x74 */
	uint32_t RESERVED6[2];   /*!< Reserved, 0x78-0x7c         */
	__vo uint32_t SSCGR;     /*!< TODO, Address offset: 0x80 */
	__vo uint32_t PLLI2SCFGR;/*!< TODO, Address offset: 0x84 */
	__vo uint32_t PLLSAICFGR;/*!< TODO, Address offset: 0x88 */
	__vo uint32_t DCKCFGR;   /*!< TODO, Address offset: 0x8c */

} RCC_RegDef_t; // Дали название нашей структуре.

// peripherak definitions (Peripheral base address typecasted to xxx_RegDef_t)
// RM -> Table 63. External interrupt/event controller register map and reset values
typedef struct {
	__vo uint32_t IMR;   /*!< TODO,                         Address offset: 0x00  */
	__vo uint32_t EMR;   /*!< TODO,                         Address offset: 0x04  */
	__vo uint32_t RTSR;  /*!< TODO,                         Address offset: 0x08  */
	__vo uint32_t FTSR;  /*!< TODO,                         Address offset: 0x0c  */
	__vo uint32_t SWIER; /*!< TODO,                         Address offset: 0x10  */
	__vo uint32_t PR;    /*!< TODO,                         Address offset: 0x14  */
} EXTI_RegDef_t;

// peripherak definitions (Peripheral base address typecasted to xxx_RegDef_t)
// RM -> SYSCFG register maps for STM32F42xxx and STM32F43xxx
typedef struct {
	__vo uint32_t MEMRMP;     /*!< TODO,                    Address offset: 0x00      */
	__vo uint32_t PMC;        /*!< TODO,                    Address offset: 0x04      */
	__vo uint32_t EXTICR[4];  /*!< TODO,                    Address offset: 0x8-0x14  */
	uint32_t RESERVED1[2];   /*!< TODO,                     Address offset: 0x18-0x1c */
	__vo uint32_t CMPCR;      /*!< TODO,                    Address offset: 0x20      */
	uint32_t RESERVED2[2];   /*!< TODO,                     Address offset: 0x24-0x28 */ // В моей таблице нет этого!
	__vo uint32_t CFGR;      /*!< TODO,                    Address offset: 0x2c       */ // В моей таблице нет этого!
} SYSCFG_RegDef_t;

//6. Peripheral definitions (Peripheral base address typecasted to xxx_RegDef_t)
#define GPIOA ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF ((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG ((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH ((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI ((GPIO_RegDef_t*)GPIOI_BASEADDR)

#define RCC ((RCC_RegDef_t*)RCC_BASEADDR)

#define EXTI ((EXTI_RegDef_t*)EXTI_BASEADDR)

#define SYSCFG ((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

// Clock Enable Macros for GPIOx EN peripherals см. RM -> RCC AHB1 peripheral clock register (RCC_AHB1ENR)
#define GPIOA_PCLK_EN() (RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN() (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN() (RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN() (RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN() (RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN() (RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN() (RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN() (RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN() (RCC->AHB1ENR |= (1 << 8))
// Clock Disable Macros for GPIOx DI peripherals см. RM -> RCC AHB1 peripheral clock register (RCC_AHB1ENR)
#define GPIOA_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 8))

// Clock Enable Macros for I2Cx EN peripherals см. RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)
#define I2C1_PCLK_EN() (RCC->AHB1ENR |= (1 << 21))
#define I2C2_PCLK_EN() (RCC->AHB1ENR |= (1 << 22))
#define I2C3_PCLK_EN() (RCC->AHB1ENR |= (1 << 23))
// Clock Disable Macros for I2Cx DI peripherals см. RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)
#define I2C1_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 23))

// Clock Enable Macros for SPIx EN peripherals
#define SPI1_PCLK_EN() (RCC->AHB2ENR |= (1 << 12)) // см.RM -> RCC APB2 peripheral clock enable register (RCC_APB2ENR)
#define SPI2_PCLK_EN() (RCC->AHB1ENR |= (1 << 14)) // см.RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)
#define SPI3_PCLK_EN() (RCC->AHB1ENR |= (1 << 15)) // см.RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)
// Clock Disable Macros for SPIx DI peripherals
#define SPI1_PCLK_DI() (RCC->AHB2ENR &= ~(1 << 12)) // см.RM -> RCC APB2 peripheral clock enable register (RCC_APB2ENR)
#define SPI2_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 14)) // см.RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)
#define SPI3_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 15)) // см.RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)

// Clock Enable Macros for USART EN peripherals
#define USART1_PCLK_EN() (RCC->AHB1ENR |= (1 << 1)) // см.RM -> RCC APB2 peripheral clock enable register (RCC_APB2ENR)
#define USART2_PCLK_EN() (RCC->AHB1ENR |= (1 << 17)) // см.RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)
#define USART3_PCLK_EN() (RCC->AHB1ENR |= (1 << 18)) // см.RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)
#define UART4_PCLK_EN() (RCC->AHB1ENR |= (1 << 19)) // см.RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)
#define UART5_PCLK_EN() (RCC->AHB1ENR |= (1 << 20)) // см.RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)
#define USART6_PCLK_EN() (RCC->AHB2ENR |= (1 << 6)) // см.RM -> RCC APB2 peripheral clock enable register (RCC_APB2ENR)
// Clock Disable Macros for USART DI peripherals
#define USART1_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 1)) // см.RM -> RCC APB2 peripheral clock enable register (RCC_APB2ENR)
#define USART2_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 17)) // см.RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)
#define USART3_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 18)) // см.RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)
#define UART4_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 19)) // см.RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)
#define UART5_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 20)) // см.RM -> RCC APB1 peripheral clock enable register (RCC_APB1ENR)
#define USART6_PCLK_DI() (RCC->AHB2ENR &= ~(1 << 6)) // см.RM -> RCC APB2 peripheral clock enable register (RCC_APB2ENR)

// Clock Enable Macros for SYSCFG EN peripherals
#define SYSCFG_PCLK_EN() (RCC->AHB2ENR |= (1 << 14)) // см.RM -> RCC APB2 peripheral clock enable register (RCC_APB2ENR)
// Clock Disable Macros for SYSCFG DI peripherals
#define SYSCFG_PCLK_DI() (RCC->AHB2ENR &= ~(1 << 14)) // см.RM -> RCC APB2 peripheral clock enable register (RCC_APB2ENR)

// Macros to reset GPIOx peripherals RM -> RCC AHB1 peripheral reset register (RCC_AHB1RSTR)
#define GPIOA_REG_RESET()   do{(RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); }while(0)
#define GPIOB_REG_RESET()   do{(RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); }while(0)
#define GPIOC_REG_RESET()   do{(RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); }while(0)
#define GPIOD_REG_RESET()   do{(RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); }while(0)
#define GPIOE_REG_RESET()   do{(RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); }while(0)
#define GPIOF_REG_RESET()   do{(RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); }while(0)
#define GPIOG_REG_RESET()   do{(RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); }while(0)
#define GPIOH_REG_RESET()   do{(RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); }while(0)
#define GPIOI_REG_RESET()   do{(RCC->AHB1RSTR |= (1 << 8)); (RCC->AHB1RSTR &= ~(1 << 8)); }while(0)

// returns port code for given GPIOx base address   Conditional or ternary operators in C:
#define GPIO_BASEADDR_TO_CODE(x)    ( (x == GPIOA)?0:\
		                              (x == GPIOB)?1:\
				                      (x == GPIOC)?2:\
			                          (x == GPIOD)?3:\
			                          (x == GPIOE)?4:\
			               			  (x == GPIOF)?5:\
			               			  (x == GPIOG)?6:\
			                 		  (x == GPIOH)?7:\
			                 		  (x == GPIOI)?8: 0 )
		                              // Если X равен GPIOA то 0 иначе если X равен GPIOB то 1 иначе ...

// IRQ(Interrupt Request numbers of MCU) Numbers of STM32F407x MCU см RF -> Table 62. Vector table for STM32F42xxx and STM32F43xxx (continued)
// Это на стороне процессора включение прерыаний! (NVIC)
#define IRQ_NO_EXTI0       6
#define IRQ_NO_EXTI1       7
#define IRQ_NO_EXTI2       8
#define IRQ_NO_EXTI3       9
#define IRQ_NO_EXTI4       10
#define IRQ_NO_EXTI9_5     23
#define IRQ_NO_EXTI15_10   40

// Macros for all the possible priority levels
#define NVIC_IRQ_PRI0   0
#define NVIC_IRQ_PRI1   1
#define NVIC_IRQ_PRI2   2
#define NVIC_IRQ_PRI3   3
#define NVIC_IRQ_PRI4   4
#define NVIC_IRQ_PRI5   5
#define NVIC_IRQ_PRI6   6
#define NVIC_IRQ_PRI7   7
#define NVIC_IRQ_PRI8   8
#define NVIC_IRQ_PRI9   9
#define NVIC_IRQ_PRI10   10
#define NVIC_IRQ_PRI11   11
#define NVIC_IRQ_PRI12   12
#define NVIC_IRQ_PRI13   13
#define NVIC_IRQ_PRI14   14
#define NVIC_IRQ_PRI15   15


// Эти макросы пишем здесь т.к. этот файл виден из любого драйвера в данном проекте!
#define ENABLE          1       // Это параметр для GPIO_PeriClockControl (в файле stm32f407_gpio_driver.h)
#define DISABLE         0		// Это параметр для GPIO_PeriClockControl (в файле stm32f407_gpio_driver.h)
#define SET             ENABLE
#define RESET           DISABLE
#define GPIO_PIN_SET    SET     // Это параметр для GPIO_ReadFromOutputPin (в файле stm32f407_gpio_driver.h)
#define GPIO_PIN_RESET  RESET   // Это параметр для GPIO_ReadFromOutputPin (в файле stm32f407_gpio_driver.h)




#include "stm32f407xx_gpio_driver.h"



















#endif /* INC_STM32F407XX_H_ */











