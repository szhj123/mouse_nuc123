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
static void App_Light_Color_Streamer_Callback(void );
static void App_Light_Breath_Callback(void );
static void App_Light_Neon_Callback(void );
static void App_Light_Blink_Callback(void );
static void App_Light_MonoChrome_Trailer_Callback(void );
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
static uint8_t lightTimer = TIMER_NULL; 

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
    lightCtrl.direction = lightData.direction;
    
    switch(lightData.speed)
    {
        case 1: lightCtrl.delayTime =  4; break;
        case 2: lightCtrl.delayTime =  3; break;
        case 3: lightCtrl.delayTime =  2; break;
        case 4: lightCtrl.delayTime =  1; break;
        default: lightCtrl.delayTime = 2; break;
    }

    if(lightCtrl.direction)
    {
        lightBuf[0].red = 0;
        lightBuf[0].green = 0;
        lightBuf[0].blue = 0;
        lightBuf[0].colorState = COLOR_STATE_RED;

        lightBuf[1].red = 85;
        lightBuf[1].green = 0;
        lightBuf[1].blue = 0;
        lightBuf[1].colorState = COLOR_STATE_RED;

        lightBuf[2].red = 170;
        lightBuf[2].green = 0;
        lightBuf[2].blue = 0;
        lightBuf[2].colorState = COLOR_STATE_RED;

        lightBuf[3].red = 0;
        lightBuf[3].green = 0;
        lightBuf[3].blue = 0;
        lightBuf[3].colorState = COLOR_STATE_BLUE;

        lightBuf[4].red = 0;
        lightBuf[4].green = 0;
        lightBuf[4].blue = 85;
        lightBuf[4].colorState = COLOR_STATE_BLUE;

        lightBuf[5].red = 0;
        lightBuf[5].green = 0;
        lightBuf[5].blue = 170;
        lightBuf[5].colorState = COLOR_STATE_BLUE;
    }
    else
    {
        lightBuf[5].red = 0;
        lightBuf[5].green = 0;
        lightBuf[5].blue = 0;
        lightBuf[5].colorState = COLOR_STATE_RED;

        lightBuf[4].red = 85;
        lightBuf[4].green = 0;
        lightBuf[4].blue = 0;
        lightBuf[4].colorState = COLOR_STATE_RED;

        lightBuf[3].red = 170;
        lightBuf[3].green = 0;
        lightBuf[3].blue = 0;
        lightBuf[3].colorState = COLOR_STATE_RED;

        lightBuf[2].red = 0;
        lightBuf[2].green = 0;
        lightBuf[2].blue = 0;
        lightBuf[2].colorState = COLOR_STATE_BLUE;

        lightBuf[1].red = 0;
        lightBuf[1].green = 0;
        lightBuf[1].blue = 85;
        lightBuf[1].colorState = COLOR_STATE_BLUE;

        lightBuf[0].red = 0;
        lightBuf[0].green = 0;
        lightBuf[0].blue = 170;
        lightBuf[0].colorState = COLOR_STATE_BLUE;
    }
    
    lightCtrl.delayCnt = 0;
    lightCtrl.lightIndex =0 ;

    app_light_callback = App_Light_Color_Streamer_Callback;
}

void App_Light_Cal_Ramp_Color(color_t *color, color_t *targetColor )
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


void App_Light_Cal_Streamer_Color(light_t *light )
{
    color_t targetColor;
    
    switch(light->colorState)
    {
        case COLOR_STATE_RED:
        {
            targetColor.red = 255; targetColor.green = 0; targetColor.blue = 0;
            
            App_Light_Cal_Ramp_Color((color_t *)&light->red, &targetColor);

            if(light->red == targetColor.red && \
               light->green == targetColor.green && \
               light->blue == targetColor.blue)
            {
                light->colorState = COLOR_STATE_BLUE;
            }
            
            break;
        }
        case COLOR_STATE_BLUE:
        {
            targetColor.red = 0; targetColor.green = 0; targetColor.blue = 255;
            
            App_Light_Cal_Ramp_Color((color_t *)&light->red, &targetColor);

            if(light->red == targetColor.red && \
               light->green == targetColor.green && \
               light->blue == targetColor.blue)
            {
                light->colorState = COLOR_STATE_GREEN;
            }
            break;
        }
        case COLOR_STATE_GREEN:
        {
            targetColor.red = 0; targetColor.green = 255; targetColor.blue = 0;
            
            App_Light_Cal_Ramp_Color((color_t *)&light->red, &targetColor);

            if(light->red == targetColor.red && \
               light->green == targetColor.green && \
               light->blue == targetColor.blue)
            {
                light->colorState = COLOR_STATE_RED;
            }
            break;
        }
    }
}

static void App_Light_Color_Streamer_Callback(void )
{
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

        App_Light_Cal_Streamer_Color(&lightBuf[0]);
        App_Light_Cal_Streamer_Color(&lightBuf[1]);
        App_Light_Cal_Streamer_Color(&lightBuf[2]);
        App_Light_Cal_Streamer_Color(&lightBuf[3]);
        App_Light_Cal_Streamer_Color(&lightBuf[4]);
        App_Light_Cal_Streamer_Color(&lightBuf[5]);
    }
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

static void App_Light_Neon_Callback(void )
{
    if(++lightCtrl.delayCnt >= lightCtrl.delayTime)
    {
        lightCtrl.delayCnt = 0;

        App_Light_Cal_Ramp_Color(&lightCtrl.color, &lightCtrl.targetColor);

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

void App_Light_Clear_All_Color(void )
{
    uint8_t i;

    for(i=0;i<sizeof(lightBuf)/sizeof(light_t);i++)
    {
        lightBuf[i].red = 0;
        lightBuf[i].green = 0;
        lightBuf[i].blue = 0;
    }
}

void App_Light_Set_Trailer_Color(uint8_t lightIndex, color_t targetColor, uint8_t brightness )
{
    color_t color;
    
    App_Light_Set_Brightness(brightness, 100, targetColor, &color);
    
    lightBuf[lightIndex].red = color.red;
    lightBuf[lightIndex].green = color.green;
    lightBuf[lightIndex].blue = color.blue;
}


void App_Light_MonoChrome_Trailer(mLight_data_t lightData )
{
    lightCtrl.targetColor = lightData.lightColorBuf[0];

    lightCtrl.direction = lightData.direction;

    switch(lightData.speed)
    {
        case 1: lightCtrl.delayTime = 250; break;
        case 2: lightCtrl.delayTime = 200; break;
        case 3: lightCtrl.delayTime = 150; break;
        case 4: lightCtrl.delayTime = 100; break;
        default: lightCtrl.delayTime = 150; break;
    }

    lightCtrl.colorIndex = 0;

    lightCtrl.colorNum = 0;

    lightCtrl.lightIndex = 0;

    lightCtrl.delayCnt = 0;

    lightCtrl.lightNum = 0;

    lightCtrl.trailerShowStep = 0;

    app_light_callback = App_Light_MonoChrome_Trailer_Callback;
}


static void App_Light_MonoChrome_Trailer_Callback(void )
{
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
        
        switch(lightCtrl.lightNum)
        {
            case 0:
            {
                App_Light_Clear_All_Color();

                if(lightCtrl.direction)
                {
                    App_Light_Set_Trailer_Color(0, lightCtrl.targetColor, 100);
                }
                else
                {
                    App_Light_Set_Trailer_Color(5, lightCtrl.targetColor, 100);
                }
                lightCtrl.lightNum = 1;
                break;
            }
            case 1:
            {
                App_Light_Clear_All_Color();
                
                if(lightCtrl.direction)
                {
                    App_Light_Set_Trailer_Color(1, lightCtrl.targetColor, 100);
                    App_Light_Set_Trailer_Color(0, lightCtrl.targetColor, 75);
                }
                else
                {
                    App_Light_Set_Trailer_Color(4, lightCtrl.targetColor, 100);
                    App_Light_Set_Trailer_Color(5, lightCtrl.targetColor, 75);
                }
                lightCtrl.lightNum = 2;
                break;
            }
            case 2:
            {
                App_Light_Clear_All_Color();
                
                if(lightCtrl.direction)
                {
                    App_Light_Set_Trailer_Color(2, lightCtrl.targetColor, 100);
                    App_Light_Set_Trailer_Color(1, lightCtrl.targetColor, 75);
                    App_Light_Set_Trailer_Color(0, lightCtrl.targetColor, 50);
                }
                else
                {
                    App_Light_Set_Trailer_Color(3, lightCtrl.targetColor, 100);
                    App_Light_Set_Trailer_Color(4, lightCtrl.targetColor, 75);
                    App_Light_Set_Trailer_Color(5, lightCtrl.targetColor, 50);
                }
                lightCtrl.lightNum = 3;
                break;
            }
            case 3:
            {
                App_Light_Clear_All_Color();

                if(lightCtrl.direction)
                {
                    App_Light_Set_Trailer_Color(3, lightCtrl.targetColor, 100);
                    App_Light_Set_Trailer_Color(2, lightCtrl.targetColor, 75);
                    App_Light_Set_Trailer_Color(1, lightCtrl.targetColor, 50);
                    App_Light_Set_Trailer_Color(0, lightCtrl.targetColor, 25);
                }
                else
                {
                    App_Light_Set_Trailer_Color(2, lightCtrl.targetColor, 100);
                    App_Light_Set_Trailer_Color(3, lightCtrl.targetColor, 75);
                    App_Light_Set_Trailer_Color(4, lightCtrl.targetColor, 50);
                    App_Light_Set_Trailer_Color(5, lightCtrl.targetColor, 25);
                }
                lightCtrl.lightNum = 4;
                break;
            }
            case 4:
            {
                App_Light_Clear_All_Color();
                
                if(lightCtrl.direction)
                {
                    App_Light_Set_Trailer_Color(4, lightCtrl.targetColor, 100);
                    App_Light_Set_Trailer_Color(3, lightCtrl.targetColor, 75);
                    App_Light_Set_Trailer_Color(2, lightCtrl.targetColor, 50);
                    App_Light_Set_Trailer_Color(1, lightCtrl.targetColor, 25);
                }
                else
                {
                    App_Light_Set_Trailer_Color(1, lightCtrl.targetColor, 100);
                    App_Light_Set_Trailer_Color(2, lightCtrl.targetColor, 75);
                    App_Light_Set_Trailer_Color(3, lightCtrl.targetColor, 50);
                    App_Light_Set_Trailer_Color(4, lightCtrl.targetColor, 25);
                }
                lightCtrl.lightNum = 5;
                break;
            }
            case 5:
            {
                App_Light_Clear_All_Color();

                if(lightCtrl.direction)
                {
                    App_Light_Set_Trailer_Color(5, lightCtrl.targetColor, 100);
                    App_Light_Set_Trailer_Color(4, lightCtrl.targetColor, 75);
                    App_Light_Set_Trailer_Color(3, lightCtrl.targetColor, 50);
                    App_Light_Set_Trailer_Color(2, lightCtrl.targetColor, 25);
                }
                else
                {
                    App_Light_Set_Trailer_Color(0, lightCtrl.targetColor, 100);
                    App_Light_Set_Trailer_Color(1, lightCtrl.targetColor, 75);
                    App_Light_Set_Trailer_Color(2, lightCtrl.targetColor, 50);
                    App_Light_Set_Trailer_Color(3, lightCtrl.targetColor, 25);
                }
                lightCtrl.lightNum = 6;
                break;
            }
            case 6:
            {
                App_Light_Clear_All_Color();
                
                if(lightCtrl.direction)
                {
                    App_Light_Set_Trailer_Color(5, lightCtrl.targetColor, 75);
                    App_Light_Set_Trailer_Color(4, lightCtrl.targetColor, 50);
                    App_Light_Set_Trailer_Color(3, lightCtrl.targetColor, 25);
                }
                else
                {
                    App_Light_Set_Trailer_Color(0, lightCtrl.targetColor, 75);
                    App_Light_Set_Trailer_Color(1, lightCtrl.targetColor, 50);
                    App_Light_Set_Trailer_Color(2, lightCtrl.targetColor, 25);
                }

                lightCtrl.lightNum = 7;
                break;
            }
            case 7:
            {
                App_Light_Clear_All_Color();

                if(lightCtrl.direction)
                {
                    App_Light_Set_Trailer_Color(5, lightCtrl.targetColor, 50);
                    App_Light_Set_Trailer_Color(4, lightCtrl.targetColor, 25);
                }
                else
                {
                    App_Light_Set_Trailer_Color(0, lightCtrl.targetColor, 50);
                    App_Light_Set_Trailer_Color(1, lightCtrl.targetColor, 25);
                }

                lightCtrl.lightNum = 8;
                break;
            }
            case 8:
            {
                App_Light_Clear_All_Color();
                
                if(lightCtrl.direction)
                {
                    App_Light_Set_Trailer_Color(5, lightCtrl.targetColor, 25);
                }
                else
                {
                    App_Light_Set_Trailer_Color(0, lightCtrl.targetColor, 25);
                }

                lightCtrl.lightNum = 9;
                break;
            }
            case 9:
            {
                App_Light_Clear_All_Color();

                if(lightCtrl.colorIndex < lightCtrl.colorNum)
                {
                    lightCtrl.colorIndex++;

                    if(lightCtrl.colorIndex == lightCtrl.colorNum)
                    {
                        lightCtrl.colorIndex = 0;
                    }

                    lightCtrl.direction ^= 1;
                    
                    App_Mouse_Get_Light_Color(LIGHT_MODE_COLOURFUL_TRAILER, lightCtrl.colorIndex, &lightCtrl.targetColor);
                }
            
                lightCtrl.lightNum = 0;
                break;
            }
            default: break;
        }
    }
}

static void App_Light_Response_Callback(void *arg )
{
    Drv_Light_All_Off();
}

void App_Light_Response(mLight_data_t lightData )
{
    uint16_t delayTime;
    
    color_t color = lightData.lightColorBuf[lightCtrl.colorIndex];

    switch(lightData.speed)
    {
        case 1: delayTime = 2000; break;
        case 2: delayTime = 1500; break;
        case 3: delayTime = 1000; break;
        case 4: delayTime = 500; break;
        default: delayTime = 1000; break;
    }
    
    Drv_Light_All_On(color.red, color.green, color.blue);

    lightCtrl.colorIndex++;

    if(lightCtrl.colorIndex >= lightData.colorNum)
    {
        lightCtrl.colorIndex = 0;
    }

    Drv_Timer_Delete(lightTimer);
    
    lightTimer = Drv_Timer_Regist_Oneshot(App_Light_Response_Callback, delayTime, NULL);
        
}

void App_Light_Colourful_Trailer(mLight_data_t lightData )
{
    lightCtrl.targetColor = lightData.lightColorBuf[0];

    lightCtrl.direction = lightData.direction;
    
    lightCtrl.colorNum = lightData.colorNum;

    switch(lightData.speed)
    {
        case 1: lightCtrl.delayTime = 250; break;
        case 2: lightCtrl.delayTime = 200; break;
        case 3: lightCtrl.delayTime = 150; break;
        case 4: lightCtrl.delayTime = 100; break;
        default: lightCtrl.delayTime = 150; break;
    }
    
    lightCtrl.colorIndex = 0;

    lightCtrl.lightIndex = 0;

    lightCtrl.delayCnt = 0;

    lightCtrl.lightNum = 0;

    lightCtrl.trailerShowStep = 0;

    app_light_callback = App_Light_MonoChrome_Trailer_Callback;
}

void App_Light_Set_Light_Effect(mLight_mode_t lightMode)
{
    mLight_data_t lightData;
    
    if(lightMode == LIGHT_MODE_OFF)
    {
        lightCtrl.lightOffFlag = LIGHT_OFF;
    }
    else
    {
        lightCtrl.lightOffFlag = LIGHT_ON;
    }

    switch(lightMode)
    {
        case LIGHT_MODE_OFF: App_Light_Off(); break;
        case LIGHT_MODE_COLOR_STREAM: 
        {
            App_Mouse_Get_Light_Data(LIGHT_MODE_COLOR_STREAM, &lightData);
            
            App_Light_Color_Streamer(lightData); 
            break;
        }
        case LIGHT_MODE_SOLID: 
        {
             App_Mouse_Get_Light_Data(LIGHT_MODE_SOLID, &lightData);
             
             App_Light_Solid(lightData); 
             break;
        }
        case LIGHT_MODE_BREATH: 
        {
            App_Mouse_Get_Light_Data(LIGHT_MODE_BREATH, &lightData);
            
            App_Light_Breath(lightData); 
            break;
        }
        case LIGHT_MODE_NEON: 
        {
            App_Mouse_Get_Light_Data(LIGHT_MODE_NEON, &lightData);
            
            App_Light_Neon(lightData); 
            break;
        }
        case LIGHT_MODE_BLINK: 
        {
            App_Mouse_Get_Light_Data(LIGHT_MODE_BLINK, &lightData);
            
            App_Light_Blink(lightData); 
            break;
        }
        case LIGHT_MODE_MONOCHROME_TRAILER: 
        {
            App_Mouse_Get_Light_Data(LIGHT_MODE_MONOCHROME_TRAILER, &lightData);
            
            App_Light_MonoChrome_Trailer(lightData); 
            break;
        }
        case LIGHT_MODE_RESPONSE: 
        {            
            lightCtrl.colorIndex = 0;
            
            app_light_callback = NULL;

            Drv_Light_All_Off();
            
            break;
        }
        case LIGHT_MODE_COLOURFUL_TRAILER: 
        {
            App_Mouse_Get_Light_Data(LIGHT_MODE_COLOURFUL_TRAILER, &lightData);
            
            App_Light_Colourful_Trailer(lightData); 
            break;
        }
        default: break;
    }
}

void App_Light_Set_Off_Flag(uint8_t flag )
{
    lightCtrl.lightOffFlag = flag;
}

uint8_t App_Light_Get_Off_Flag(void )
{
    return lightCtrl.lightOffFlag;    
}

static void App_Light_Set_Dpi_Color_Callback(void *arg )
{
    mLight_mode_t lightMode = App_Mouse_Get_Light_Mode();
    
    App_Light_Set_Light_Effect(lightMode);
}

void App_Light_Set_Dpi_Color(uint8_t dpiColorIndex )
{
    color_t dpiColor;

    App_Mouse_Get_Dpi_Color(dpiColorIndex, &dpiColor);

    app_light_callback = NULL;
    
    Drv_Light_All_On(dpiColor.red, dpiColor.green, dpiColor.blue);

    Drv_Timer_Delete(lightTimer);
    
    lightTimer = Drv_Timer_Regist_Oneshot(App_Light_Set_Dpi_Color_Callback, 1000, NULL);
}

