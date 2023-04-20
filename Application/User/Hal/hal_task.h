#ifndef _HAL_KERNEL_H
#define _HAL_KERNEL_H

#include "hal_nuc123.h"

void Hal_Task_Regist_Isr_Callback(hal_isr_callback_t callback );
void Hal_Task_Isr_Handler(void );


#endif 

