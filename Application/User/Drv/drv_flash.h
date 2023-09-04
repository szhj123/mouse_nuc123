#ifndef _DRV_FLASH_H
#define _DRV_FLASH_H

#include "hal_nuc123.h"

#define PAGE_SIZE                      512

#define APP1_START_ADDR                0x00000200
#define APP1_MAX_SIZE                  0x00007E00
#define APP2_START_ADDR                (APP1_START_ADDR + APP1_MAX_SIZE)
#define APP2_MAX_SIZE                  APP1_MAX_SIZE

#define FW_UPG_INFO_ADDR               (APP2_START_ADDR + APP2_MAX_SIZE)
#define FW_UPG_INFO_SIZE               0x00000200

#define MOUSE_PARA_START_ADDR          0x0001f000
#define MOUSE_PARA_SIZE                0x00000400
#define MOUSE_MACRO1_START_ADDR        (MOUSE_PARA_START_ADDR + MOUSE_PARA_SIZE)
#define MOUSE_MACRO1_SIZE              0x00000200
#define MOUSE_MACRO2_START_ADDR        (MOUSE_MACRO1_START_ADDR + MOUSE_MACRO1_SIZE)
#define MOUSE_MACRO2_SIZE              0x00000200
#define MOUSE_MACRO3_START_ADDR        (MOUSE_MACRO2_START_ADDR + MOUSE_MACRO2_SIZE)
#define MOUSE_MACRO3_SIZE              0x00000200
#define MOUSE_MACRO4_START_ADDR        (MOUSE_MACRO3_START_ADDR + MOUSE_MACRO3_SIZE)
#define MOUSE_MACRO4_SIZE              0x00000200
#define MOUSE_MACRO5_START_ADDR        (MOUSE_MACRO4_START_ADDR + MOUSE_MACRO4_SIZE)
#define MOUSE_MACRO5_SIZE              0x00000200
#define MOUSE_MACRO6_START_ADDR        (MOUSE_MACRO5_START_ADDR + MOUSE_MACRO5_SIZE)
#define MOUSE_MACRO6_SIZE              0x00000200

void Drv_Flash_Erase(uint32_t flashAddr, uint32_t flashSize );
void Drv_Flash_Read(uint32_t flashAddr, uint8_t *buf, uint16_t length );
void Drv_Flash_Write(uint32_t flashAddr, uint8_t *buf, uint16_t length );

#endif 

