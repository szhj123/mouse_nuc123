#ifndef _APP_LIGHT_H
#define _APP_LIGHT_H

#include "drv_light.h"

typedef void (*app_light_callback_t)(void );

void App_Light_Init(void );
void App_Light_Display(void );
void App_Light_Solid_On(uint8_t rVal, uint8_t gVal, uint8_t bVal );
void App_Light_Solid_Off(void );

#endif 

