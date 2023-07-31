#ifndef _APP_LCD_H
#define _APP_LCD_H

#include "drv_task.h"
#include "drv_spi_flash.h"
#include "drv_lcd.h"

#define LCD_PIC_MAX_SIZE                       0x20000 //128k


#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F 
#define BRED             0xF81F
#define GRED 			 0xFFE0
#define GBLUE			 0x07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0xBC40
#define BRRED 			 0xFC07 
#define GRAY  			 0x8430

#define LCD_IDLE         0
#define LCD_BUSY         (!LCD_IDLE)

#define LCD_SHOW_DISABEL 0
#define LCD_SHOW_ENABLE  (!LCD_SHOW_DISABEL)

typedef enum _lcd_flash_erase_stat
{
    LCD_FLASH_ERASE_EN = 0,
    LCD_FLASH_ERASE_DONE
}lcd_flash_erase_stat;

typedef enum _pic_stat_t
{
    PIC_STAT_INIT = 0,
    PIC_STAT_WRITE_DATA,
    PIC_STAT_IDLE
}pic_stat_t;

typedef struct _lcd_ctrl_block_t
{
    uint8_t picIdBuf[16];
    uint8_t picTotalNum;
    uint8_t lcdRwStat;
    pic_stat_t picStat;
    uint8_t picUpdateEn;
    uint8_t picIndex;
    uint32_t picFlashAddr;
    uint32_t picOffset;
    uint16_t delayCnt;
}lcd_ctrl_block_t;


void App_Lcd_Init(void );
void App_Lcd_Clr(uint16_t color );

void App_Lcd_Set_Rw_Stat(uint8_t stat );
uint8_t App_Lcd_Get_Rw_Stat(void );

void App_Lcd_Updae_Show_Pic_ID(void );

void App_Lcd_Pic_Show_Disable(void );

void App_Lcd_Show_Pic(void );

#endif 

