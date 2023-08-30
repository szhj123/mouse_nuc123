/********************************************************
* @file       app_flash.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_flash.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
static user_para_t userPara;

void App_Flash_Set_Fw_Size(uint16_t fwSize )
{
    userPara.fwSize = fwSize;
}

void App_Flash_Fw_Erase(uint16_t fwSize )
{
    Drv_Flash_Erase(APP2_START_ADDR, fwSize);
}

void App_FlasH_Write_Fw_Data(uint16_t offset, uint8_t *buf, uint16_t length )
{
    Drv_Flash_Write(APP2_START_ADDR+offset, buf, length);
}

