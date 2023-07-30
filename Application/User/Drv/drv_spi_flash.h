#ifndef _DRV_SPI_FLASH_H
#define _DRV_SPI_FLASH_H

#include "hal_spi_flash.h"

#define FLASH_WRITE_ENABLE              0x06
#define FLASH_READ                      0x03
#define FLASH_PAGE_PROGRAM              0x02
#define FLASH_STATUS_READ               0x05
#define FLASH_STATUS_WRITE              0x01
#define FLASH_CHIP_ERASE                0xc7
#define FLASH_SECTOR_ERASE              0x20
#define FLASH_BLOCK_32K_ERASE           0x52
#define FLASH_BLOCK_64K_ERASE           0xd8

#define SPI_FLASH_PAGE_SIZE             256     

void Drv_Spi_Flash_Init(void );
//void Drv_Spi_Flash_Read_Id(void );
void Drv_Spi_Flash_Wait_Idle(void );
void Drv_Spi_Flash_Write_Enable(void );
void Drv_Spi_Flash_Erase_64k(uint32_t addr );
void Drv_Spi_Flash_Loop_Write(uint32_t addr, uint8_t *buf, uint32_t length );
void Drv_Spi_Flash_Dma_Write(uint32_t addr, uint8_t *buf, uint32_t length );
void Drv_Spi_Flash_Write(uint32_t addr, uint8_t *buf, uint32_t length );
void Drv_Spi_Flash_Read(uint32_t addr, uint8_t *buf, uint32_t length );


#endif 

