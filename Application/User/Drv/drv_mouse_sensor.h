#ifndef _DRV_MOUSE_SENSOR_H
#define _DRV_MOUSE_SENSOR_H

#include "hal_mouse_sensor.h"

#define P3325_ID                 0x43

void Drv_Sensor_P3325_Init(int16_t *xVal, int16_t *yVal );
uint8_t Drv_Sensor_Get_Product_ID(void );
void Drv_Sensor_Write(uint8_t addr, uint8_t val );
uint8_t Drv_Sensor_Single_Read(uint8_t addr );
void Drv_Sensor_Delay(uint16_t delayCnt );

#endif 

