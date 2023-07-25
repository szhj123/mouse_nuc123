#ifndef _APP_MOUSE_SENSOR_H
#define _APP_MOUSE_SENSOR_H

#include "drv_task.h"
#include "drv_timer.h"
#include "drv_mouse_sensor.h"

typedef struct _mSensor_ctrl_block_t
{
    uint8_t productID;
    int16_t deltaX;
    int16_t deltaY;

    uint16_t delayTime;
    uint16_t delayCnt;
}mSensor_ctrl_block_t;

void App_Sensor_Init(void );
void App_Sensor_Set_Detect_Time(mRate_t mRate );

#endif 

