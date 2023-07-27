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

static void App_Key_Mouse_Press_Handler(mKey_t mKey );
static void App_Key_Dpi_Press_Handler(mKey_t mKey );
static void App_Key_Keyboard_Press_Handler(mKey_t mKey );
static void App_Key_Multimedia_Press_Handler(mKey_t mKey );
static void App_Key_Macro_Press_Handler(mKey_t mKey );
static void App_Key_Other_Press_Handler(mKey_t mKey );

static void App_Key_Mouse_Release_Handler(mKey_t mKey );
static void App_Key_Keyboard_Release_Handler(mKey_t mKey );
static void App_Key_Multimedia_Release_Handler(mKey_t mKey );
static void App_Key_Dpi_Release_Handler(mKey_t mKey );
static void App_Key_Macro_Release_Handler(mKey_t mKey );
static void App_Key_Other_Release_Handler(mKey_t mKey );

/* Private variables ------------------------------------*/
static u8Data_t mKeyVal;

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
            case KEY_1 | KEY_DOWN: App_Mouse_Get_Key_Data(0, &mKey);break;
            case KEY_2 | KEY_DOWN: App_Mouse_Get_Key_Data(1, &mKey);break;
            case KEY_3 | KEY_DOWN: App_Mouse_Get_Key_Data(2, &mKey);break;
            case KEY_4 | KEY_DOWN: App_Mouse_Get_Key_Data(3, &mKey);break;
            case KEY_5 | KEY_DOWN: App_Mouse_Get_Key_Data(4, &mKey);break;
            case KEY_6 | KEY_DOWN: App_Mouse_Get_Key_Data(5, &mKey);break;
            case KEY_7 | KEY_DOWN: App_Mouse_Get_Key_Data(6, &mKey);break;
            case KEY_8 | KEY_DOWN: App_Mouse_Get_Key_Data(7, &mKey);break;
            case KEY_9 | KEY_DOWN: App_Mouse_Get_Key_Data(8, &mKey);break;
            default: break;
        }

        App_Key_Press_Handler(mKey);
    }
    else if(keyVal & KEY_UP)
    {
        switch(keyVal)
        {
            case KEY_1 | KEY_UP: App_Mouse_Get_Key_Data(0, &mKey);break;
            case KEY_2 | KEY_UP: App_Mouse_Get_Key_Data(1, &mKey);break;
            case KEY_3 | KEY_UP: App_Mouse_Get_Key_Data(2, &mKey);break;
            case KEY_4 | KEY_UP: App_Mouse_Get_Key_Data(3, &mKey);break;
            case KEY_5 | KEY_UP: App_Mouse_Get_Key_Data(4, &mKey);break;
            case KEY_6 | KEY_UP: App_Mouse_Get_Key_Data(5, &mKey);break;
            case KEY_7 | KEY_UP: App_Mouse_Get_Key_Data(6, &mKey);break;
            case KEY_8 | KEY_UP: App_Mouse_Get_Key_Data(7, &mKey);break;
            case KEY_9 | KEY_UP: App_Mouse_Get_Key_Data(8, &mKey);break;
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
        case KEY_NAME_MOUSE: App_Key_Mouse_Press_Handler(mKey);break;
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
        case KEY_NAME_MOUSE: App_Key_Mouse_Release_Handler(mKey);break;
        case KEY_NAME_KEYBOARD: App_Key_Keyboard_Release_Handler(mKey);break;
        case KEY_NAME_MULTIMEDIA: App_Key_Multimedia_Release_Handler(mKey);break;
        case KEY_NAME_DPI: App_Key_Dpi_Release_Handler(mKey);break;
        case KEY_NAME_MACRO: App_Key_Macro_Release_Handler(mKey);break;
        case KEY_NAME_OTHER: App_Key_Other_Release_Handler(mKey);break;
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

    App_Usb_Mouse_Evt_Input(0x01, dpiIndex);
}

static void App_Key_Mouse_Press_Handler(mKey_t mKey )
{
    mLight_data_t lightData;
    
    switch(mKey.func)
    {
        case FUNC_MOUSE_LEFT: mKeyVal.bit_t.bit0 = 1;break;
        case FUNC_MOUSE_RIGHT: mKeyVal.bit_t.bit1 = 1; break;
        case FUNC_MOUSE_MIDDLE: mKeyVal.bit_t.bit2 = 1; break;
        case FUNC_MOUSE_FORWARD: mKeyVal.bit_t.bit3 = 1; break;
        case FUNC_MOUSE_BACKWARD: mKeyVal.bit_t.bit4 = 1; break;
        default: break;
    }

    if(App_Mouse_Get_Light_Mode() == LIGHT_MODE_RESPONSE)
    {
        App_Mouse_Get_Light_Data(LIGHT_MODE_RESPONSE, &lightData);
            
        App_Light_Response(lightData); 
    }

    App_Usb_Mouse_Press_Handler(mKeyVal.u8Data);
}


static void App_Key_Keyboard_Press_Handler(mKey_t mKey )
{
    uint8_t i;
    uint8_t tmpBuf[3] = {0};

    tmpBuf[0] = 0x0;
    
    for(i=0;i<8;i++)
    {
        if(mKey.func & (1<<i))
        {
            tmpBuf[0] |= (1 << i);
        }
    }

    tmpBuf[1] = mKey.val_l;
    tmpBuf[2] = mKey.val_h;

    App_Usb_Keyboard_Press_Handler(RPT_ID_KEYBOARD, tmpBuf, sizeof(tmpBuf));
}

static void App_Key_Multimedia_Press_Handler(mKey_t mKey )
{
    uint16_t multiMediaVal = 0;
    uint8_t i;
    u8Data_t tmpBuf[3];

    multiMediaVal = mKey.val_l;
    multiMediaVal = multiMediaVal << 8 | mKey.val_h;

    for(i=0;i<3;i++)
    {
        tmpBuf[i].u8Data = 0x0;
    }

    switch(multiMediaVal)
    {
        case 0x0221: tmpBuf[0].bit_t.bit0 = 1;break;
        case 0x0223: tmpBuf[0].bit_t.bit1 = 1;break;
        case 0x0224: tmpBuf[0].bit_t.bit2 = 1;break;
        case 0x0225: tmpBuf[0].bit_t.bit3 = 1;break;
        case 0x0226: tmpBuf[0].bit_t.bit4 = 1;break;
        case 0x0227: tmpBuf[0].bit_t.bit5 = 1;break;
        case 0x022a: tmpBuf[0].bit_t.bit6 = 1;break;
        case 0x02b1: tmpBuf[0].bit_t.bit7 = 1;break;

        case 0x00b5: tmpBuf[1].bit_t.bit0 = 1;break;
        case 0x00b6: tmpBuf[1].bit_t.bit1 = 1;break;
        case 0x00b7: tmpBuf[1].bit_t.bit2 = 1;break;
        case 0x00cd: tmpBuf[1].bit_t.bit3 = 1;break;
        case 0x00e2: tmpBuf[1].bit_t.bit4 = 1;break;
        case 0x00a2: tmpBuf[1].bit_t.bit5 = 1;break;
        case 0x00e9: tmpBuf[1].bit_t.bit6 = 1;break;
        case 0x00ea: tmpBuf[1].bit_t.bit7 = 1;break;

        case 0x0183: tmpBuf[2].bit_t.bit0 = 1;break;
        case 0x0194: tmpBuf[2].bit_t.bit1 = 1;break;
        case 0x0186: tmpBuf[2].bit_t.bit2 = 1;break;
        case 0x0188: tmpBuf[2].bit_t.bit3 = 1;break;
        case 0x018a: tmpBuf[2].bit_t.bit4 = 1;break;
        case 0x0192: tmpBuf[2].bit_t.bit5 = 1;break;
        case 0x02a8: tmpBuf[2].bit_t.bit6 = 1;break;
        case 0x0184: tmpBuf[2].bit_t.bit7 = 1;break;
        default: break;
    }

    App_Usb_Keyboard_Press_Handler(RPT_ID_CONSUMER, (uint8_t *)tmpBuf, sizeof(tmpBuf)/sizeof(u8Data_t));
}


static void App_Key_Macro_Press_Handler(mKey_t mKey )
{
    
}

static void App_Key_Other_Press_Handler(mKey_t mKey )
{
    mLight_mode_t lightMode;
    
    if(mKey.func == (uint8_t )FUNC_LIGHT_SWITCH)
    {
         lightMode = App_Mouse_Get_Light_Mode();

        lightMode++;

        if(lightMode > LIGHT_MODE_COLOURFUL_TRAILER)
        {
            lightMode = LIGHT_MODE_OFF;
        }

        App_Mouse_Set_Light_Mode(lightMode);

        App_Light_Set_Light_Effect(lightMode);

    }
    else if(mKey.func == (uint8_t )FUNC_LIGHT_ON_OFF)
    {
        if(App_Light_Get_Off_Flag() == LIGHT_ON)
        {
            App_Light_Set_Off_Flag(LIGHT_OFF);

            App_Light_Set_Light_Effect(LIGHT_MODE_OFF);
        }
        else
        {
            App_Light_Set_Off_Flag(LIGHT_ON);
            
            lightMode = App_Mouse_Get_Light_Mode();

            App_Light_Set_Light_Effect(lightMode);
        }
    }
    else if(mKey.func == (uint8_t )FUNC_KEY_MODE_SWITCH)
    {
        mKey_mode_t curKeyMode = App_Mouse_Get_Cur_Key_Mode();
        
        curKeyMode++;

        if(curKeyMode > KEY_MODE_GAME)
        {
            curKeyMode = KEY_MODE_OFFICE;
        }
        
        App_Mouse_Set_Key_Mode_Data(curKeyMode);
        
        App_Mouse_Set_Cur_Key_Mode(curKeyMode);

        App_Usb_Mouse_Evt_Input(0x03, curKeyMode);
        
    }
    else if(mKey.func == (uint8_t )FUNC_KEY_FIRE)
    {
        
    }
    else if(mKey.func == (uint8_t )FUNC_RATE_SWITCH)
    {
        mRate_t rate = App_Mouse_Get_Rate();

        rate++;

        if(rate > RATE_1000HZ)
        {
            rate = RATE_125HZ;
        }

        App_Mouse_Set_Rate(rate);

        App_Sensor_Set_Detect_Time(rate);

        App_Usb_Mouse_Evt_Input(0x02, rate);
    }
}

static void App_Key_Mouse_Release_Handler(mKey_t mKey )
{    
    switch(mKey.func)
    {
        case FUNC_MOUSE_LEFT: mKeyVal.bit_t.bit0 = 0;break;
        case FUNC_MOUSE_RIGHT: mKeyVal.bit_t.bit1 = 0; break;
        case FUNC_MOUSE_MIDDLE: mKeyVal.bit_t.bit2 = 0; break;
        case FUNC_MOUSE_FORWARD: mKeyVal.bit_t.bit3 = 0; break;
        case FUNC_MOUSE_BACKWARD: mKeyVal.bit_t.bit4 = 0; break;
        default: break;
    }

    App_Usb_Mouse_Relase_Handler(mKeyVal.u8Data);
}

static void App_Key_Keyboard_Release_Handler(mKey_t mKey )
{
    App_Usb_Keyboard_Release_Handler(RPT_ID_KEYBOARD);
}

static void App_Key_Multimedia_Release_Handler(mKey_t mKey )
{
    App_Usb_Keyboard_Release_Handler(RPT_ID_CONSUMER);
}

static void App_Key_Dpi_Release_Handler(mKey_t mKey )
{
    
}

static void App_Key_Macro_Release_Handler(mKey_t mKey )
{
    
}

static void App_Key_Other_Release_Handler(mKey_t mKey )
{
    
}



