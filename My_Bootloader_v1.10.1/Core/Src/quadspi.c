/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    quadspi.c
  * @brief   This file provides code for the configuration
  *          of the QUADSPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "quadspi.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
/* USER CODE END 0 */

QSPI_HandleTypeDef hqspi;

/* QUADSPI init function */
void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 1;
  hqspi.Init.FifoThreshold = 1;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 22;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_2_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

void HAL_QSPI_MspInit(QSPI_HandleTypeDef* qspiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspInit 0 */

  /* USER CODE END QUADSPI_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
    PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* QUADSPI clock enable */
    __HAL_RCC_QSPI_CLK_ENABLE();

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**QUADSPI GPIO Configuration
    PE2     ------> QUADSPI_BK1_IO2
    PB2     ------> QUADSPI_CLK
    PD11     ------> QUADSPI_BK1_IO0
    PD12     ------> QUADSPI_BK1_IO1
    PD13     ------> QUADSPI_BK1_IO3
    PB6     ------> QUADSPI_BK1_NCS
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* QUADSPI interrupt Init */
    HAL_NVIC_SetPriority(QUADSPI_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(QUADSPI_IRQn);
  /* USER CODE BEGIN QUADSPI_MspInit 1 */

  /* USER CODE END QUADSPI_MspInit 1 */
  }
}

void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* qspiHandle)
{

  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

  /* USER CODE END QUADSPI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_QSPI_CLK_DISABLE();

    /**QUADSPI GPIO Configuration
    PE2     ------> QUADSPI_BK1_IO2
    PB2     ------> QUADSPI_CLK
    PD11     ------> QUADSPI_BK1_IO0
    PD12     ------> QUADSPI_BK1_IO1
    PD13     ------> QUADSPI_BK1_IO3
    PB6     ------> QUADSPI_BK1_NCS
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13);

    /* QUADSPI interrupt Deinit */
    HAL_NVIC_DisableIRQ(QUADSPI_IRQn);
  /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

  /* USER CODE END QUADSPI_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

int8_t QSPI_W25Qxx_Init(void)
{
    uint32_t	Device_ID;	// 器件ID

    MX_QUADSPI_Init();							//инициализация QSPI
    QSPI_W25Qxx_Reset();							// 复位器件
    Device_ID = QSPI_W25Qxx_ReadID(); 		// 读取器件ID

    if( Device_ID == W25Qxx_FLASH_ID )		// 进行匹配
    {
        return QSPI_W25Qxx_OK;			// 返回成功标志
    }
    else
    {
        return W25Qxx_ERROR_INIT;		// 返回错误标志
    }
}

/*************************************************************************************************
*	�? �? �?: QSPI_W25Qxx_AutoPollingMemReady
*	入口参数: �?
*	�? �? �?: QSPI_W25Qxx_OK - 通信正常结束，W25Qxx_ERROR_AUTOPOLLING - 轮询等待无响�?
*	函数功能: 使用自动轮询标志查询，等待�?�信结束
*	�?    �?: 每一次�?�信都应该调用次函数，等待�?�信结束，避免错误的操作
**************************************************************************************************/

int8_t QSPI_W25Qxx_AutoPollingMemReady(void)
{
    QSPI_CommandTypeDef     s_command;	   // QSPI传输配置
    QSPI_AutoPollingTypeDef s_config;		// 轮询比较相关配置参数

    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;			// 1线指令模�?
    s_command.AddressMode       = QSPI_ADDRESS_NONE;					// 无地�?模式
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;			//	无交替字�?
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;	     	 	// 禁止DDR模式
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;	   	// DDR模式中数据延迟，这里用不�?
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;	   	//	每次传输数据都发送指�?
    s_command.DataMode          = QSPI_DATA_1_LINE;						// 1线数据模�?
    s_command.DummyCycles       = 0;											//	空周期个�?
    s_command.Instruction       = W25Qxx_CMD_ReadStatus_REG1;	   // 读状态信息寄存器

// 不停的查�? W25Qxx_CMD_ReadStatus_REG1 寄存器，将读取到的状态字节中�? W25Qxx_Status_REG1_BUSY 不停的与0作比�?
// 读状态寄存器1的第0位（只读），Busy标志位，当正在擦�?/写入数据/写命令时会被�?1，空闲或通信结束�?0

    s_config.Match           = 0;   									//	匹配�?
    s_config.MatchMode       = QSPI_MATCH_MODE_AND;	      	//	与运�?
    s_config.Interval        = 0x10;	                     	//	轮询间隔
    s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;	// 自动停止模式
    s_config.StatusBytesSize = 1;	                        	//	状�?�字节数
    s_config.Mask            = W25Qxx_Status_REG1_BUSY;	   // 对在轮询模式下接收的状�?�字节进行屏蔽，只比较需要用到的�?

    // 发�?�轮询等待命�?
    if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING; // 轮询等待无响�?
    }
    return QSPI_W25Qxx_OK; // 通信正常结束

}

/*************************************************************************************************
*	�? �? �?: QSPI_W25Qxx_Reset
*	入口参数: �?
*	�? �? �?: QSPI_W25Qxx_OK - 复位成功，W25Qxx_ERROR_INIT - 初始化错�?
*	函数功能: 复位器件
*	Функция: сбросить устройство
*************************************************************************************************/

int8_t QSPI_W25Qxx_Reset(void)
{
    QSPI_CommandTypeDef s_command;	// QSPI传输配置

    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;   	// 1线指令模�?
    s_command.AddressMode 		 = QSPI_ADDRESS_NONE;   			// 无地�?模式
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 	// 无交替字�?
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;     	// 禁止DDR模式
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY; 	// DDR模式中数据延迟，这里用不�?
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;	 	// 每次传输数据都发送指�?
    s_command.DataMode 			 = QSPI_DATA_NONE;       			// 无数据模�?
    s_command.DummyCycles 		 = 0;                     			// 空周期个�?
    s_command.Instruction 		 = W25Qxx_CMD_EnableReset;       // 执行复位使能命令

    // 发�?�复位使能命�?
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_INIT;			// 如果发�?�失败，返回错误信息
    }
    // 使用自动轮询标志位，等待通信结束
    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;	// 轮询等待无响�?
    }

    s_command.Instruction  = W25Qxx_CMD_ResetDevice;     // 复位器件命令

    //发�?�复位器件命�?
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_INIT;		  // 如果发�?�失败，返回错误信息
    }
    // 使用自动轮询标志位，等待通信结束
    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;	// 轮询等待无响�?
    }
    return QSPI_W25Qxx_OK;	// 复位成功
}

/*************************************************************************************************
*	�? �? �?: QSPI_W25Qxx_ReadID
*	入口参数: �?
*	�? �? �?: W25Qxx_ID - 读取到的器件ID，W25Qxx_ERROR_INIT - 通信、初始化错误
*	函数功能: 初始�? QSPI 配置，读取器件ID
*	�?    �?: �?
**************************************************************************************************/

uint32_t QSPI_W25Qxx_ReadID(void)
{
    QSPI_CommandTypeDef s_command;	// QSPI传输配置
    uint8_t	QSPI_ReceiveBuff[3];		// 存储QSPI读到的数�?
    uint32_t	W25Qxx_ID;					// 器件的ID

    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    // 1线指令模�?
    s_command.AddressSize       = QSPI_ADDRESS_24_BITS;     	 // 24位地�?
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  // 无交替字�?
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;      // 禁止DDR模式
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  // DDR模式中数据延迟，这里用不�?
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;	 // 每次传输数据都发送指�?
    s_command.AddressMode		 = QSPI_ADDRESS_NONE;   		 // 无地�?模式
    s_command.DataMode			 = QSPI_DATA_1_LINE;       	 // 1线数据模�?
    s_command.DummyCycles 		 = 0;                   		 // 空周期个�?
    s_command.NbData 				 = 3;                          // 传输数据的长�?
    s_command.Instruction 		 = W25Qxx_CMD_JedecID;         // 执行读器件ID命令

    // 发�?�指�?
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
//		return W25Qxx_ERROR_INIT;		// 如果发�?�失败，返回错误信息
    }
    // 接收数据
    if (HAL_QSPI_Receive(&hqspi, QSPI_ReceiveBuff, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
//		return W25Qxx_ERROR_TRANSMIT;  // 如果接收失败，返回错误信�?
    }
    // 将得到的数据组合成ID
    W25Qxx_ID = (QSPI_ReceiveBuff[0] << 16) | (QSPI_ReceiveBuff[1] << 8 ) | QSPI_ReceiveBuff[2];

    return W25Qxx_ID; // 返回ID
}



/*************************************************************************************************
*	�? �? �?: QSPI_W25Qxx_MemoryMappedMode
*	入口参数: �?
*	�? �? �?: QSPI_W25Qxx_OK - 写使能成功，W25Qxx_ERROR_WriteEnable - 写使能失�?
*	函数功能: 将QSPI设置为内存映射模�?
*	�?    �?: 设置为内存映射模式时，只能读，不能写！！�?
**************************************************************************************************/

int8_t QSPI_W25Qxx_MemoryMappedMode(void)
{
    QSPI_CommandTypeDef      s_command;				 // QSPI传输配置
    QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;	 // 内存映射访问参数

    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    		// 1线指令模�?
    s_command.AddressSize       = QSPI_ADDRESS_24_BITS;            // 24位地�?
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  		// 无交替字�?
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;     		// 禁止DDR模式
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY; 		// DDR模式中数据延迟，这里用不�?
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;			// 每次传输数据都发送指�?
    s_command.AddressMode 		 = QSPI_ADDRESS_4_LINES; 				// 4线地�?模式
    s_command.DataMode    		 = QSPI_DATA_4_LINES;    				// 4线数据模�?
    s_command.DummyCycles 		 = 6;                    				// 空周期个�?
    s_command.Instruction 		 = W25Qxx_CMD_FastReadQuad_IO; 		// 1-4-4模式�?(1线指�?4线地�?4线数�?)，快速读取指�?

    s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE; // 禁用超时计数�?, nCS 保持�?活状�?
    s_mem_mapped_cfg.TimeOutPeriod     = 0;									 // 超时判断周期

    QSPI_W25Qxx_Reset();		// 复位W25Qxx

    if (HAL_QSPI_MemoryMapped(&hqspi, &s_command, &s_mem_mapped_cfg) != HAL_OK)	// 进行配置
    {
        return W25Qxx_ERROR_MemoryMapped; 	// 设置内存映射模式错误
    }
    return QSPI_W25Qxx_OK; // 配置成功
}

/*************************************************************************************************
*	�? �? �?: QSPI_W25Qxx_WriteEnable
*	入口参数: �?
*	�? �? �?: QSPI_W25Qxx_OK - 写使能成功，W25Qxx_ERROR_WriteEnable - 写使能失�?
*	函数功能: 发�?�写使能命令
*	�?    �?: �?
**************************************************************************************************/

int8_t QSPI_W25Qxx_WriteEnable(void)
{
    QSPI_CommandTypeDef     s_command;	   // QSPI传输配置
    QSPI_AutoPollingTypeDef s_config;		// 轮询比较相关配置参数

    s_command.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;    	// 1线指令模�?
    s_command.AddressMode 			= QSPI_ADDRESS_NONE;   		      // 无地�?模式
    s_command.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;  	// 无交替字�?
    s_command.DdrMode           	= QSPI_DDR_MODE_DISABLE;      	// 禁止DDR模式
    s_command.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;  	// DDR模式中数据延迟，这里用不�?
    s_command.SIOOMode          	= QSPI_SIOO_INST_EVERY_CMD;		// 每次传输数据都发送指�?
    s_command.DataMode 				= QSPI_DATA_NONE;       	      // 无数据模�?
    s_command.DummyCycles 			= 0;                   	         // 空周期个�?
    s_command.Instruction	 		= W25Qxx_CMD_WriteEnable;      	// 发�?�写使能命令

    // 发�?�写使能命令
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_WriteEnable;	//
    }

// 不停的查�? W25Qxx_CMD_ReadStatus_REG1 寄存器，将读取到的状态字节中�? W25Qxx_Status_REG1_WEL 不停的与 0x02 作比�?
// 读状态寄存器1的第1位（只读），WEL写使能标志位，该标志位为1时，代表可以进行写操�?

    s_config.Match           = 0x02;  								// 匹配�?
    s_config.Mask            = W25Qxx_Status_REG1_WEL;	 		// 读状态寄存器1的第1位（只读），WEL写使能标志位，该标志位为1时，代表可以进行写操�?
    s_config.MatchMode       = QSPI_MATCH_MODE_AND;			 	// 与运�?
    s_config.StatusBytesSize = 1;									 	// 状�?�字节数
    s_config.Interval        = 0x10;							 		// 轮询间隔
    s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;	// 自动停止模式

    s_command.Instruction    = W25Qxx_CMD_ReadStatus_REG1;	// 读状态信息寄存器
    s_command.DataMode       = QSPI_DATA_1_LINE;					// 1线数据模�?
    s_command.NbData         = 1;										// 数据长度

    // 发�?�轮询等待命�?
    if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING; 	// 轮询等待无响�?
    }
    return QSPI_W25Qxx_OK;  // 通信正常结束
}

/*************************************************************************************************
*
*	�? �? �?: QSPI_W25Qxx_SectorErase
*
*	入口参数: SectorAddress - 要擦除的地址
*
*	�? �? �?: QSPI_W25Qxx_OK - 擦除成功
*			    W25Qxx_ERROR_Erase - 擦除失败
*				 W25Qxx_ERROR_AUTOPOLLING - 轮询等待无响�?
*
*	函数功能: 进行扇区擦除操作，每次擦�?4K字节
*
*	�?    �?: 1.按照 W25Q64JV 数据手册给出的擦除参考时间，典型值为 45ms，最大�?�为400ms
*				 2.实际的擦除�?�度可能大于45ms，也可能小于45ms
*				 3.flash使用的时间越长，擦除�?�?时间也会越长
*
**************************************************************************************************/

int8_t QSPI_W25Qxx_SectorErase(uint32_t SectorAddress)
{
    QSPI_CommandTypeDef s_command;	// QSPI传输配置

    s_command.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;    // 1线指令模�?
    s_command.AddressSize       	= QSPI_ADDRESS_24_BITS;       // 24位地�?模式
    s_command.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;  //	无交替字�?
    s_command.DdrMode           	= QSPI_DDR_MODE_DISABLE;      // 禁止DDR模式
    s_command.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;  // DDR模式中数据延迟，这里用不�?
    s_command.SIOOMode          	= QSPI_SIOO_INST_EVERY_CMD;	// 每次传输数据都发送指�?
    s_command.AddressMode 			= QSPI_ADDRESS_1_LINE;        // 1线地�?模式
    s_command.DataMode 				= QSPI_DATA_NONE;             // 无数�?
    s_command.DummyCycles 			= 0;                          // 空周期个�?
    s_command.Address           	= SectorAddress;              // 要擦除的地址
    s_command.Instruction	 		= W25Qxx_CMD_SectorErase;     // 扇区擦除命令

    // 发�?�写使能
    if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;		// 写使能失�?
    }
    // 发出擦除命令
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_Erase;				// 擦除失败
    }
    // 使用自动轮询标志位，等待擦除的结�?
    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;		// 轮询等待无响�?
    }
    return QSPI_W25Qxx_OK; // 擦除成功
}

/*************************************************************************************************
*
*	�? �? �?: QSPI_W25Qxx_BlockErase_32K
*
*	入口参数: SectorAddress - 要擦除的地址
*
*	�? �? �?: QSPI_W25Qxx_OK - 擦除成功
*			    W25Qxx_ERROR_Erase - 擦除失败
*				 W25Qxx_ERROR_AUTOPOLLING - 轮询等待无响�?
*
*	函数功能: 进行块擦除操作，每次擦除32K字节
*
*	�?    �?: 1.按照 W25Q64JV 数据手册给出的擦除参考时间，典型值为 120ms，最大�?�为1600ms
*				 2.实际的擦除�?�度可能大于120ms，也可能小于120ms
*				 3.flash使用的时间越长，擦除�?�?时间也会越长
*
*************************************************************************************************/

int8_t QSPI_W25Qxx_BlockErase_32K (uint32_t SectorAddress)
{
    QSPI_CommandTypeDef s_command;	// QSPI传输配置

    s_command.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;    // 1线指令模�?
    s_command.AddressSize       	= QSPI_ADDRESS_24_BITS;       // 24位地�?模式
    s_command.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;  //	无交替字�?
    s_command.DdrMode           	= QSPI_DDR_MODE_DISABLE;      // 禁止DDR模式
    s_command.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;  // DDR模式中数据延迟，这里用不�?
    s_command.SIOOMode          	= QSPI_SIOO_INST_EVERY_CMD;	// 每次传输数据都发送指�?
    s_command.AddressMode 			= QSPI_ADDRESS_1_LINE;        // 1线地�?模式
    s_command.DataMode 				= QSPI_DATA_NONE;             // 无数�?
    s_command.DummyCycles 			= 0;                          // 空周期个�?
    s_command.Address           	= SectorAddress;              // 要擦除的地址
    s_command.Instruction	 		= W25Qxx_CMD_BlockErase_32K;  // 块擦除命令，每次擦除32K字节

    // 发�?�写使能
    if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;		// 写使能失�?
    }
    // 发出擦除命令
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_Erase;				// 擦除失败
    }
    // 使用自动轮询标志位，等待擦除的结�?
    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;		// 轮询等待无响�?
    }
    return QSPI_W25Qxx_OK;	// 擦除成功
}

/*************************************************************************************************
*
*	�? �? �?: QSPI_W25Qxx_BlockErase_64K
*
*	入口参数: SectorAddress - 要擦除的地址
*
*	�? �? �?: QSPI_W25Qxx_OK - 擦除成功
*			    W25Qxx_ERROR_Erase - 擦除失败
*				 W25Qxx_ERROR_AUTOPOLLING - 轮询等待无响�?
*
*	函数功能: 进行块擦除操作，每次擦除64K字节
*
*	�?    �?: 1.按照 W25Q64JV 数据手册给出的擦除参考时间，典型值为 150ms，最大�?�为2000ms
*				 2.实际的擦除�?�度可能大于150ms，也可能小于150ms
*				 3.flash使用的时间越长，擦除�?�?时间也会越长
*				 4.实际使用建议使用64K擦除，擦除的时间�?�?
*
**************************************************************************************************/

int8_t QSPI_W25Qxx_BlockErase_64K (uint32_t SectorAddress)
{
    QSPI_CommandTypeDef s_command;	// QSPI传输配置

    s_command.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;    // 1线指令模�?
    s_command.AddressSize       	= QSPI_ADDRESS_24_BITS;       // 24位地�?模式
    s_command.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;  //	无交替字�?
    s_command.DdrMode           	= QSPI_DDR_MODE_DISABLE;      // 禁止DDR模式
    s_command.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;  // DDR模式中数据延迟，这里用不�?
    s_command.SIOOMode          	= QSPI_SIOO_INST_EVERY_CMD;	// 每次传输数据都发送指�?
    s_command.AddressMode 			= QSPI_ADDRESS_1_LINE;        // 1线地�?模式
    s_command.DataMode 				= QSPI_DATA_NONE;             // 无数�?
    s_command.DummyCycles 			= 0;                          // 空周期个�?
    s_command.Address           	= SectorAddress;              // 要擦除的地址
    s_command.Instruction	 		= W25Qxx_CMD_BlockErase_64K;  // 块擦除命令，每次擦除64K字节

    // 发�?�写使能
    if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;	// 写使能失�?
    }
    // 发出擦除命令
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_Erase;			// 擦除失败
    }
    // 使用自动轮询标志位，等待擦除的结�?
    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;	// 轮询等待无响�?
    }
    return QSPI_W25Qxx_OK;		// 擦除成功
}

/*************************************************************************************************
*
*	�? �? �?: QSPI_W25Qxx_ChipErase
*
*	入口参数: �?
*
*	�? �? �?: QSPI_W25Qxx_OK - 擦除成功
*			    W25Qxx_ERROR_Erase - 擦除失败
*				 W25Qxx_ERROR_AUTOPOLLING - 轮询等待无响�?
*
*	函数功能: 进行整片擦除操作
*
*	�?    �?: 1.按照 W25Q64JV 数据手册给出的擦除参考时间，典型值为 20s，最大�?�为100s
*				 2.实际的擦除�?�度可能大于20s，也可能小于20s
*				 3.flash使用的时间越长，擦除�?�?时间也会越长
*
*************************************************************************************************/

int8_t QSPI_W25Qxx_ChipErase (void)
{
    QSPI_CommandTypeDef s_command;		// QSPI传输配置
    QSPI_AutoPollingTypeDef s_config;	// 轮询等待配置参数

    s_command.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;    // 1线指令模�?
    s_command.AddressSize       	= QSPI_ADDRESS_24_BITS;       // 24位地�?模式
    s_command.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;  //	无交替字�?
    s_command.DdrMode           	= QSPI_DDR_MODE_DISABLE;      // 禁止DDR模式
    s_command.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;  // DDR模式中数据延迟，这里用不�?
    s_command.SIOOMode          	= QSPI_SIOO_INST_EVERY_CMD;	// 每次传输数据都发送指�?
    s_command.AddressMode 			= QSPI_ADDRESS_NONE;       	// 无地�?
    s_command.DataMode 				= QSPI_DATA_NONE;             // 无数�?
    s_command.DummyCycles 			= 0;                          // 空周期个�?
    s_command.Instruction	 		= W25Qxx_CMD_ChipErase;       // 擦除命令，进行整片擦�?

    // 发�?�写使能
    if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;	// 写使能失�?
    }
    // 发出擦除命令
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_Erase;		 // 擦除失败
    }

// 不停的查�? W25Qxx_CMD_ReadStatus_REG1 寄存器，将读取到的状态字节中�? W25Qxx_Status_REG1_BUSY 不停的与0作比�?
// 读状态寄存器1的第0位（只读），Busy标志位，当正在擦�?/写入数据/写命令时会被�?1，空闲或通信结束�?0

    s_config.Match           = 0;   									//	匹配�?
    s_config.MatchMode       = QSPI_MATCH_MODE_AND;	      	//	与运�?
    s_config.Interval        = 0x10;	                     	//	轮询间隔
    s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;	// 自动停止模式
    s_config.StatusBytesSize = 1;	                        	//	状�?�字节数
    s_config.Mask            = W25Qxx_Status_REG1_BUSY;	   // 对在轮询模式下接收的状�?�字节进行屏蔽，只比较需要用到的�?

    s_command.Instruction    = W25Qxx_CMD_ReadStatus_REG1;	// 读状态信息寄存器
    s_command.DataMode       = QSPI_DATA_1_LINE;					// 1线数据模�?
    s_command.NbData         = 1;										// 数据长度

    // W25Q64整片擦除的典型参考时间为20s，最大时间为100s，这里的超时等待�? W25Qxx_ChipErase_TIMEOUT_MAX �? 100S
    if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, W25Qxx_ChipErase_TIMEOUT_MAX) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;	 // 轮询等待无响�?
    }
    return QSPI_W25Qxx_OK;
}

/**********************************************************************************************************
*
*	�? �? �?: QSPI_W25Qxx_WritePage
*
*	入口参数: pBuffer 		 - 要写入的数据
*				 WriteAddr 		 - 要写�? W25Qxx 的地�?
*				 NumByteToWrite - 数据长度，最大只�?256字节
*
*	�? �? �?: QSPI_W25Qxx_OK 		     - 写数据成�?
*			    W25Qxx_ERROR_WriteEnable - 写使能失�?
*				 W25Qxx_ERROR_TRANSMIT	  - 传输失败
*				 W25Qxx_ERROR_AUTOPOLLING - 轮询等待无响�?
*
*	函数功能: 按页写入，最大只�?256字节，在数据写入之前，请务必完成擦除操作
*
*	�?    �?: 1.Flash的写入时间和擦除时间�?样，是限定的，并不是说QSPI驱动时钟133M就可以以这个速度进行写入
*				 2.按照 W25Q64JV 数据手册给出�? �?(256字节) 写入参�?�时间，典型值为 0.4ms，最大�?�为3ms
*				 3.实际的写入�?�度可能大于0.4ms，也可能小于0.4ms
*				 4.Flash使用的时间越长，写入�?�?时间也会越长
*				 5.在数据写入之前，请务必完成擦除操�?
*
***********************************************************************************************************/

int8_t QSPI_W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    QSPI_CommandTypeDef s_command;	// QSPI传输配置

    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    		// 1线指令模�?
    s_command.AddressSize       = QSPI_ADDRESS_24_BITS;            // 24位地�?
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  		// 无交替字�?
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;     		// 禁止DDR模式
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY; 		// DDR模式中数据延迟，这里用不�?
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;			// 每次传输数据都发送指�?
    s_command.AddressMode 		 = QSPI_ADDRESS_1_LINE; 				// 1线地�?模式
    s_command.DataMode    		 = QSPI_DATA_4_LINES;    				// 4线数据模�?
    s_command.DummyCycles 		 = 0;                    				// 空周期个�?
    s_command.NbData      		 = NumByteToWrite;      			   // 数据长度，最大只�?256字节
    s_command.Address     		 = WriteAddr;         					// 要写�? W25Qxx 的地�?
    s_command.Instruction 		 = W25Qxx_CMD_QuadInputPageProgram; // 1-1-4模式�?(1线指�?1线地�?4线数�?)，页编程指令

    // 写使�?
    if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;	// 写使能失�?
    }
    // 写命�?
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_TRANSMIT;		// 传输数据错误
    }
    // �?始传输数�?
    if (HAL_QSPI_Transmit(&hqspi, pBuffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_TRANSMIT;		// 传输数据错误
    }
    // 使用自动轮询标志位，等待写入的结�?
    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING; // 轮询等待无响�?
    }
    return QSPI_W25Qxx_OK;	// 写数据成�?
}

/**********************************************************************************************************
*
*	�? �? �?: QSPI_W25Qxx_WriteBuffer
*
*	入口参数: pBuffer 		 - 要写入的数据
*				 WriteAddr 		 - 要写�? W25Qxx 的地�?
*				 NumByteToWrite - 数据长度，最大不能超过flash芯片的大�?
*
*	�? �? �?: QSPI_W25Qxx_OK 		     - 写数据成�?
*			    W25Qxx_ERROR_WriteEnable - 写使能失�?
*				 W25Qxx_ERROR_TRANSMIT	  - 传输失败
*				 W25Qxx_ERROR_AUTOPOLLING - 轮询等待无响�?
*
*	函数功能: 写入数据，最大不能超过flash芯片的大小，请务必完成擦除操�?
*
*	�?    �?: 1.Flash的写入时间和擦除时间�?样，是有限定的，并不是说QSPI驱动时钟133M就可以以这个速度进行写入
*				 2.按照 W25Q64JV 数据手册给出�? �? 写入参�?�时间，典型值为 0.4ms，最大�?�为3ms
*				 3.实际的写入�?�度可能大于0.4ms，也可能小于0.4ms
*				 4.Flash使用的时间越长，写入�?�?时间也会越长
*				 5.在数据写入之前，请务必完成擦除操�?
*				 6.该函数移植于 stm32h743i_eval_qspi.c
*
**********************************************************************************************************/

int8_t QSPI_W25Qxx_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t Size)
{
    uint32_t end_addr, current_size, current_addr;
    uint8_t *write_data;  // 要写入的数据

    current_size = W25Qxx_PageSize - (WriteAddr % W25Qxx_PageSize); // 计算当前页还剩余的空�?

    if (current_size > Size)	// 判断当前页剩余的空间是否足够写入�?有数�?
    {
        current_size = Size;		// 如果足够，则直接获取当前长度
    }

    current_addr = WriteAddr;		// 获取要写入的地址
    end_addr = WriteAddr + Size;	// 计算结束地址
    write_data = pBuffer;			// 获取要写入的数据

    do
    {
        // 按页写入数据
        if(QSPI_W25Qxx_WritePage(write_data, current_addr, current_size) != QSPI_W25Qxx_OK)
        {
            return W25Qxx_ERROR_TRANSMIT;
        }

        else // 按页写入数据成功，进行下�?次写数据的准备工�?
        {
            current_addr += current_size;	// 计算下一次要写入的地�?
            write_data += current_size;	// 获取下一次要写入的数据存储区地址
            // 计算下一次写数据的长�?
            current_size = ((current_addr + W25Qxx_PageSize) > end_addr) ? (end_addr - current_addr) : W25Qxx_PageSize;
        }
    }
    while (current_addr < end_addr) ; // 判断数据是否全部写入完毕
    return QSPI_W25Qxx_OK;	// 写入数据成功

}

/**********************************************************************************************************************************
*
*	�? �? �?: QSPI_W25Qxx_ReadBuffer
*
*	入口参数: pBuffer 		 - 要读取的数据
*				 ReadAddr 		 - 要读�? W25Qxx 的地�?
*				 NumByteToRead  - 数据长度，最大不能超过flash芯片的大�?
*
*	�? �? �?: QSPI_W25Qxx_OK 		     - 读数据成�?
*				 W25Qxx_ERROR_TRANSMIT	  - 传输失败
*				 W25Qxx_ERROR_AUTOPOLLING - 轮询等待无响�?
*
*	函数功能: 读取数据，最大不能超过flash芯片的大�?
*
*	�?    �?: 1.Flash的读取�?�度取决于QSPI的�?�信时钟，最大不能超�?133M
*				 2.这里使用的是1-4-4模式�?(1线指�?4线地�?4线数�?)，快速读取指�? Fast Read Quad I/O
*				 3.使用快�?�读取指令是有空周期的，具体参�?�W25Q64JV的手�?  Fast Read Quad I/O  �?0xEB）指�?
*				 4.实际使用中，是否使用DMA、编译器的优化等级以及数据存储区的位�?(内部 TCM SRAM 或�?? AXI SRAM)都会影响读取的�?�度
*			    5.在本例程中，使用的是库函数进行直接读写，keil版本5.30，编译器AC6.14，编译等级Oz image size，读取�?�度�? 6.7M字节/S �?
*		         数据放在 TCM SRAM 或�?? AXI SRAM 都是差不多的结果
*		       6.因为CPU直接访问外设寄存器的效率很低，直接使用HAL库进行读写的话，速度很慢，使用MDMA进行读取，可以达�? 58M字节/S
*	          7. W25Q64JV �?允许的最高驱动频率为133MHz�?750的QSPI�?高驱动频率也�?133MHz ，但是对于HAL库函数直接读取�?�言�?
*		          驱动时钟超过15M已经不会对�?�能有提升，对�?�度要求高的场合可以用MDMA的方�?
*
*****************************************************************************************************************FANKE************/

int8_t QSPI_W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
    QSPI_CommandTypeDef s_command;	// QSPI传输配置

    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    		// 1线指令模�?
    s_command.AddressSize       = QSPI_ADDRESS_24_BITS;            // 24位地�?
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  		// 无交替字�?
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;     		// 禁止DDR模式
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY; 		// DDR模式中数据延迟，这里用不�?
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;			// 每次传输数据都发送指�?
    s_command.AddressMode 		 = QSPI_ADDRESS_4_LINES; 				// 4线地�?模式
    s_command.DataMode    		 = QSPI_DATA_4_LINES;    				// 4线数据模�?
    s_command.DummyCycles 		 = 6;                    				// 空周期个�?
    s_command.NbData      		 = NumByteToRead;      			   	// 数据长度，最大不能超过flash芯片的大�?
    s_command.Address     		 = ReadAddr;         					// 要读�? W25Qxx 的地�?
    s_command.Instruction 		 = W25Qxx_CMD_FastReadQuad_IO; 		// 1-4-4模式�?(1线指�?4线地�?4线数�?)，快速读取指�?

    // 发�?�读取命�?
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_TRANSMIT;		// 传输数据错误
    }

    //	接收数据

    if (HAL_QSPI_Receive(&hqspi, pBuffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_TRANSMIT;		// 传输数据错误
    }

    // 使用自动轮询标志位，等待接收的结�?
    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING; // 轮询等待无响�?
    }
    return QSPI_W25Qxx_OK;	// 读取数据成功
}
// 实验平台：反客STM32H750VBT6核心�?
/* USER CODE END 1 */
