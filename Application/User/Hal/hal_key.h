#ifndef _HAL_KEY_H
#define _HAL_KEY_H

#include "hal_nuc123.h"

void Hal_Key_Init(void );
uint8_t Hal_Key_Get_Gpio(port_t portx, pin_t pin );

#endif 

