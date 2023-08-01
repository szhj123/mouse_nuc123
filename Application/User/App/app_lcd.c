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
#define LCD_COLOR_BUF_MAX_SIZE                 9600
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

    App_Lcd_Set_RW_Flash_Stat(LCD_FLASH_IDLE);

    lcdCtrl.picIndex = App_Mouse_Get_Pic_Index();

    if(lcdCtrl.picIndex < 5 || lcdCtrl.picIndex > 16)
    {
        lcdCtrl.picIndex = 5;
    }
    
    App_Lcd_Show_Pic(lcdCtrl.picIndex);

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

void App_Lcd_Set_RW_Flash_Stat(uint8_t stat )
{
    lcdCtrl.lcdRwFlashStat = stat;
}

uint8_t App_Lcd_Get_RW_Flash_Stat(void )
{
    return lcdCtrl.lcdRwFlashStat;
}

void App_Lcd_Pic_Show_Disable(void )
{
    lcdCtrl.picUpdateEn = 0;
}

void App_Lcd_Show_Pic(uint8_t picIndex )
{
    lcdCtrl.picIndex = picIndex;

    lcdCtrl.picFlashAddr = (picIndex - 1) * LCD_PIC_MAX_SIZE;

    lcdCtrl.picOffset = 0;
    
    lcdCtrl.picUpdateEn = 1;

    lcdCtrl.picStat = PIC_STAT_INIT;

    lcd_show_callback = App_Lcd_Show_Pic_Callback;
}

pic_stat_t App_Lcd_Get_Pic_Show_Stat(void )
{
    return lcdCtrl.picStat;
}

void App_Lcd_Set_Pic_Index(uint8_t picIndex )
{
    lcdCtrl.picIndex = picIndex;
}

uint8_t App_Lcd_Get_Pic_Index(void )
{
    return lcdCtrl.picIndex;
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
            Drv_Lcd_Wr_Cmd(0x36);
            Drv_Lcd_Wr_Data(0x00);   //ACCESS
            Drv_Lcd_Wr_Cmd(0x28);
            Drv_Lcd_Set_Address(0, 0, 239, 239);
            lcdCtrl.picStat = PIC_STAT_READ_DATA;
            break;
        }
        case PIC_STAT_READ_DATA:
        {
            App_Lcd_Set_RW_Flash_Stat(LCD_FLASH_BUSY);

            Drv_Spi_Flash_Read(lcdCtrl.picFlashAddr+lcdCtrl.picOffset, lcdColorBuf, LCD_COLOR_BUF_MAX_SIZE);

            lcdCtrl.picStat = PIC_STAT_READ_WAIT;
            break;
        }
        case PIC_STAT_READ_WAIT:
        {
            if(Drv_Spi_Flash_Get_Rx_Flag())
            {
                Drv_Spi_Flash_Clr_Rx_Flag();
                
                App_Lcd_Set_RW_Flash_Stat(LCD_FLASH_IDLE);

                lcdCtrl.picStat = PIC_STAT_WRITE_DATA;
            }           
            break;
        }
        case PIC_STAT_WRITE_DATA:
        {
            Drv_Lcd_Write_Pic_Data(lcdColorBuf, LCD_COLOR_BUF_MAX_SIZE);

            lcdCtrl.picStat = PIC_STAT_WRITE_WAIT;
            
            break;
        }
        case PIC_STAT_WRITE_WAIT:
        {
            if(Drv_Lcd_Spi_Get_Tx_Done_Flag())
            {
                Drv_Lcd_Spi_Clr_Tx_Done_Flag();

                lcdCtrl.picOffset += LCD_COLOR_BUF_MAX_SIZE;

                if(lcdCtrl.picOffset >= (LCD_W * LCD_H * 2))
                {                    
                    Drv_Lcd_Wr_Cmd(0x29);
                    
                    lcdCtrl.picOffset = 0 ;

                    if(App_Mouse_Get_Pic_Auto_Switch_Time() != 0)
                    {
                        lcdCtrl.picIndex++;

                        if(lcdCtrl.picIndex > LCD_PIC_MAX_NUM)
                        {
                            lcdCtrl.picIndex = 5;
                        }
                    }
                    
                    lcdCtrl.picStat = PIC_STAT_IDLE;
                }
                else
                {
                    lcdCtrl.picStat = PIC_STAT_READ_DATA;
                }
            }
            break;
        }
        case PIC_STAT_IDLE:
        {
            if(App_Mouse_Get_Pic_Auto_Switch_Time() != 0)
            {
                if((App_Mouse_Get_Pic_Show_Mask() & (1 << (lcdCtrl.picIndex - 5))) == 0)
                {
                    if(++lcdCtrl.delayCnt >= App_Mouse_Get_Pic_Auto_Switch_Time())
                    {
                        lcdCtrl.delayCnt = 0;
                        
                        App_Lcd_Show_Pic(lcdCtrl.picIndex);

                        App_Mouse_Set_Pic_Index(lcdCtrl.picIndex);
                    }
                }
                else
                {
                    lcdCtrl.picIndex++;

                    if(lcdCtrl.picIndex > LCD_PIC_MAX_NUM)
                    {
                        lcdCtrl.picIndex = 5;
                    }
                }
            }
            break;
        }
        default: break;
    }
}

