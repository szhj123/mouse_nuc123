#ifndef _APP_FLASH_H
#define _APP_FLASH_H

#include "drv_flash.h"

typedef struct _user_para_t
{
     uint8_t fwUpgEn;
     uint16_t fwSize;
}user_para_t;

void App_Flash_Set_Fw_Size(uint16_t fwSize );
void App_Flash_Fw_Erase(uint16_t fwSize );
void App_FlasH_Write_Fw_Data(uint16_t offset, uint8_t *buf, uint16_t length );

#endif 

