#ifndef _DRV_LCD_H
#define _DRV_LCD_H

#include "hal_lcd.h"

#define LCD_W                 240
#define LCD_H                 240

void Drv_Lcd_Init(void );
void Drv_Lcd_Write_Pic_Data(uint8_t *buf, uint16_t len );

void Drv_Lcd_Set_Address(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 );


void Drv_Lcd_Wr_Cmd(uint8_t cmd );
void Drv_Lcd_Wr_Data(uint8_t dat );
void Drv_Lcd_Delay_Us(uint32_t us );

void Drv_Lcd_Bg_Led_On(void );
void Drv_Lcd_Bg_Led_Off(void );

#endif 

