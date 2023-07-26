/********************************************************
* @file       app_key.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_key.h"
#include "app_event.h"
#include "app_usb.h"
#include "app_light.h"
#include "app_mouse_sensor.h"
#include "app_mouse_protocol.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Key_MsgPut(uint8_t *msgBuf, uint8_t msgLen );
static void App_Key_Press_Handler(mKey_t mKey );
static void App_Key_Relase_Handler(mKey_t mkey );

static void App_Key_Dpi_Press_Handler(mKey_t mKey );
static void App_Key_Keyboard_Press_Handler(mKey_t mKey );
static void App_Key_Multimedia_Press_Handler(mKey_t mKey );
static void App_Key_Macro_Press_Handler(mKey_t mKey );
static void App_Key_Other_Press_Handler(mKey_t mKey );

static void App_Key_Keyboard_Relase_Handler(mKey_t mKey );
static void App_Key_Multimedia_Relase_Handler(mKey_t mKey );
static void App_Key_Dpi_Relase_Handler(mKey_t mKey );
static void App_Key_Macro_Relase_Handler(mKey_t mKey );
static void App_Key_Other_Relase_Handler(mKey_t mKey );

/* Private variables ------------------------------------*/

void App_Key_Init(void )
{
    Drv_Key_Init();

    Drv_Key_Regist_MsgPut_Callback(App_Key_MsgPut );
}

static void App_Key_MsgPut(uint8_t *msgBuf, uint8_t msgLen )
{
    Drv_Msg_Put(APP_EVENT_KEY, msgBuf, msgLen);
}

void App_Key_Handler(uint8_t *buf, uint8_t len )
{
    uint16_t keyVal = KEY_NULL;
    mKey_t mKey;

    keyVal = (uint16_t )buf[1] << 8 | buf[0];

    if(keyVal & KEY_DOWN)
    {
        switch(keyVal)
        {
            case KEY_1 | KEY_DOWN: App_Mouse_Get_Key(0, &mKey);break;
            case KEY_2 | KEY_DOWN: App_Mouse_Get_Key(1, &mKey);break;
            case KEY_3 | KEY_DOWN: App_Mouse_Get_Key(2, &mKey);break;
            case KEY_4 | KEY_DOWN: App_Mouse_Get_Key(3, &mKey);break;
            case KEY_5 | KEY_DOWN: App_Mouse_Get_Key(4, &mKey);break;
            case KEY_6 | KEY_DOWN: App_Mouse_Get_Key(5, &mKey);break;
            case KEY_7 | KEY_DOWN: App_Mouse_Get_Key(6, &mKey);break;
            case KEY_8 | KEY_DOWN: App_Mouse_Get_Key(7, &mKey);break;
            case KEY_9 | KEY_DOWN: App_Mouse_Get_Key(8, &mKey);break;
            default: break;
        }

        App_Key_Press_Handler(mKey);
    }
    else if(keyVal & KEY_UP)
    {
        switch(keyVal)
        {
            case KEY_1 | KEY_UP: App_Mouse_Get_Key(0, &mKey);break;
            case KEY_2 | KEY_UP: App_Mouse_Get_Key(1, &mKey);break;
            case KEY_3 | KEY_UP: App_Mouse_Get_Key(2, &mKey);break;
            case KEY_4 | KEY_UP: App_Mouse_Get_Key(3, &mKey);break;
            case KEY_5 | KEY_UP: App_Mouse_Get_Key(4, &mKey);break;
            case KEY_6 | KEY_UP: App_Mouse_Get_Key(5, &mKey);break;
            case KEY_7 | KEY_UP: App_Mouse_Get_Key(6, &mKey);break;
            case KEY_8 | KEY_UP: App_Mouse_Get_Key(7, &mKey);break;
            case KEY_9 | KEY_UP: App_Mouse_Get_Key(8, &mKey);break;
            default: break;
        }

        App_Key_Relase_Handler(mKey);
    }

    if(keyVal == (KEY_10 | WHEEL_DONW))
    {
        App_Usb_Mouse_Wheel_Handler(MOUSE_WHEEL_DOWM);
    }
    else if(keyVal == (KEY_10 | WHEEL_UP))
    {
        App_Usb_Mouse_Wheel_Handler(MOUSE_WHEEL_UP);
    }
}

static void App_Key_Press_Handler(mKey_t mKey )
{
    switch(mKey.name)
    {
        case KEY_NAME_MOUSE: App_Usb_Mouse_Press_Handler(mKey);break;
        case KEY_NAME_KEYBOARD: App_Key_Keyboard_Press_Handler(mKey);break;
        case KEY_NAME_MULTIMEDIA: App_Key_Multimedia_Press_Handler(mKey);break;
        case KEY_NAME_DPI: App_Key_Dpi_Press_Handler(mKey);break;
        case KEY_NAME_MACRO: App_Key_Macro_Press_Handler(mKey);break;
        case KEY_NAME_OTHER: App_Key_Other_Press_Handler(mKey);break;
        default: break;
    }
}

static void App_Key_Relase_Handler(mKey_t mKey )
{
    switch(mKey.name)
    {
        case KEY_NAME_MOUSE: App_Usb_Mouse_Relase_Handler(mKey);break;
        case KEY_NAME_KEYBOARD: App_Key_Keyboard_Relase_Handler(mKey);break;
        case KEY_NAME_MULTIMEDIA: App_Key_Multimedia_Relase_Handler(mKey);break;
        case KEY_NAME_DPI: App_Key_Dpi_Relase_Handler(mKey);break;
        case KEY_NAME_MACRO: App_Key_Macro_Relase_Handler(mKey);break;
        case KEY_NAME_OTHER: App_Key_Other_Relase_Handler(mKey);break;
        default: break;
    }
}

static void App_Key_Dpi_Press_Handler(mKey_t mKey )
{
    uint8_t dpiIndex = App_Mouse_Get_Dpi_Index();
    
    if(mKey.func == (uint8_t )FUNC_DPI_LOOP)
    {
        if(dpiIndex < App_Mouse_Get_Dpi_Total_Num())
        {
            dpiIndex++;
        }
        else
        {
            dpiIndex = 1;
        }
    }
    else if(mKey.func == (uint8_t )FUNC_DPI_INC)
    {
        if(dpiIndex < App_Mouse_Get_Dpi_Total_Num())
        {
            dpiIndex++;
        }
    }
    else if(mKey.func == (uint8_t )FUNC_DPI_DEC)
    {
        if(dpiIndex > 1)
        {
            dpiIndex--;
        }
    }
    
    App_Mouse_Set_Dpi_Index(dpiIndex);

    App_Sensor_Set_Dpi(dpiIndex);

    App_Light_Set_Dpi_Color(dpiIndex-1);

    App_Usb_Mouse_Dpi_Input(0x01, dpiIndex);
}


static void App_Key_Keyboard_Press_Handler(mKey_t mKey )
{
    
}

static void App_Key_Multimedia_Press_Handler(mKey_t mKey )
{
    
}


static void App_Key_Macro_Press_Handler(mKey_t mKey )
{
    
}

static void App_Key_Other_Press_Handler(mKey_t mKey )
{
    
}


static void App_Key_Keyboard_Relase_Handler(mKey_t mKey )
{
    
}

static void App_Key_Multimedia_Relase_Handler(mKey_t mKey )
{
    
}

static void App_Key_Dpi_Relase_Handler(mKey_t mKey )
{
    
}

static void App_Key_Macro_Relase_Handler(mKey_t mKey )
{
    
}

static void App_Key_Other_Relase_Handler(mKey_t mKey )
{
    
}






