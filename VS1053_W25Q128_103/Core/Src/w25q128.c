/*The W25Q128BV array is organized into 65,536 programmable pages of 256-bytes each. Up to 256 bytes
can be programmed at a time. Pages can be erased in groups of 16 (4KB sector erase), groups of 128
(32KB block erase), groups of 256 (64KB block erase) or the entire chip (chip erase). The W25Q128BV
has 4,096 erasable sectors and 256 erasable blocks respectively. The small 4KB sectors allow for greater
flexibility in applications that require data and parameter storage. (See Figure 2.) */
#include "w25q128.h"
#include "stm32f1xx.h"
#include "main.h"
//#include "delay.h"
#define SPI_PORT	SPI2
uint8_t response;
#define CS_ON GPIOB->ODR &= ~GPIO_ODR_ODR12
#define CS_OFF GPIOB->ODR |= GPIO_ODR_ODR12

 uint8_t SPI_SendReceiveByte( uint8_t data )
{
    uint8_t rxbyte;

    while ( !( SPI_PORT -> SR & SPI_SR_TXE ) );
    SPI_PORT -> DR = data;

    while ( !( SPI_PORT -> SR & SPI_SR_RXNE ) );
    rxbyte = SPI_PORT->DR;

    return rxbyte;
}
//=========================================================================//
//+++++++++++++++сброс флеш и влючение ее в работу+++++++++++++++++++++++++//
//=========================================================================//
void flash_reset()
{
    CS_ON;
    SPI_SendReceiveByte(F_ENABLE_RESET);
    CS_OFF;

    CS_ON;
    SPI_SendReceiveByte(F_RESET_DEVICE);
    CS_OFF;
	  HAL_Delay(1000);
}
//=========================================================================//
//++++++++++++++++++++++++++разрешаем запись+++++++++++++++++++++++++++++++//
//=========================================================================//
void flash_write_enable(){
CS_ON;
SPI_SendReceiveByte(F_WRITE_ENABLE);
CS_OFF;
}

//=========================================================================//
//++++++++++++++++++++++++читаем статус регистр++++++++++++++++++++++++++++//
//=========================================================================//
uint8_t flash_read_status_reg1()
{
    uint8_t ret;
    CS_ON;
    SPI_SendReceiveByte(F_READ_STATUS_REG1);
	  ret = SPI_SendReceiveByte(0x00);
    CS_OFF;

    return ret;
}
//=========================================================================//
//+++++++++++++++++++++++++стрирание всей памяти+++++++++++++++++++++++++++//
//=========================================================================//
void flash_chip_erase()
{
	  flash_write_enable();
    CS_ON;
    SPI_SendReceiveByte(F_CHIP_ERASE);
	  CS_OFF;
		while(flash_read_status_reg1() & BUSY){};
}
//=========================================================================//
//++++++++++++++++++++++++стрирание сектора++++++++++++++++++++++++++++++++//
//=========================================================================//
void erase_sector(uint32_t addr)
{
	flash_write_enable();

	CS_ON;
	SPI_SendReceiveByte(F_CHIP_ERASE_SECTOR);
  SPI_SendReceiveByte((addr >> 16) & 0xFF);
  SPI_SendReceiveByte((addr >> 8) & 0xFF);
  SPI_SendReceiveByte(addr & 0xFF);
	CS_OFF;
	while(flash_read_status_reg1()& BUSY){;}
}
//=========================================================================//
//++++++++++++++++++++++++++запись страницы++++++++++++++++++++++++++++++++//
//=========================================================================//
void flash_write_page(const uint8_t data[], uint32_t addr, uint8_t len)
{//256 bytes max per read / write
	 flash_write_enable();
	  CS_ON;
    SPI_SendReceiveByte(F_PAGE_PROGRAMM);
    SPI_SendReceiveByte((addr >> 16) & 0xFF);
    SPI_SendReceiveByte((addr >> 8) & 0xFF);
    SPI_SendReceiveByte(addr & 0xFF);
	  SPI_SendReceiveByte(*data++);
    while(len--)
    {
        SPI_SendReceiveByte(*data++);
    }
		CS_OFF;
		while(flash_read_status_reg1()& BUSY){;}

}
//=========================================================================//
//+++++++++++++++++++++++++++чтение страницы+++++++++++++++++++++++++++++++//
//=========================================================================//
 void flash_read_page(uint8_t data[], uint32_t addr, uint8_t len)//len 0 = 1 байт
{//256 bytes max per read / write.Clock frequency for Read Data instruction (03h) max 33 MHz
    CS_ON;
    SPI_SendReceiveByte(F_READ);
    SPI_SendReceiveByte((addr >> 16) & 0xFF);
    SPI_SendReceiveByte((addr >> 8) & 0xFF);
    SPI_SendReceiveByte(addr & 0xFF);
	  *data++ = SPI_SendReceiveByte(0);// send zero dummy to receive one byte
    while(len--)
    {
        *data++ = SPI_SendReceiveByte(0);
    }
    CS_OFF;
}
//=========================================================================//
//++++++++++++++++++++++++чтение айди флэш+++++++++++++++++++++++++++++++++//
//=========================================================================//
uint8_t get_flash_id()
{

    uint8_t buff[2];
    CS_ON;
    SPI_SendReceiveByte(F_GET_DEV_ID);
	  SPI_SendReceiveByte(0x00);//ПОСЫЛАЕМ 000000 24-bit address
    SPI_SendReceiveByte(0x00);//ПОСЫЛАЕМ 000000 24-bit address
    SPI_SendReceiveByte(0x00);//ПОСЫЛАЕМ 000000 24-bit address
	  buff[1] = SPI_SendReceiveByte(0);// send zero dummy to receive one byte
	  buff[0] = SPI_SendReceiveByte(0);// send zero dummy to receive one byte
    CS_OFF;
   //EFh manufaturer id for windbond,17h device ID
    if (buff[0] == 0x17  && buff[1] == 0xEF)
        return 1;//OK;

    return 0;//Problem;


}

