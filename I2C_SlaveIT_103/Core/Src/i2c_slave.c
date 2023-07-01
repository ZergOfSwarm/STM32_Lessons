/*
 * i2c_slave.c
 *
 *  Created on: Jun 9, 2023
 *      Author: controllerstech
 */


#include "main.h"
#include "i2c_slave.h"

extern I2C_HandleTypeDef hi2c1;

#define RxSIZE  6
uint8_t RxData[RxSIZE];
uint8_t rxcount = 0;

int countAddr = 0; // Для подсчета количества срабатываний HAL_I2C_AddrCallback();
int countrxcplt = 0;
int counterror = 0;

void process_data (void)
{
	//do something here
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);// Снова установим в режим прослушивания.
}

// Если совпадет адрес запроса от MASTER и адрес SLAVE то, сработает это Callback!
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	if (TransferDirection == I2C_DIRECTION_TRANSMIT)  // if the master wants to transmit the data
	{
		rxcount = 0;
		countAddr++;
		// receive using sequential function.
		// Для получения данных от MASTER мы используем sequential function для сохранения в RxData.
		// The I2C_FIRST_AND_LAST_FRAME implies that the slave will send a NACK after receiving "entered" num of bytes
		HAL_I2C_Slave_Sequential_Receive_IT(hi2c, RxData+rxcount, 1, I2C_FIRST_FRAME);
	}
	else  // if the master requests the data from the slave
	{
		Error_Handler();  // call error handler
	}
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) // Когда получили данные их нужно сохранить.
{
	rxcount++;
	if (rxcount < RxSIZE)
	{
		if (rxcount == RxSIZE-1)
		{
			HAL_I2C_Slave_Sequential_Receive_IT(hi2c, RxData+rxcount, 1, I2C_LAST_FRAME);
		}
		else
		{
			HAL_I2C_Slave_Sequential_Receive_IT(hi2c, RxData+rxcount, 1, I2C_NEXT_FRAME);
		}
	}

	if (rxcount == RxSIZE)
	{
		process_data();
	}

}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	counterror++;
	uint32_t errorcode = HAL_I2C_GetError(hi2c);
	if (errorcode == 4)  // AF error
	{
		process_data();
	}
	HAL_I2C_EnableListen_IT(hi2c);
}
