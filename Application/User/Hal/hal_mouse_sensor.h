#ifndef _HAL_MOUSE_SENSOR_H
#define _HAL_MOUSE_SENSOR_H

#include "hal_nuc123.h"

#define M_SENSOR_SCL_L                 (PC->DOUT &= ~(1 << 1))
#define M_SENSOR_SCL_H                 (PC->DOUT |= (1 << 1))
#define M_SENSOR_SDA_L                 (PC->DOUT &= ~(1 << 2))
#define M_SENSOR_SDA_H                 (PC->DOUT |= (1 << 2))
#define M_SENSOR_SDA_PIN               (PC->PIN & (1 << 2))


void Hal_Sensor_Pin_Reuse(void );
void Hal_Sensor_Cs_Enable(void );
void Hal_Sensor_Cs_Disable(void );

#endif 

