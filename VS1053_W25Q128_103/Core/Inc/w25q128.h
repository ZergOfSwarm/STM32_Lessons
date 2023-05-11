#ifndef W25Q128_H
#define W25Q128_H
#include "stdint.h"

#define    BUSY                  0x01
#define    WEL                   0b00000010 //write enable
#define    F_WRITE_ENABLE        0x06
#define    F_WRITE_DISABLE       0x04
#define    F_READ_STATUS_REG1    0x05
#define    F_CHIP_ERASE          0xC7 //0x60
#define    F_CHIP_ERASE_SECTOR   0xD8
#define    F_GET_DEV_ID          0x90
#define    F_ENABLE_RESET        0x66
#define    F_RESET_DEVICE        0x99
#define    F_PAGE_PROGRAMM       0x02
#define    F_READ                0x03
void w25q128_spi2_ini(void);
void flash_reset(void);
void erase_sector(uint32_t addr);
void flash_chip_erase(void);
void flash_write_enable(void);
void flash_write_page(const uint8_t data[], uint32_t addr, uint8_t len);
void flash_read_page(uint8_t data[], uint32_t addr, uint8_t len);
uint8_t get_flash_id(void);
#endif
