#ifndef _HAL_LIGHT_H
#define _HAL_LIGHT_H

#include "hal_nuc123.h"

void Hal_Light_Init(void );
void Hal_Light_All_Off(void );
void Hal_Light_All_On(void );
void Hal_Light_On(port_t portx, uint8_t pin );
void Hal_Light_Off(port_t portx, uint8_t pin );
void Hal_Light_Pwm_Duty_Set(uint8_t rVal, uint8_t gVal, uint8_t bVal );

#endif 

