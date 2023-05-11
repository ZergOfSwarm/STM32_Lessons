#ifndef VS1053_H
#define VS1053_H
#include "stdint.h"
#include "stdbool.h"
// vs1053 SCI Registers
#define vs1053_MODE         0x00	// Mode control
#define vs1053_STATUS       0x01	// Status of vs1053b
#define vs1053_BASS         0x02	// Built-in bass enhancer
#define vs1053_CLOCKF       0x03	// Clock freq + doubler
#define vs1053_DECODE_TIME  0x04	// Decode time in seconds
#define vs1053_AUDATA       0x05	// Misc. audio data
#define vs1053_WRAM         0x06	// RAM write
#define vs1053_WRAMADDR     0x07	// Base address for RAM write
#define vs1053_HDAT0        0x08	// Stream header data 0
#define vs1053_HDAT1        0x09	// Stream header data 1
#define vs1053_AIADDR       0x0A	// Start address of application
#define vs1053_VOL          0x0B	// Volume control
#define vs1053_AICTRL0      0x0C	// Application control register 0
#define vs1053_AICTRL1      0x0D	// Application control register 1
#define vs1053_AICTRL2      0x0E	// Application control register 2
#define vs1053_AICTRL3      0x0F	// Application control register 3

// VS1011 commands
#define VS1053_CMD_READ		    0x03	// Read Opcode
#define VS1053_CMD_WRITE	    0x02	// Write Opcode

// Bit definition Mode Register
#define SM_DIFF         		0x0001	// Differential
#define SM_SETTOZERO1   		0x0002	// Set to zero
#define SM_RESET        		0x0004	// Soft Reset
#define SM_OUTOFWAV     		0x0008	// Jump out of WAV decoding
#define SM_SETTOZERO2   		0x0010	// Set to zero
#define SM_TESTS        		0x0020	// Allow SDI tests
#define SM_STREAM       		0x0040	// Stream mode
#define SM_SETTOZERO3   		0x0080	// Set to zero
#define SM_DACT         		0x0100	// DCLK active edge
#define SM_SDIORD       		0x0200	// SDI bit order
#define SM_SDISHARE     		0x0400	// Share SPI chip select
#define SM_SDINEW       		0x0800	// VS1002 native SPI modes 			-- 11 bit
#define SM_ADPCM		   		  0x1000	// 1 - PCM/ADPCM recording active   -- 12 bit
#define SM_SETTOZERO5   		0x2000	// Not using, Set to zero   				    -- 13 bit
#define SM_MIC_LINE1	   		0x4000	// MIC / LINE1 selector: 0 - MIC, 1 - Line		-- 14 bit
#define SM_CLK_RANGE	   		0x8000	// Input clock range: 0 - 12..13 MHz; 1 - 24..26 MHz  -- 15 bit

// function prototypes
void vs1053_init ( void );
void vs1053_reset ( void );
void vs1053_sinewave_beep ( uint16_t count );
bool vs1053_isDataReq ( void );
void vs1053_send32 ( uint8_t* pData );
void vs1053_setvolume ( uint8_t volumeLeft, uint8_t volumeRight );
void vs1053_writeData ( uint8_t data );

// Получить название аудиодекодера
char *vs1053_GetChipName ( void );


#endif // vs1053_H
