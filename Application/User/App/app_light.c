/********************************************************
* @file       app_light.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_light.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
static light_ctrl_block_t  lightBuf[] = 
{
    {
        .port = PORTB,
        .pin  = PIN8
    },
    {
        .port = PORTB,
        .pin  = PIN14
    },
    {
        .port = PORTA,
        .pin  = PIN11
    },
    {
        .port = PORTA,
        .pin  = PIN10
    },
    {   .port = PORTB,
        .pin  = PIN4
    },
    {
        .port = PORTB,
        .pin  = PIN5
    }
};

static app_light_callback_t app_light_callback = NULL;


void App_Light_Init(void )
{
    Drv_Light_Init();   

    App_Light_Solid_On(255, 0, 0);
}

void App_Light_Display(void )
{
    if(app_light_callback != NULL)
    {
        app_light_callback();
    }
}

void App_Light_Solid_On(uint8_t rVal, uint8_t gVal, uint8_t bVal )
{
    Drv_Light_All_On(rVal, gVal, bVal);

    app_light_callback = NULL;
}

void App_Light_Solid_Off(void )
{
    Drv_Light_All_Off();
}

