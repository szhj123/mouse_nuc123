#ifndef _APP_LCD_H
#define _APP_LCD_H

#include "drv_lcd.h"

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


void App_Lcd_Init(void );
void App_Lcd_Clr(uint16_t color );

#endif 

