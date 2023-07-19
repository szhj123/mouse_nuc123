#ifndef _HAL_CALENDAR_H
#define _HAL_CALENDAR_H

#include "hal_nuc123.h"

void Hal_Calendar_Init(hal_isr_callback_t callback );
void Hal_Calendar_Isr_Handler(void );

#endif 

