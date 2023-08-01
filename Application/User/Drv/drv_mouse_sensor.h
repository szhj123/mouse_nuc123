#ifndef _DRV_MOUSE_SENSOR_H
#define _DRV_MOUSE_SENSOR_H

#include "hal_mouse_sensor.h"

#define P3325_ID                   0x43
#define P3325_PRODUCT_ID           0x00								  //0x43
#define P3325_MOTION               0x02								  //0x00
#define P3325_DELTA_XL             0x03								  //0x00
#define P3325_DELTA_XH             0x04								  //0x00
#define P3325_DELTA_YL             0x05								  //0x00
#define P3325_DELTA_YH             0x06
#define P3325_SQUAL                0x07								  //Any
#define P3325_PIXEL_SUM            0x08								  //Any
#define P3325_MAXMUM_PIXEL         0x09								  //Any
#define P3325_MIN_PIXEL            0x0A								  //Any
#define P3325_SHUTTER_LOWER        0x0B								  //Any
#define P3325_SHUTTER_UPPER        0x0C								  //Any
#define P3325_CHIP_OBSERVATION     0x15
#define P3325_BURST_MOTION_READ    0x16								  //Any
#define P3325_FRAME_CAPTURE_STATUS 0x19
#define P3325_RESOLUTION           0x1b
#define P3325_ANGLE_SNAP           0x1e
#define P3325_AXIS_CONTROL  	   0x20
#define P3325_RUN_DOWNSHIFT        0x24
#define P3325_REST1_PERIOD         0x25								  //0x08
#define P3325_REST1_DOWNSHIFT      0x26								  //0x08
#define P3325_REST2_PERIOD         0x27								  //0x08
#define P3325_REST2_DOWNSHIFT      0x28
#define P3325_REST3_PERIOD         0x29
#define P3325_FRAM_CAPTURE         0x32
#define P3325_POWER_UP_RESET  	   0x3a
#define P3325_SHUTDOWN             0x3b
#define P3325_INV_PRODUCT_ID	   0x3F
#define P3325_CONFIG 		 	   0x40


void Drv_Sensor_P3325_Init(int16_t *xVal, int16_t *yVal );

void Drv_Sensor_Write(uint8_t addr, uint8_t val );
uint8_t Drv_Sensor_Single_Read(uint8_t addr );
void Drv_Sensor_Multiple_Read(uint8_t addr, uint8_t *buf, uint8_t len );
void Drv_Sensor_Delay(uint32_t delayCnt );

uint8_t Drv_Sensor_Get_Product_ID(void );
void Drv_Sensor_Set_Dpi(uint8_t dpiVal );
void Drv_Sensor_Get_Burst_Motion(uint8_t *buf, uint8_t len );

#endif 

