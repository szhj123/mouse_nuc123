#ifndef _DRV_FLASH_H
#define _DRV_FLASH_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "NUC123.h"

typedef union _u32Data_t
{
    uint32_t u32Data;
    struct
    {
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
    }byte_t;
}u32Data_t;

#define PAGE_SIZE                      512

#define BLD_FIRST_PAGE_ADDR            0
#define BLD_FIRST_PAGE_SIZE            0x00000200
#define APP1_START_ADDR                (BLD_FIRST_PAGE_ADDR + BLD_FIRST_PAGE_SIZE)
#define APP1_MAX_SIZE                  0x00007E00
#define APP2_START_ADDR                (APP1_START_ADDR + APP1_MAX_SIZE)
#define APP2_MAX_SIZE                  0x00007E00

#define FW_UPG_INFO_ADDR               (APP2_START_ADDR + APP2_MAX_SIZE)
#define FW_UPG_INFO_SIZE               0x00000200


void Drv_Flash_Erase(uint32_t flashAddr, uint32_t flashSize );
void Drv_Flash_Read(uint32_t flashAddr, uint8_t *buf, uint16_t length );
void Drv_Flash_Write(uint32_t flashAddr, uint8_t *buf, uint16_t length );
int32_t DrvFMC_SetVectorPage(uint32_t u32addr);

#endif 

