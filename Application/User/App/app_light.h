#ifndef _APP_LIGHT_H
#define _APP_LIGHT_H

#include "app_mouse_protocol.h"
#include "drv_light.h"
#include "drv_task.h"
#include "drv_timer.h"

#define LIGHT_OFF                0
#define LIGHT_ON                 (!LIGHT_OFF)

typedef void (*app_light_callback_t)(void );

typedef enum _neon_color_state_t
{
    NEON_RED = 0,
    NEON_BLUE,
    NEON_GREEN
}neon_color_state;


typedef struct _light_ctrl_block_t
{
    neon_color_state neonColorState;
    color_t  targetColor;
    color_t  color;
    uint16_t delayCnt;
    uint16_t delayTime;
    uint8_t  brightness;
    uint8_t  brightnessMax;
    uint8_t  colorIndex;
    uint8_t  colorNum;
    uint8_t  direction;
    uint8_t  lightIndex;
    uint8_t  lightNum;
    uint8_t  trailerShowStep;
    uint8_t  lightOffFlag;
}light_ctrl_block_t;


void App_Light_Init(void );
void App_Light_Off(void );
void App_Light_Color_Streamer(mLight_data_t lightData );
void App_Light_Solid(mLight_data_t lightData );
void App_Light_Breath(mLight_data_t lightData );
void App_Light_Neon(mLight_data_t lightData );
void App_Light_Blink(mLight_data_t lightData );
void App_Light_MonoChrome_Trailer(mLight_data_t lightData );
void App_Light_Response(mLight_data_t lightData );
void App_Light_Colourful_Trailer(mLight_data_t lightData );

void App_Light_Set_Off_Flag(uint8_t flag );
uint8_t App_Light_Get_Off_Flag(void );
void App_Light_Set_Light_Effect(mLight_mode_t lightMode);
void App_Light_Set_Dpi_Color(uint8_t dpiColorIndex );

#endif 

