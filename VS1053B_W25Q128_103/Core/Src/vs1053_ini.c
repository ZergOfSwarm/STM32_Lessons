#include <stdbool.h>
#include "stm32f1xx.h"
#include "main.h"
#include "vs1053_ini.h"
/*
 PA6   - MISO SPI_PORT
 PA7   - SI(MOSI) SPI_PORT
 PA5   - SCK  SPI_PORT
 PA4   - XCS   - Chip select input - out+
 PA1   - xRESET -         - out+
 PA3   - XDCS  -
 PA2   - DREQ  -
*/

#define 	SPI_PORT			SPI1
//=========================XRESET - PA1 -   -  ============================//
//When the XRESET signal is driven low, VS1053b is reset and all
// the control registers and internal states are set to the initial values
//==========================DREQ   - PA2 -  ===================================//
//DREQ rises when initialization is complete. You should not send any data or commands before that.
//DREQ pin/signal is used to signal if VS1053b  2048-byte FIFO is capable of receiving data. If
//DREQ is high, VS1053b can take at least 32 bytes of SDI data or one SCI command. DREQ is turned
//low when the stream buffer is too full and for the duration of a SCI command.
//============================  XCS============================//
#define VS1053_CS()   GPIOA->ODR &= ~GPIO_ODR_ODR4;
#define VS1053_DCS()	GPIOA->ODR |= GPIO_ODR_ODR4;
//============================ XDCS==================================//
#define VS1053_SET_XDCS()   		GPIOA->ODR |= GPIO_ODR_ODR3;
#define VS1053_RESET_XDCS()		  GPIOA->ODR &= ~GPIO_ODR_ODR3;
//===========================  XRESET==================================//
#define VS1053_SET_RES()   			GPIOA->ODR |= GPIO_ODR_ODR1;
#define VS1053_RESET_RES()			GPIOA->ODR &= ~GPIO_ODR_ODR1;
//================================================================================//
#define VS1053_DREQ					   ((GPIOA->IDR)&GPIO_IDR_IDR2)
uint8_t chip_id;

 void vs1053_writeCommand ( uint8_t addr, uint16_t data );
 uint16_t vs1053_readData ( uint8_t addr );

//============================================================================//
//++++++++++++++++++++++++++++++++  SPI  +++++++++++++++++++++++++++++++++++++//
//============================================================================//
static void SpiInit ( uint32_t value )
{

   SPI1->CR1 |= value;
}
//============================================================================//
//++++++++++++++++++++++++++++++++  SPI  +++++++++++++++++++++++++++++++++++++//
//============================================================================//
static uint8_t SPI_sendByte ( uint8_t data )
{
    uint8_t rxbyte;

    while ( !( SPI_PORT -> SR & SPI_SR_TXE ) );
    SPI_PORT -> DR = data;

    while ( !( SPI_PORT -> SR & SPI_SR_RXNE ) );
    rxbyte = SPI_PORT->DR;

    return rxbyte;
}
//============================================================================//
//++++++++++++++++++++++++++  vs1053_init  +++++++++++++++++++++++++++++++++++//
//============================================================================//
void vs1053_init(void)
{
	VS1053_RESET_RES();		 // XRESET = 0
	VS1053_SET_XDCS();		 // XDCS = 1
	VS1053_DCS();			   	 // nCS = 1
	SpiInit((uint32_t)(SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0));//256
	chip_id = 0xFF;
}

//============================================================================//
//+++++++++++++++++++ Perform a vs1053 hard soft reset +++++++++++++++++++++++//
//============================================================================//
void vs1053_reset(void)
{
	uint16_t temp;
	SpiInit((uint32_t)(SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0));//НАСТРОЙКУ ПРОИЗВОДИТЬ НА НИЗКОЙ!!!!!
	VS1053_RESET_RES();		// XRESET = 0 Harware reset VS1053b is reset and all the control registers and
	HAL_Delay (5);
	VS1053_SET_RES();			// XRESET = 1 //internal states are set to the initial values
	HAL_Delay (5); //500

	while(!VS1053_DREQ){};

    temp = vs1053_readData(vs1053_STATUS);	// read status, must be 0x000C
    temp = vs1053_readData(vs1053_MODE);	// read status, must be 0x4000
  	// Soft reset 16
  	vs1053_writeCommand(vs1053_MODE,SM_RESET|SM_SDINEW);//| SM_CLK_RANGE for 24MHz-26
		//SM_RESET-soft Reset
		//SM_SDINEW- by default at startup native SPI modes
  	HAL_Delay(500);
    do
    {
    	temp=vs1053_readData(vs1053_MODE);
    } while((temp&SM_RESET)!=0);
    vs1053_writeCommand(vs1053_MODE, SM_SDINEW);// | SM_CLK_RANGE 24MHz только для 24MHz

    temp = vs1053_readData(vs1053_STATUS);
    chip_id=(temp >> 4)&0x0F;// 0 - VS1001, 1 - VS1011, 2 - VS1002, 3 - VS1003, 4 - VS1053
		// CLOCK_F: - p.43
	  //15..13 - 3 bits  - SC_MULT - Clock multplier\ SC_MULT = x3.5 - 0x8000 \ Recommended (4.2, p. 7)
    //12..11 - 2 bits  - SC_ADD - Allowed multiplier addition\SC_ADD  = x1.0 - 0x0800 // Recommended (4.2, p. 7)
    //10..00 - 11 bits - SC_FREQ - Clock frequency \ SC_FREQ = 0x0430 (12.288 MHz)
	  //Для 12.288MHz:
	  //(XTLI_HZ - 8000000) / 4000 = (12288000 - 8000000) / 4000 = 4288000 / 4000 = 1072 = 0x0430 = 0000 0100 0011 0000
		/*The maximum sample rate that can be played with correct speed is XTALI/256 (or XTALI/512
    if SM_CLK_RANGE is set). Thus, XTALI must be at least 12.288 MHz (24.576 MHz) to be able
    to play 48 kHz at correct speed. Reset value is 1.0 × . Recommended SC_MULT=3.5 × , SC_ADD=1.0 × (SCI_CLOCKF=0x8800).*/
    vs1053_writeCommand (vs1053_CLOCKF, 0x8000 | 0x0800 | 0x0430 );// 0x0430 можно было не указывать,ноль в SC_FREQ означает 12,288!!!
	  vs1053_setvolume ( 50, 50 );// Начальная громкость чем меньше тем громче))
  	SpiInit (SPI_CR1_BR_2);//музыку гоним на высокой скрости
}

//============================================================================//
//++++++++++++++++++++++ Play sine wave  on vs1053 +++++++++++++++++++++++++++//
//============================================================================//
void vs1053_sinewave_beep (uint16_t time_ms)
{
    uint8_t sine_on[8]={ 0x53, 0xEF, 0x6E, 0x44, 0x00, 0x00, 0x00, 0x00 };
    uint8_t sine_off[8]={0x45, 0x78, 0x69, 0x74, 0x00, 0x00, 0x00, 0x00 };
    uint8_t i;

	  VS1053_RESET_RES();		// XRESET = 0
	  HAL_Delay(1);
	  VS1053_SET_RES ( );		// XRESET = 1
	  HAL_Delay(500);

	  // Test sine - p. 67
    vs1053_writeCommand(vs1053_MODE,SM_SDINEW|SM_TESTS);//| SM_CLK_RANGE 24MHz
        while(!VS1053_DREQ){};
        for(i=0;i<8;i++)
            vs1053_writeData(sine_on[i]);
        HAL_Delay (time_ms);

        for(i=0;i<8;i++)
           vs1053_writeData(sine_off[i]);
			  HAL_Delay(time_ms);
}

//============================================================================//
//+++++++++++++++++++++++++++++ VS1053_DREQ STATE ++++++++++++++++++++++++++++//
//============================================================================//
bool vs1053_isDataReq(void)
{
  if (VS1053_DREQ == RESET )
    return false;

  return true;
}
//============================================================================//
//+++++++++++++++++ Send an 32 byte block of audio stream data +++++++++++++++//
//============================================================================//
void vs1053_send32(uint8_t *pData)
{
	uint8_t i;
	VS1053_RESET_XDCS();
	for (i=0;i <32;i++)// send 32 bytes
		SPI_sendByte(*pData ++ );
	VS1053_SET_XDCS();
}
//============================================================================//
//++++++++++++++++++++++++++++ Set audio volume ++++++++++++++++++++++++++++++//
//============================================================================//
void vs1053_setvolume(uint8_t volumeLeft,uint8_t volumeRight)
{
	uint16_t vol;
	vol = volumeLeft;
	vol <<= 8;
	vol |= volumeRight;
 	SpiInit((uint32_t)(SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0));
	vs1053_writeCommand (vs1053_VOL,vol);			// set vs1053 volume register
 	SpiInit(SPI_CR1_BR_0);
}
//============================================================================//
//+++++++++++++++++++++ Write a data word to specific address ++++++++++++++++//
//============================================================================//
void vs1053_writeCommand(uint8_t addr,uint16_t data)
{
    VS1053_CS();
    SPI_sendByte(VS1053_CMD_WRITE );// send write opcode 0x02
    SPI_sendByte(addr);			    // send address
    SPI_sendByte(data >> 8);		// send MSB
    SPI_sendByte(data);			    //send LSB
    VS1053_DCS();
}

//============================================================================//
//++++++++++++++++++++ Send one byte of audio stream data ++++++++++++++++++++//
//============================================================================//
void vs1053_writeData ( uint8_t data )
{
	VS1053_RESET_XDCS();
	SPI_sendByte(data);
	VS1053_SET_XDCS();
}


//============================================================================//
//+++++++++++++++ Read a data word from vs1053 specific address ++++++++++++++//
//============================================================================//
uint16_t vs1053_readData(uint8_t addr)
{
    uint16_t tempdata = 0;
    uint8_t temp;
	  VS1053_SET_XDCS();              // XDCS = 0
    VS1053_CS();			              // xCS = 0
    SPI_sendByte(VS1053_CMD_READ );	// send read opcode 0x03
    SPI_sendByte(addr);				      // send address
    temp = SPI_sendByte(0);		      // send zero dummy to receive one byte
    tempdata = temp;
    tempdata <<= 8;						      // MSB was sent first
    temp = SPI_sendByte(0);		      // send zero dummy to receive one byte
    tempdata |= temp;
    VS1053_DCS();			              // xCS = 1
    return tempdata;	             	// return data word
}
