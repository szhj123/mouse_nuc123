#ifndef _DRV_LIGHT_H
#define _DRV_LIGHT_H

#include "hal_light.h"

typedef enum _color_state_t
{
    COLOR_STATE_RED = 0,
    COLOR_STATE_GREEN,
    COLOR_STATE_BLUE
}color_state_t;

typedef struct _light_ctlr_block_t
{
    port_t port;
    pin_t  pin;
    
    color_state_t colorState;

    uint8_t rVal;
    uint8_t gVal;
    uint8_t bVal;    
}light_ctrl_block_t;

void Drv_Light_Init(void );

void Drv_Light_All_Off(void );
void Drv_Light_All_On(uint8_t rVal, uint8_t gVal, uint8_t bVal );
void Drv_Light_Single_On(light_ctrl_block_t *light );
void Drv_Light_Single_Off(light_ctrl_block_t *light );

#endif 

