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
static void App_Light_Breath_Callback(void );
static void App_Light_Neon_Callback(void );
static void App_Light_Blink_Callback(void );
/* Private variables ------------------------------------*/
static light_t  lightBuf[] = 
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

static light_ctrl_block_t lightCtrl;
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

    color_t color;

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

void App_Light_Breath(mLight_data_t lightData )
{
    switch(lightData.speed)
    {
        case 1: lightCtrl.delayTime = 20; break;
        case 2: lightCtrl.delayTime = 15; break;
        case 3: lightCtrl.delayTime = 10; break;
        case 4: lightCtrl.delayTime =  5; break;
        default: lightCtrl.delayTime = 5; break;
    }
    
    lightCtrl.delayCnt = 0;
    lightCtrl.brightness = 0;
    lightCtrl.brightnessMax = 100;
    lightCtrl.colorIndex = 0;
    lightCtrl.colorNum = lightData.colorNum;
    lightCtrl.direction = 0;

    App_Mouse_Get_Light_Color(LIGHT_MODE_BREATH, lightCtrl.colorIndex, &lightCtrl.targetColor);

    app_light_callback = App_Light_Breath_Callback;
}

static void App_Light_Set_Brightness(uint8_t brightness, uint8_t brightnessMax, color_t targeColor, color_t *color )
{
    color->red = (uint32_t )targeColor.red * brightness / brightnessMax;
    color->green = (uint32_t )targeColor.green * brightness / brightnessMax;
    color->blue = (uint32_t )targeColor.blue * brightness / brightnessMax;
}


static void App_Light_Breath_Callback(void )
{
    color_t color;
    
    if(++lightCtrl.delayCnt >= lightCtrl.delayTime)
    {
        lightCtrl.delayCnt = 0;

        if(!lightCtrl.direction)
        {
            if(lightCtrl.brightness < lightCtrl.brightnessMax)
            {
                App_Light_Set_Brightness(lightCtrl.brightness, lightCtrl.brightnessMax, lightCtrl.targetColor, &color);

                Drv_Light_All_On(color.red, color.green, color.blue);

                lightCtrl.brightness++;
            }

            if(lightCtrl.brightness == lightCtrl.brightnessMax)
            {
                lightCtrl.direction = 1;
            }
        }
        else
        {
            if(lightCtrl.brightness > 0)
            {
                App_Light_Set_Brightness(lightCtrl.brightness, lightCtrl.brightnessMax, lightCtrl.targetColor, &color);

                Drv_Light_All_On(color.red, color.green, color.blue);

                lightCtrl.brightness--;
            }

            if(lightCtrl.brightness == 0)
            {
                lightCtrl.direction = 0;
                
                lightCtrl.colorIndex++;

                if(lightCtrl.colorIndex >= lightCtrl.colorNum)
                {
                    lightCtrl.colorIndex = 0;
                }

                App_Mouse_Get_Light_Color(LIGHT_MODE_BREATH, lightCtrl.colorIndex, &lightCtrl.targetColor);
            }
        }
    }
}

void App_Light_Neon(mLight_data_t lightData )
{
    switch(lightData.speed)
    {
        case 1: lightCtrl.delayTime = 20; break;
        case 2: lightCtrl.delayTime = 15; break;
        case 3: lightCtrl.delayTime = 10; break;
        case 4: lightCtrl.delayTime =  5; break;
        default: lightCtrl.delayTime = 5; break;
    }

    lightCtrl.delayCnt = 0;
    
    lightCtrl.targetColor.red = 0xff;
    lightCtrl.targetColor.green = 0;
    lightCtrl.targetColor.blue = 0;

    lightCtrl.color.red = 0;
    lightCtrl.color.green = 0;
    lightCtrl.color.blue = 0;

    lightCtrl.neonColorState = NEON_RED;

    app_light_callback = App_Light_Neon_Callback;
}

void App_Light_Neon_Cal_Color(color_t *color, color_t *targetColor )
{
    if(color->red < targetColor->red)
    {
        color->red++;
    }
    else if(color->red > targetColor->red)
    {
        color->red--;
    }

    if(color->green < targetColor->green)
    {
        color->green++;
    }
    else if(color->green > targetColor->green)
    {
        color->green--;
    }

    if(color->blue < targetColor->blue)
    {
        color->blue++;
    }
    else if(color->blue > targetColor->blue)
    {
        color->blue--;
    }
}

static void App_Light_Neon_Callback(void )
{
    if(++lightCtrl.delayCnt >= lightCtrl.delayTime)
    {
        lightCtrl.delayCnt = 0;

        App_Light_Neon_Cal_Color(&lightCtrl.color, &lightCtrl.targetColor);

        Drv_Light_All_On(lightCtrl.color.red, lightCtrl.color.green, lightCtrl.color.blue);

        switch(lightCtrl.neonColorState)
        {
            case NEON_RED:
            {
                if(lightCtrl.color.red == lightCtrl.targetColor.red && \
                   lightCtrl.color.green == lightCtrl.targetColor.green && \
                   lightCtrl.color.blue == lightCtrl.targetColor.blue)
                {
                    lightCtrl.targetColor.red = 0;
                    lightCtrl.targetColor.green = 0;
                    lightCtrl.targetColor.blue = 255;

                    lightCtrl.neonColorState = NEON_BLUE;
                }
                break;
            }
            case NEON_BLUE:
            {
                if(lightCtrl.color.red == lightCtrl.targetColor.red && \
                   lightCtrl.color.green == lightCtrl.targetColor.green && \
                   lightCtrl.color.blue == lightCtrl.targetColor.blue)
                {
                    lightCtrl.targetColor.red = 0;
                    lightCtrl.targetColor.green = 255;
                    lightCtrl.targetColor.blue = 0;

                    lightCtrl.neonColorState = NEON_GREEN;
                }
                break;
            }
            case NEON_GREEN:
            {
                if(lightCtrl.color.red == lightCtrl.targetColor.red && \
                   lightCtrl.color.green == lightCtrl.targetColor.green && \
                   lightCtrl.color.blue == lightCtrl.targetColor.blue)
                {
                    lightCtrl.targetColor.red = 255;
                    lightCtrl.targetColor.green = 0;
                    lightCtrl.targetColor.blue = 0;

                    lightCtrl.neonColorState = NEON_RED;
                }
                break;
            }
        }
    }
}

void App_Light_Blink(mLight_data_t lightData )
{
    color_t color;

    App_Mouse_Get_Light_Color(LIGHT_MODE_BLINK, 0, &color);
    
    lightBuf[0].red = color.red;
    lightBuf[0].green = color.green;
    lightBuf[0].blue = color.blue;

    lightBuf[2].red = color.red;
    lightBuf[2].green = color.green;
    lightBuf[2].blue = color.blue;

    lightBuf[4].red = color.red;
    lightBuf[4].green = color.green;
    lightBuf[4].blue = color.blue;
    
    App_Mouse_Get_Light_Color(LIGHT_MODE_BLINK, 1, &color);

    lightBuf[1].red = color.red;
    lightBuf[1].green = color.green;
    lightBuf[1].blue = color.blue;

    lightBuf[3].red = color.red;
    lightBuf[3].green = color.green;
    lightBuf[3].blue = color.blue;

    lightBuf[5].red = color.red;
    lightBuf[5].green = color.green;
    lightBuf[5].blue = color.blue;

    lightCtrl.lightIndex = 0;
    lightCtrl.delayCnt = 0;
    lightCtrl.delayTime = 250;
    lightCtrl.direction = 0;

    app_light_callback = App_Light_Blink_Callback;
}

static void App_Light_Blink_Callback(void )
{
    color_t color;
    
    if(lightCtrl.lightIndex < 6)
    {
        Drv_Light_Single_On(&lightBuf[lightCtrl.lightIndex]);

        lightCtrl.lightIndex++;
    }

    if(lightCtrl.lightIndex == 6)
    {
        lightCtrl.lightIndex = 0;
    }

    if(++lightCtrl.delayCnt >= lightCtrl.delayTime)
    {
        lightCtrl.delayCnt = 0;

        if(!lightCtrl.direction)
        {
            lightCtrl.direction = 1;

            App_Mouse_Get_Light_Color(LIGHT_MODE_BLINK, 1, &color);
    
            lightBuf[0].red = color.red;
            lightBuf[0].green = color.green;
            lightBuf[0].blue = color.blue;

            lightBuf[2].red = color.red;
            lightBuf[2].green = color.green;
            lightBuf[2].blue = color.blue;

            lightBuf[4].red = color.red;
            lightBuf[4].green = color.green;
            lightBuf[4].blue = color.blue;
            
            App_Mouse_Get_Light_Color(LIGHT_MODE_BLINK, 0, &color);

            lightBuf[1].red = color.red;
            lightBuf[1].green = color.green;
            lightBuf[1].blue = color.blue;

            lightBuf[3].red = color.red;
            lightBuf[3].green = color.green;
            lightBuf[3].blue = color.blue;

            lightBuf[5].red = color.red;
            lightBuf[5].green = color.green;
            lightBuf[5].blue = color.blue;
        }
        else
        {
            lightCtrl.direction = 0;

            App_Mouse_Get_Light_Color(LIGHT_MODE_BLINK, 0, &color);
    
            lightBuf[0].red = color.red;
            lightBuf[0].green = color.green;
            lightBuf[0].blue = color.blue;

            lightBuf[2].red = color.red;
            lightBuf[2].green = color.green;
            lightBuf[2].blue = color.blue;

            lightBuf[4].red = color.red;
            lightBuf[4].green = color.green;
            lightBuf[4].blue = color.blue;
            
            App_Mouse_Get_Light_Color(LIGHT_MODE_BLINK, 1, &color);

            lightBuf[1].red = color.red;
            lightBuf[1].green = color.green;
            lightBuf[1].blue = color.blue;

            lightBuf[3].red = color.red;
            lightBuf[3].green = color.green;
            lightBuf[3].blue = color.blue;

            lightBuf[5].red = color.red;
            lightBuf[5].green = color.green;
            lightBuf[5].blue = color.blue;
        }
    }
}

void App_Light_Trailer(mLight_data_t lightData )
{
    color_t color;
    
    lightCtrl.targetColor = lightData.lightColorBuf[0];

    App_Light_Set_Brightness(20, 100, lightCtrl.targetColor, &color);

    
}

