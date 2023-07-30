/********************************************************
* @file       app_lcd.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_lcd.h"
#include "app_mouse_protocol.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
lcd_ctrl_block_t lcdCtrl;

void App_Lcd_Init(void )
{
    Drv_Lcd_Init();    

    App_Lcd_Set_Rw_Stat(LCD_IDLE);
    
    App_Lcd_Clr(RED);
}

void App_Lcd_Clr(uint16_t color )
{
    Drv_Lcd_Bg_Led_On();
    
    Drv_Lcd_Clear(color);
}

void App_Lcd_Set_Rw_Stat(uint8_t stat )
{
    lcdCtrl.lcdRwStat = stat;
}

uint8_t App_Lcd_Get_Rw_Stat(void )
{
    return lcdCtrl.lcdRwStat;
}

void App_Lcd_Updae_Show_Pic_ID(void )
{
    uint8_t i,j;
    uint16_t lcdShowMask = App_Mouse_Get_Pic_Show_Mask();

    j = 0;
    for(i=0;i<16;i++)
    {
        if((lcdShowMask & ((uint16_t )1 << i)) == 0)
        {
            lcdCtrl.lcdPicIdBuf[j++] = i+5;
        }
    }

    lcdCtrl.lcdPicIdBufLen = j;
    
}

