/********************************************************
* @file       main.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "NUC123.h"
#include "drv_flash.h"
/* Private typedef --------------------------------------*/
typedef void (*func_t)(void );

typedef struct _fw_info_t
{
     uint8_t fwUpgFlag;
     uint16_t fwSize;
}fw_info_t;
/* Private define ---------------------------------------*/
#define FW_UPG_DISABLE                0x0
#define FW_UPG_ENABLE                 0x1
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Clk_Init(void );
static void Fw_Get_Info(uint32_t flashAddr, uint8_t *buf, uint16_t length );
static void Fw_Set_Info(uint32_t flashAddr, uint8_t *buf, uint16_t length );
static void Fw_App1_Erase(uint32_t flashAddr, uint16_t fwSize );
static void Fw_Cpy_App2_To_App1(uint32_t srcFlashAddr, uint32_t desFlashAddr, uint16_t length );
static uint16_t Fw_Cal_Checksum(uint32_t flashAddr, uint16_t fwSize );
static void Ldrom_To_Aprom(void );
/* Private variables ------------------------------------*/
static fw_info_t fwInfo;
static uint16_t wCRCin = 0x0000;
static func_t func_app = NULL;

int main(void )
{
		uint16_t app1FwChecksum = 0;
		uint16_t app2FwChecksum = 0;
	
		Clk_Init();
	
		Fw_Get_Info(FW_UPG_INFO_ADDR, (uint8_t *)&fwInfo, sizeof(fw_info_t));
		
		if(fwInfo.fwUpgFlag == FW_UPG_ENABLE)
		{
				Fw_App1_Erase(APP1_START_ADDR, fwInfo.fwSize);
			
				Fw_Cpy_App2_To_App1(APP2_START_ADDR, APP1_START_ADDR ,fwInfo.fwSize);		
			
				app1FwChecksum = Fw_Cal_Checksum(APP1_START_ADDR, fwInfo.fwSize);
			
				app2FwChecksum = Fw_Cal_Checksum(APP2_START_ADDR, fwInfo.fwSize);
			
				if(app1FwChecksum == app2FwChecksum)
				{
						 fwInfo.fwUpgFlag = FW_UPG_DISABLE;
					
						 Fw_Set_Info(FW_UPG_INFO_ADDR, (uint8_t *)&fwInfo, sizeof(fw_info_t));
					
				     Ldrom_To_Aprom();    
				}
				else
				{
					   NVIC_SystemReset();
				}
		}
		else
		{
			  Ldrom_To_Aprom();   
		}
}

void Clk_Init(void )
{
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    SYS_UnlockReg();
    /* Enable XT1_OUT(PF0) and XT1_IN(PF1) */
    SYS->GPF_MFP |= SYS_GPF_MFP_PF0_XT1_OUT | SYS_GPF_MFP_PF1_XT1_IN;

    /* Enable Internal RC 22.1184MHz clock */
    CLK->PWRCON |= CLK_PWRCON_OSC22M_EN_Msk;

    /* Waiting for Internal RC clock ready */
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_OSC22M_STB_Msk));

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLK_S_Msk)) | CLK_CLKSEL0_HCLK_S_HIRC;
    CLK->CLKDIV = (CLK->CLKDIV & (~CLK_CLKDIV_HCLK_N_Msk)) | CLK_CLKDIV_HCLK(1);

    /* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware */
    CLK->PLLCON |= CLK_PLLCON_PD_Msk;

    /* Enable external XTAL 12MHz clock */
    CLK->PWRCON |= CLK_PWRCON_XTL12M_EN_Msk | CLK_PWRCON_OSC10K_EN_Msk;

    /* Waiting for external XTAL clock ready */
    while((CLK->CLKSTATUS & (CLK_CLKSTATUS_XTL12M_STB_Msk | CLK_CLKSTATUS_OSC10K_STB_Msk)) != (CLK_CLKSTATUS_XTL12M_STB_Msk | CLK_CLKSTATUS_OSC10K_STB_Msk));

    /* Set core clock as PLL_CLOCK from PLL */
    CLK->PLLCON = 0xc28e;
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB_Msk));
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    CLK->CLKDIV = (CLK->CLKDIV & ~CLK_CLKDIV_USB_N_Msk) | (2 << CLK_CLKDIV_USB_N_Pos);
    CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLK_S_Msk)) | CLK_CLKSEL0_HCLK_S_PLL_DIV2;

    CLK->CLKSEL0 = (CLK->CLKSEL0 & ~CLK_CLKSEL0_STCLK_S_Msk) | CLK_CLKSEL0_STCLK_S_HCLK;

    /* Enable FMC ISP functions */
    FMC->ISPCON |=  FMC_ISPCON_ISPEN_Msk | FMC_ISPCON_APUEN_Msk | FMC_ISPCON_LDUEN_Msk | FMC_ISPCON_CFGUEN_Msk;

    /* Enable peripheral clock */
    CLK->AHBCLK |= CLK_AHBCLK_PDMA_EN_Msk;

    SystemCoreClockUpdate();

}

static void Fw_Get_Info(uint32_t flashAddr, uint8_t *buf, uint16_t length )
{
	  Drv_Flash_Read(flashAddr, (uint8_t *)buf, length);
}

static void Fw_Set_Info(uint32_t flashAddr, uint8_t *buf, uint16_t length )
{
	  Drv_Flash_Erase(flashAddr, FW_UPG_INFO_SIZE);
	
		Drv_Flash_Write(flashAddr, buf, length);
}

static void Fw_App1_Erase(uint32_t flashAddr, uint16_t fwSize )
{
	  Drv_Flash_Erase(flashAddr, fwSize);
}

static void Fw_Cpy_App2_To_App1(uint32_t srcFlashAddr, uint32_t desFlashAddr, uint16_t fwSize )
{
		uint32_t tmpData;
		uint16_t i;
	
		for(i=0;i<(fwSize/4 + 1);i++)
	  {
		   tmpData = FMC_Read(srcFlashAddr + i* 4);
			
			 FMC_Write(desFlashAddr+i*4, tmpData);
		}
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

static uint16_t Fw_Cal_Checksum(uint32_t flashAddr, uint16_t fwSize )
{
		uint32_t tmpData;
    uint16_t i;
	
		wCRCin = 0x0000;

    for(i=0;i<fwSize/4;i++)
    {
       tmpData = FMC_Read(flashAddr + i* 4);

       Cal_Checksum((uint8_t *)&tmpData, 4);
    }

    if((tmpData % 4) != 0)
    {
        tmpData = FMC_Read(APP2_START_ADDR + i* 4);

        Cal_Checksum((uint8_t *)&tmpData, fwSize % 4);
    }

    return wCRCin;
}

static void Ldrom_To_Aprom(void )
{
		uint32_t appAddress;
		
	  appAddress = *(__IO uint32_t*) (APP1_START_ADDR + 4);
	
		func_app = (func_t) appAddress;
	
		DrvFMC_SetVectorPage(APP1_START_ADDR);
	
		func_app();
}
