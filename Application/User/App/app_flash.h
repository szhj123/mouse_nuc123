#ifndef _APP_FLASH_H
#define _APP_FLASH_H

#include "drv_flash.h"

#define FW_VER_BUILD                  0
#define FW_VER_MINOR                  0
#define FW_VER_MAJOR                  1


#define FW_UPG_DISABLE                0x0
#define FW_UPG_ENABLE                 0x1

typedef struct _fw_info_t
{
     uint8_t fwUpgFlag;
     uint16_t fwSize;
}fw_info_t;

void App_Flash_Set_Fw_Size(uint16_t fwSize );
uint16_t App_Flash_Get_Fw_Size(void );
void App_Flash_Fw_Erase(uint16_t fwSize );
void App_Flash_Write_Fw_Data(uint16_t offset, uint8_t *buf, uint16_t length );
uint16_t App_Flash_Get_Fw_Checksum(uint16_t length );
void App_Flash_Fw_Upg_Enable(void );
void App_Flash_Fw_Info_Save(void );
void Aprom_To_Ldrom(void );

#endif 

