#ifndef _APP_LCD_H
#define _APP_LCD_H

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

typedef enum _lcd_flash_erase_stat
{
    LCD_FLASH_ERASE_EN = 0,
    LCD_FLASH_ERASE_DONE
}lcd_flash_erase_stat;

typedef struct _lcd_ctrl_block_t
{
    uint8_t lcdPicIdBuf[16];
    uint8_t lcdPicIdBufLen;
    uint8_t lcdRwStat;
}lcd_ctrl_block_t;


void App_Lcd_Init(void );
void App_Lcd_Clr(uint16_t color );
void App_Lcd_Set_Rw_Stat(uint8_t stat );
uint8_t App_Lcd_Get_Rw_Stat(void );
void App_Lcd_Updae_Show_Pic_ID(void );

#endif 

