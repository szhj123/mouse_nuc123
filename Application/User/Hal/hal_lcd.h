#ifndef _HAL_LCD_H
#define _HAL_LCD_H

#include "hal_nuc123.h"

#define LCD_BG_LED_ON                            PC->DOUT |= (1 << 5)
#define LCD_BG_LED_OFF                           PC->DOUT &= ~(1 << 5)

#define LCD_DC_CLR                               PD->DOUT &= ~(1 << 5)
#define LCD_DC_SET                               PD->DOUT |= (1 << 5)

#define LCD_RST_CLR                              PD->DOUT &= ~(1 << 4)
#define LCD_RST_SET                              PD->DOUT |= (1 << 4)


void Hal_Lcd_Init(void );
void Hal_Lcd_Spi_Cs_Enable(void );
void Hal_Lcd_Spi_Cs_Disable(void );
void Hal_Lcd_Spi_Single_Tx(uint8_t dat );
void Hal_Lcd_Spi_Dma_Tx(const uint8_t *buf, uint16_t length, hal_isr_callback_t callback );
void Hal_Lcd_Spi_Isr_Handler(void );

#endif 

