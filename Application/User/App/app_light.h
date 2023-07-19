#ifndef _APP_LIGHT_H
#define _APP_LIGHT_H

#include "app_mouse_protocol.h"
#include "drv_light.h"
#include "drv_task.h"

typedef void (*app_light_callback_t)(void );

void App_Light_Init(void );
void App_Light_Off(void );
void App_Light_Color_Streamer(mLight_data_t lightData );
void App_Light_Solid(mLight_data_t lightData );

#endif 

