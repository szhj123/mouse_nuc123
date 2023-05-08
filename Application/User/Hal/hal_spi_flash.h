#ifndef _HAL_FLASH_H
#define _HAL_FLASH_H

#include "hal_nuc123.h"

void Hal_Spi_Flash_Init(void );
void Hal_Spi_Flash_Cs_Enable(void );
void Hal_Spi_Flash_Cs_Disable(void );
void Hal_Spi_Flash_Single_Tx(uint8_t dat );
uint8_t Hal_Spi_Flash_Single_Rx(void );

void Hal_Spi_Flash_Dma_Tx(uint8_t *buf, uint32_t length, hal_isr_callback_t callback );
void Hal_Spi_Flash_Dma_Rx(uint8_t *buf, uint32_t length, hal_isr_callback_t callback );
void Hal_Spi_Flash_Tx_Isr_Handler(void );
void Hal_Spi_Flash_Rx_Isr_Handler(void );


#endif 

