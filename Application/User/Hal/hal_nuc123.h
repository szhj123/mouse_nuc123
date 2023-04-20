#ifndef _HAL_NUC123_H
#define _HAL_NUC123_H

#include <stdint.h>
#include "NUC123.h"
#include "clk.h"
#include "gpio.h"
#include "timer.h"

typedef void (*hal_isr_callback_t)(void );

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

