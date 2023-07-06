#ifndef _HAL_NUC123_H
#define _HAL_NUC123_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "NUC123.h"

typedef void (*hal_isr_callback_t)(void );

typedef union _u32Data_t
{
    uint32_t u32Data;
    struct
    {
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
    }byte_t;
}u32Data_t;

typedef union _u8Data_t
{
    uint8_t u8Data;
    struct 
    {
        uint8_t bit0:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
    }bit_t;
}u8Data_t;

void Clk_Init(void );
void Systick_Init(void );
void Gpio_Init(void );
void Gpio_Intp_Init(void );
void Timer0_Init(void );
void Pwm_Init(void );
void Spi0_Init(void );
void Spi2_Init(void );

void Hardware_Init(void );

#endif 

