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
typedef void (*lcd_show_callbac_t)(void );
/* Private define ---------------------------------------*/
#define LCD_COLOR_BUF_MAX_SIZE                 4800
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Lcd_Handler(void *arg );
static void App_Lcd_Show_Pic_Callback(void );
/* Private variables ------------------------------------*/
static lcd_ctrl_block_t lcdCtrl;
static lcd_show_callbac_t lcd_show_callback = NULL;
static uint8_t lcdColorBuf[LCD_COLOR_BUF_MAX_SIZE];

void App_Lcd_Init(void )
{
    Drv_Lcd_Init();    

    App_Lcd_Set_Rw_Stat(LCD_IDLE);
    
    App_Lcd_Clr(BLUE);

    App_Lcd_Show_Pic();

    Drv_Task_Regist_Period(App_Lcd_Handler, 0, 1, NULL);
}

static void App_Lcd_Handler(void *arg )
{
    if(lcd_show_callback != NULL)
    {
        lcd_show_callback();
    }
}

void App_Lcd_Clr(uint16_t color )
{
    uint32_t i;
    uint32_t totalLength = LCD_W * LCD_H * 2;
    
    Drv_Lcd_Bg_Led_On();

    Drv_Lcd_Set_Address(0, 0, 239, 239);

    while(totalLength)
    {
        for(i=0;i<LCD_COLOR_BUF_MAX_SIZE/2;i++)
        {
            lcdColorBuf[i*2] = (uint8_t )(color>>8);
            lcdColorBuf[i*2+1] = (uint8_t )color;
        }

        Drv_Lcd_Write_Pic_Data(lcdColorBuf, LCD_COLOR_BUF_MAX_SIZE);
       
        totalLength -= LCD_COLOR_BUF_MAX_SIZE;
    }
}

void App_Lcd_Set_Rw_Stat(uint8_t stat )
{
    lcdCtrl.lcdRwStat = stat;
}

uint8_t App_Lcd_Get_Rw_Stat(void )
{
    return lcdCtrl.lcdRwStat;
}

void App_Lcd_Pic_Show_Disable(void )
{
    lcdCtrl.picUpdateEn = 0;
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
            lcdCtrl.picIdBuf[j++] = i+5;
        }
    }

    lcdCtrl.picTotalNum = j;
}

void App_Lcd_Show_Pic(void )
{
    lcdCtrl.picIndex = 0;

    lcdCtrl.picStat = PIC_STAT_INIT;
    
    lcdCtrl.picUpdateEn = 1;

    lcd_show_callback = App_Lcd_Show_Pic_Callback;
}

static void App_Lcd_Show_Pic_Callback(void )
{
    if(!lcdCtrl.picUpdateEn)
    {
        return ;
    }
    
    switch(lcdCtrl.picStat)
    {
        case PIC_STAT_INIT:
        {
            lcdCtrl.picFlashAddr = (lcdCtrl.picIdBuf[lcdCtrl.picIndex] - 1) * LCD_PIC_MAX_SIZE;
            lcdCtrl.picOffset = 0;
            Drv_Lcd_Wr_Cmd(0x36);
            Drv_Lcd_Wr_Data(0x00);   //ACCESS
            Drv_Lcd_Wr_Cmd(0x29);
            Drv_Lcd_Set_Address(0, 0, 239, 239);
            lcdCtrl.picStat = PIC_STAT_WRITE_DATA;
            break;
        }
        case PIC_STAT_WRITE_DATA:
        {
            App_Lcd_Set_Rw_Stat(LCD_BUSY);
            
            Drv_Spi_Flash_Read(lcdCtrl.picFlashAddr+lcdCtrl.picOffset, lcdColorBuf, LCD_COLOR_BUF_MAX_SIZE);
            
            App_Lcd_Set_Rw_Stat(LCD_IDLE);
            
            Drv_Lcd_Write_Pic_Data(lcdColorBuf, LCD_COLOR_BUF_MAX_SIZE);
            
            lcdCtrl.picOffset += LCD_COLOR_BUF_MAX_SIZE;
            
            if(lcdCtrl.picOffset >= (LCD_W * LCD_H * 2))
            {
                Drv_Lcd_Wr_Cmd(0x29);
                
                lcdCtrl.picStat = PIC_STAT_IDLE;
            }
            break;
        }
        case PIC_STAT_IDLE:
        {
            if(lcdCtrl.picTotalNum > 1)
            {
                if(++lcdCtrl.delayCnt >= 5000)
                {
                    lcdCtrl.delayCnt = 0;

                    lcdCtrl.picIndex++;

                    if(lcdCtrl.picIndex >= lcdCtrl.picTotalNum)
                    {
                        lcdCtrl.picIndex = 0;
                    }

                    lcdCtrl.picStat = PIC_STAT_IDLE;
                }
            }
            break;
        }
        default: break;
    }
}

