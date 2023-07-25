#ifndef _HAL_KEY_H
#define _HAL_KEY_H

#include "hal_nuc123.h"

#define KNOB_A                   ((PF->PIN & (1 << 2)) ? 1 : 0)
#define KNOB_B                   ((PF->PIN & (1 << 3)) ? 1 : 0)


void Hal_Key_Init(void );
uint8_t Hal_Key_Get_Gpio(port_t portx, pin_t pin );

#endif 

