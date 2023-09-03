/********************************************************
* @file       drv_flash.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_flash.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Drv_Flash_Erase(uint32_t flashAddr, uint32_t flashSize )
{
    uint32_t i;
    uint32_t pageCnt;

    if((flashSize % PAGE_SIZE) != 0)
    {
        pageCnt = flashSize/PAGE_SIZE + 1;
    }
    else
    {
        pageCnt = flashSize/PAGE_SIZE;
    }

    for(i=0;i<pageCnt;i++)
    {
        FMC_Erase(flashAddr);

        flashAddr += PAGE_SIZE;
    }
}

void Drv_Flash_Read(uint32_t flashAddr, uint8_t *buf, uint16_t length )
{
    u32Data_t tmpData;
    uint16_t i;

    for(i=0;i<length/4;i++)
    {
       tmpData.u32Data = FMC_Read(flashAddr + i* 4);

       buf[i*4+0] = tmpData.byte_t.byte0;
       buf[i*4+1] = tmpData.byte_t.byte1;
       buf[i*4+2] = tmpData.byte_t.byte2;
       buf[i*4+3] = tmpData.byte_t.byte3;
    }

    if((length % 4) != 0)
    {
        tmpData.u32Data = FMC_Read(flashAddr + i* 4);
        
        switch(length % 4)
        {
            case 1: buf[i*4+0] = tmpData.byte_t.byte0; 
                    break;
            case 2: buf[i*4+0] = tmpData.byte_t.byte0;
                    buf[i*4+1] = tmpData.byte_t.byte1;
                    break;
            case 3: buf[i*4+0] = tmpData.byte_t.byte0;
                    buf[i*4+1] = tmpData.byte_t.byte1;
                    buf[i*4+2] = tmpData.byte_t.byte2;
                    break;
            default: break;
        }
    }
}

void Drv_Flash_Write(uint32_t flashAddr, uint8_t *buf, uint16_t length )
{
    u32Data_t tmpData;
    uint16_t i;

    for(i=0;i<length/4;i++)
    {
        tmpData.byte_t.byte0 = buf[i*4+0];
        tmpData.byte_t.byte1 = buf[i*4+1];
        tmpData.byte_t.byte2 = buf[i*4+2];
        tmpData.byte_t.byte3 = buf[i*4+3];

        FMC_Write(flashAddr+i*4, tmpData.u32Data);
    }

    if((length % 4) != 0)
    {
        tmpData.u32Data = FMC_Read(flashAddr + i* 4);

        switch(length%4)
        {
            case 1: tmpData.byte_t.byte0 = buf[i*4+0]; 
                    break;
            case 2: tmpData.byte_t.byte0 = buf[i*4+0];
                    tmpData.byte_t.byte1 = buf[i*4+1];
                    break;
            case 3: tmpData.byte_t.byte0 = buf[i*4+0];
                    tmpData.byte_t.byte1 = buf[i*4+1];
                    tmpData.byte_t.byte2 = buf[i*4+2];
                    break;
            default: break;
        }

        FMC_Write(flashAddr+i*4, tmpData.u32Data);
    }
}

#define E_DRVFMC_ERR_ISP_FAIL   0xFFFF0010

int32_t DrvFMC_SetVectorPage(uint32_t u32addr)
{
	  FMC->ISPCMD = FMC_ISPCMD_VECMAP;
		FMC->ISPADR = u32addr;
    FMC->ISPTRG |= (1ul << FMC_ISPTRG_ISPGO_Pos);
    __ISB();
    while (FMC->ISPTRG);

    if (FMC->ISPCON & FMC_ISPCON_ISPFF_Msk)
    {
        FMC->ISPCON |= 1ul << FMC_ISPCON_ISPFF_Pos;
        return E_DRVFMC_ERR_ISP_FAIL;
    }
	
    return 0;
}
