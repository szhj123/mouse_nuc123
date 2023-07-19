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
static void App_Light_Handler(void *arg );
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
    
    Drv_Task_Regist_Period(App_Light_Handler, 0, 1, NULL);
}

static void App_Light_Handler(void *arg )
{
    if(app_light_callback != NULL)
    {
        app_light_callback();
    }
}

void App_Light_Off(void )
{
    app_light_callback = NULL;
    
    Drv_Light_All_Off();
}

void App_Light_Color_Streamer(mLight_data_t lightData )
{
    
}

void App_Light_Solid(mLight_data_t lightData )
{   
    app_light_callback = NULL;

    mColor_t color;

    switch(lightData.brightness)
    {
        case 1: 
        {
            color.red = lightData.lightColorBuf[0].red / 4;
            color.green = lightData.lightColorBuf[0].green / 4;
            color.blue = lightData.lightColorBuf[0].blue / 4;
            break;
        }
        case 2: 
        {
            color.red = lightData.lightColorBuf[0].red / 2;
            color.green = lightData.lightColorBuf[0].green / 2;
            color.blue = lightData.lightColorBuf[0].blue / 2;
            break;
        }
        case 3: 
        {
            color.red = lightData.lightColorBuf[0].red * 3 / 4;
            color.green = lightData.lightColorBuf[0].green * 3 / 4;
            color.blue = lightData.lightColorBuf[0].blue * 3 / 4;
            break;
        }
        case 4: 
        {
            color.red = lightData.lightColorBuf[0].red;
            color.green = lightData.lightColorBuf[0].green;
            color.blue = lightData.lightColorBuf[0].blue;
            break;
        }
        default: break;
    }

    Drv_Light_All_On(color.red, color.green, color.blue);
}



