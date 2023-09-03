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
static uint16_t wCRCin = 0x0000;

static fw_info_t fwInfo;

void App_Flash_Set_Fw_Size(uint16_t fwSize )
{
    fwInfo.fwSize = fwSize;
}

uint16_t App_Flash_Get_Fw_Size(void )
{
    return fwInfo.fwSize;
}

void App_Flash_Fw_Upg_Enable(void )
{
    fwInfo.fwUpgFlag = FW_UPG_ENABLE;
}

void App_Flash_Fw_Info_Save(void )
{
    Drv_Flash_Erase(FW_UPG_INFO_ADDR, FW_UPG_INFO_SIZE);

    Drv_Flash_Write(FW_UPG_INFO_ADDR, (uint8_t *)&fwInfo, sizeof(fw_info_t));
}

void App_Flash_Fw_Erase(uint16_t fwSize )
{
    Drv_Flash_Erase(APP2_START_ADDR, fwSize);
}

void App_Flash_Write_Fw_Data(uint16_t offset, uint8_t *buf, uint16_t length )
{
    Drv_Flash_Write(APP2_START_ADDR+offset, buf, length);
}

void InvertUint16(uint16_t *poly )
{
    uint8_t i;
    uint16_t tmp = 0;
    uint16_t polyVal = *poly;

    for(i=0;i<16;i++)
    {
        if(polyVal & (1 << i))
            tmp |= 1 << (15-i);
    }
    *poly = tmp;
}

void Cal_Checksum(uint8_t *data, uint32_t length)
{
    uint8_t i;
    uint16_t wCPoly = 0x8005;

    InvertUint16(&wCPoly);

    while(length--)
    {
        wCRCin ^= *data++;
        for(i=0;i<8;i++)
        {

            if(wCRCin & 0x0001)
                wCRCin = (wCRCin >> 1) ^ wCPoly;
            else
                wCRCin >>= 1;
        }
    }
}


uint16_t App_Flash_Get_Fw_Checksum(uint16_t length )
{
    u32Data_t tmpData;
    uint16_t i;

    for(i=0;i<length/4;i++)
    {
       tmpData.u32Data = FMC_Read(APP2_START_ADDR + i* 4);

       Cal_Checksum((uint8_t *)&tmpData.u32Data, 4);
    }

    if((length % 4) != 0)
    {
        tmpData.u32Data = FMC_Read(APP2_START_ADDR + i* 4);

        Cal_Checksum((uint8_t *)&tmpData.u32Data, length % 4);
    }

    return wCRCin;
}

void Aprom_To_Ldrom(void )
{
    /* Mask all interrupt before changing VECMAP to avoid wrong interrupt handler fetched */
    __set_PRIMASK(1);

    FMC_SetBootSource(1);

    /* Set VECMAP to LDROM for booting from LDROM */
    FMC_SetVectorPageAddr(FMC_LDROM_BASE);

    /* Software reset to boot to LDROM */
    NVIC_SystemReset();
}

