/********************************************************
* @file       app_mouse_protocol.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_mouse_protocol.h"
#include "app_calendar.h"
#include "app_usb.h"
#include "app_mouse_sensor.h"
#include "app_light.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Mouse_Mode_Office_init(void );
static void App_Mouse_Mode_Multimedia_init(void );
static void App_Mouse_Mode_Game_init(void );
static void App_Mouse_Dpi_Color_Init(void );
static void App_Mouse_Light_Init(void );
/* Private variables ------------------------------------*/
static mouse_para_t mousePara;
static mKey_t keyModeBuf[15];

void App_Mouse_Para_Init(void )
{   
    Drv_Flash_Read(MOUSE_PARA_START_ADDR, (uint8_t *)&mousePara, sizeof(mouse_para_t));

    if(mousePara.keyMode == 0 || mousePara.keyMode > 3)
    {
        mousePara.keyMode = KEY_MODE_OFFICE;

        mousePara.mSensor = PMW3325;

        mousePara.mRate = RATE_1000HZ;

        mousePara.mLightMode = LIGHT_MODE_COLOR_STREAM;

        mousePara.dpiIndex = 1;

        mousePara.dpiTotalNum = 6;

        mousePara.dpiValBuf[0] = 0x16;        //1000
        mousePara.dpiValBuf[1] = 0x2e;        //2000
        mousePara.dpiValBuf[2] = 0x45;        //3000
        mousePara.dpiValBuf[3] = 0x73;        //5000
        mousePara.dpiValBuf[4] = 0x50 | 0x80; //7000
        mousePara.dpiValBuf[5] = 0x73 | 0x80; //10000

        mousePara.picIndex = 0;

        App_Mouse_Dpi_Color_Init();

        App_Mouse_Light_Init();
        
        App_Mouse_Mode_Office_init();

        App_Mouse_Mode_Multimedia_init();

        App_Mouse_Mode_Game_init();

        App_Mouse_Para_Save();
    }

    App_Sensor_Set_Detect_Time(mousePara.mRate);

    App_Sensor_Set_Dpi(mousePara.dpiIndex);
    
    App_Mouse_Set_Key_Mode_Buf(mousePara.keyMode);

    App_Mouse_Set_Light_Mode(mousePara.mLightMode);
}

static void App_Mouse_Mode_Office_init(void )
{
    mousePara.keyModeOffice[0].name = KEY_NAME_MOUSE;
    mousePara.keyModeOffice[0].func = 0xf0;
    mousePara.keyModeOffice[0].val_l = 0;
    mousePara.keyModeOffice[0].val_h = 0;

    mousePara.keyModeOffice[1].name = KEY_NAME_MOUSE;
    mousePara.keyModeOffice[1].func = 0xf1;
    mousePara.keyModeOffice[1].val_l = 0;
    mousePara.keyModeOffice[1].val_h = 0;

    mousePara.keyModeOffice[2].name = KEY_NAME_MOUSE;
    mousePara.keyModeOffice[2].func = 0xf2;
    mousePara.keyModeOffice[2].val_l = 0;
    mousePara.keyModeOffice[2].val_h = 0;

    mousePara.keyModeOffice[3].name = KEY_NAME_MOUSE;
    mousePara.keyModeOffice[3].func = 0xf3;
    mousePara.keyModeOffice[3].val_l = 0;
    mousePara.keyModeOffice[3].val_h = 0;

    mousePara.keyModeOffice[4].name = KEY_NAME_MOUSE;
    mousePara.keyModeOffice[4].func = 0xf4;
    mousePara.keyModeOffice[4].val_l = 0;
    mousePara.keyModeOffice[4].val_h = 0;

    mousePara.keyModeOffice[5].name = KEY_NAME_DPI;
    mousePara.keyModeOffice[5].func = 0x01;
    mousePara.keyModeOffice[5].val_l = 0;
    mousePara.keyModeOffice[5].val_h = 0;

    mousePara.keyModeOffice[6].name = KEY_NAME_DPI;
    mousePara.keyModeOffice[6].func = 0x02;
    mousePara.keyModeOffice[6].val_l = 0;
    mousePara.keyModeOffice[6].val_h = 0;

    mousePara.keyModeOffice[7].name = KEY_NAME_OTHER;
    mousePara.keyModeOffice[7].func = 0x04;
    mousePara.keyModeOffice[7].val_l = 0;
    mousePara.keyModeOffice[7].val_h = 50;

    mousePara.keyModeOffice[8].name = KEY_NAME_OTHER;
    mousePara.keyModeOffice[8].func = 0x05;
    mousePara.keyModeOffice[8].val_l = 0;
    mousePara.keyModeOffice[8].val_h = 0;
}

static void App_Mouse_Mode_Multimedia_init(void )
{
    mousePara.keyModeMultimedia[0].name = KEY_NAME_MOUSE;
    mousePara.keyModeMultimedia[0].func = 0xf0;
    mousePara.keyModeMultimedia[0].val_l = 0;
    mousePara.keyModeMultimedia[0].val_h = 0;

    mousePara.keyModeMultimedia[1].name = KEY_NAME_MOUSE;
    mousePara.keyModeMultimedia[1].func = 0xf1;
    mousePara.keyModeMultimedia[1].val_l = 0;
    mousePara.keyModeMultimedia[1].val_h = 0;

    mousePara.keyModeMultimedia[2].name = KEY_NAME_MOUSE;
    mousePara.keyModeMultimedia[2].func = 0xf2;
    mousePara.keyModeMultimedia[2].val_l = 0;
    mousePara.keyModeMultimedia[2].val_h = 0;

    mousePara.keyModeMultimedia[3].name = KEY_NAME_MULTIMEDIA;
    mousePara.keyModeMultimedia[3].func = 0x00;
    mousePara.keyModeMultimedia[3].val_l = 0xb6;
    mousePara.keyModeMultimedia[3].val_h = 0x00;

    mousePara.keyModeMultimedia[4].name = KEY_NAME_MULTIMEDIA;
    mousePara.keyModeMultimedia[4].func = 0x00;
    mousePara.keyModeMultimedia[4].val_l = 0xb5;
    mousePara.keyModeMultimedia[4].val_h = 0;

    mousePara.keyModeMultimedia[5].name = KEY_NAME_MULTIMEDIA;
    mousePara.keyModeMultimedia[5].func = 0x00;
    mousePara.keyModeMultimedia[5].val_l = 0xe9;
    mousePara.keyModeMultimedia[5].val_h = 0x00;

    mousePara.keyModeMultimedia[6].name = KEY_NAME_MULTIMEDIA;
    mousePara.keyModeMultimedia[6].func = 0x00;
    mousePara.keyModeMultimedia[6].val_l = 0xea;
    mousePara.keyModeMultimedia[6].val_h = 0x00;

    mousePara.keyModeMultimedia[7].name = KEY_NAME_MULTIMEDIA;
    mousePara.keyModeMultimedia[7].func = 0x00;
    mousePara.keyModeMultimedia[7].val_l = 0xcd;
    mousePara.keyModeMultimedia[7].val_h = 0x00;
}

static void App_Mouse_Mode_Game_init(void )
{
    mousePara.keyModeGame[0].name = KEY_NAME_MOUSE;
    mousePara.keyModeGame[0].func = 0xf0;
    mousePara.keyModeGame[0].val_l = 0;
    mousePara.keyModeGame[0].val_h = 0;

    mousePara.keyModeGame[1].name = KEY_NAME_MOUSE;
    mousePara.keyModeGame[1].func = 0xf1;
    mousePara.keyModeGame[1].val_l = 0;
    mousePara.keyModeGame[1].val_h = 0;

    mousePara.keyModeGame[2].name = KEY_NAME_MOUSE;
    mousePara.keyModeGame[2].func = 0xf2;
    mousePara.keyModeGame[2].val_l = 0;
    mousePara.keyModeGame[2].val_h = 0;

    mousePara.keyModeGame[3].name = KEY_NAME_MOUSE;
    mousePara.keyModeGame[3].func = 0xf3;
    mousePara.keyModeGame[3].val_l = 0;
    mousePara.keyModeGame[3].val_h = 0;

    mousePara.keyModeGame[4].name = KEY_NAME_MOUSE;
    mousePara.keyModeGame[4].func = 0xf4;
    mousePara.keyModeGame[4].val_l = 0;
    mousePara.keyModeGame[4].val_h = 0;

    mousePara.keyModeGame[5].name = KEY_NAME_DPI;
    mousePara.keyModeGame[5].func = 0x01;
    mousePara.keyModeGame[5].val_l = 0;
    mousePara.keyModeGame[5].val_h = 0;

    mousePara.keyModeGame[6].name = KEY_NAME_DPI;
    mousePara.keyModeGame[6].func = 0x02;
    mousePara.keyModeGame[6].val_l = 0;
    mousePara.keyModeGame[6].val_h = 0;

    mousePara.keyModeGame[7].name = KEY_NAME_OTHER;
    mousePara.keyModeGame[7].func = 0x04;
    mousePara.keyModeGame[7].val_l = 0;
    mousePara.keyModeGame[7].val_h = 0;
}

void App_Mouse_Set_Default_Color(uint8_t index, color_t *mColor )
{
    switch(index)
    {
        case 0: mColor->red = 0xff;mColor->green = 0x00;mColor->blue = 0x00;break;
        case 1: mColor->red = 0x00;mColor->green = 0x00;mColor->blue = 0xff;break;
        case 2: mColor->red = 0x00;mColor->green = 0xff;mColor->blue = 0x00;break;
        case 3: mColor->red = 0x80;mColor->green = 0x00;mColor->blue = 0xff;break;
        case 4: mColor->red = 0x00;mColor->green = 0xff;mColor->blue = 0xff;break;
        case 5: mColor->red = 0xff;mColor->green = 0xff;mColor->blue = 0x00;break;
        case 6: mColor->red = 0xff;mColor->green = 0xff;mColor->blue = 0xff;break;
        case 7: mColor->red = 0xff;mColor->green = 0x80;mColor->blue = 0x00;break;
        default: mColor->red = 0x00;mColor->green = 0x00;mColor->blue = 0x00;break;
    }
}

static void App_Mouse_Dpi_Color_Init(void )
{
    uint8_t i;

    color_t mColor;
    
    for(i=0;i<mousePara.dpiTotalNum;i++)
    {
        App_Mouse_Set_Default_Color(i, &mColor);

        mousePara.dpiColorBuf[i] = mColor;
    }
}

static void App_Mouse_Light_Init(void )
{
    uint8_t i,j;
    color_t mColor;

    for(i=0;i<sizeof(mousePara.mLightBuf)/sizeof(mLight_data_t);i++)
    {
        mousePara.mLightBuf[i].brightness = 2;
        mousePara.mLightBuf[i].speed = 1;
        mousePara.mLightBuf[i].direction = 1;

        for(j=0;j<7;j++)
        {
            App_Mouse_Set_Default_Color(j, &mColor);

            mousePara.mLightBuf[i].lightColorBuf[j] = mColor;
        }
    }
}

void App_Mouse_Set_Key_Mode(uint8_t *buf, uint8_t len )
{
    uint8_t modeDataFlag = buf[1];
    
    if(modeDataFlag & 0x01)
    {
        mousePara.keyMode = (mKey_mode_t )buf[2];

        App_Mouse_Set_Key_Mode_Buf(mousePara.keyMode);
    }
    else
    {
        App_Calendar_Set_Date(&buf[3], sizeof(date_t));
    }
}

void App_Mouse_Set_Key_Mode_Buf(mKey_mode_t keyMode )
{
    uint8_t i;
    
    for(i=0;i<15;i++)
    {
        if(keyMode == KEY_MODE_OFFICE)
        {
            keyModeBuf[i] = mousePara.keyModeOffice[i];
        }
        else if(keyMode == KEY_MODE_MULTIMEDIA)
        {
            keyModeBuf[i] = mousePara.keyModeMultimedia[i];
        }
        else if(keyMode == KEY_MODE_GAME)
        {
            keyModeBuf[i] = mousePara.keyModeGame[i];
        }
    }
}

void App_Mouse_Get_Key_Mode(uint8_t *buf, uint8_t len )
{
    mKey_mode_pack_t *keyModePack = (mKey_mode_pack_t *)buf;
    
    keyModePack->rptID = RPT_ID_KEY_MODE;
    keyModePack->modeDateFlag = 1;
    keyModePack->keyMode = (uint8_t )mousePara.keyMode;
}

void App_Mouse_Set_Key_Mode_Value(uint8_t *buf, uint8_t len )
{
    uint8_t i;
    mKey_pack_t *mKeyPack = (mKey_pack_t *)buf;

    for(i=0;i<15;i++)
    {
        if(mKeyPack->keyMode == KEY_MODE_OFFICE)
        {
            mousePara.keyModeOffice[i] = mKeyPack->keyVal[i];    
        }
        else if(mKeyPack->keyMode == KEY_MODE_MULTIMEDIA)
        {
            mousePara.keyModeMultimedia[i] = mKeyPack->keyVal[i];
        }
        else if(mKeyPack->keyMode == KEY_MODE_GAME)
        {
            mousePara.keyModeGame[i] = mKeyPack->keyVal[i];
        }
    }
}

void App_Mouse_Set_Light_Dpi_Report(uint8_t *buf, uint8_t len )
{
    uint8_t i;
    mLdr_pack_t *ldrPack = (mLdr_pack_t *)buf;

    mousePara.mRate = (mRate_t )ldrPack->mRate;
    mousePara.mSensor = (mSensor_t )ldrPack->mSensor;
    mousePara.mLightMode = (mLight_mode_t )ldrPack->mLightMode;
    mousePara.dpiIndex = ldrPack->dpiIndex;
    mousePara.dpiTotalNum = ldrPack->dpiTotalNum;
    
    for(i=0;i<16;i++)
    {
        mousePara.dpiValBuf[i] = ldrPack->dpiValBuf[i];
    }

    for(i=0;i<8;i++)
    {
        mousePara.dpiColorBuf[i] = ldrPack->dpiColorBuf[i];
    }

    mousePara.picShowMask_l = ldrPack->picShowMask_l;
    mousePara.picShowMask_h = ldrPack->picShowMask_h;
    mousePara.picIndex = ldrPack->picIndex;

    App_Mouse_Para_Save();

    App_Sensor_Set_Detect_Time(mousePara.mRate);

    App_Mouse_Set_Light_Mode(mousePara.mLightMode);
}

void App_Mouse_Set_Light_Mode(mLight_mode_t lightMode )
{
    switch(lightMode)
    {
        case LIGHT_MODE_OFF: App_Light_Off(); break;
        case LIGHT_MODE_COLOR_STREAM: App_Light_Color_Streamer(mousePara.mLightBuf[1]); break;
        case LIGHT_MODE_SOLID: App_Light_Solid(mousePara.mLightBuf[2]); break;
        case LIGHT_MODE_BREATH: App_Light_Breath(mousePara.mLightBuf[3]); break;
        case LIGHT_MODE_NEON: App_Light_Neon(mousePara.mLightBuf[4]); break;
        case LIGHT_MODE_BLINK: App_Light_Blink(mousePara.mLightBuf[5]); break;
        case LIGHT_MODE_MONOCHROME_TRAILER: App_Light_MonoChrome_Trailer(mousePara.mLightBuf[6]); break;
        case LIGHT_MODE_RESPONSE: App_Light_Response(mousePara.mLightBuf[7]); break;
        case LIGHT_MODE_COLOURFUL_TRAILER: App_Light_Colourful_Trailer(mousePara.mLightBuf[8]); break;
        default: break;
    }
}

void App_Mouse_Get_Light_Dpi_Report(uint8_t *buf, uint8_t len )
{
    uint8_t i;
    mLdr_pack_t *ldrPack = (mLdr_pack_t *)buf;
    
    ldrPack->rptID = RPT_ID_LDR;
    ldrPack->mRate = mousePara.mRate;
    ldrPack->mSensor = mousePara.mSensor;
    ldrPack->mLightMode = mousePara.mLightMode;
    ldrPack->dpiIndex = mousePara.dpiIndex;
    ldrPack->dpiTotalNum = mousePara.dpiTotalNum;

    for(i=0;i<16;i++)
    {
        ldrPack->dpiValBuf[i] = mousePara.dpiValBuf[i];
    }

    for(i=0;i<8;i++)
    {
        ldrPack->dpiColorBuf[i] = mousePara.dpiColorBuf[i];
    }

    ldrPack->picShowMask_l = mousePara.picShowMask_l;
    ldrPack->picShowMask_h = mousePara.picShowMask_h;
    ldrPack->picIndex = mousePara.picIndex;
}

void App_Mouse_Set_Light_Effect(uint8_t *buf, uint8_t len )
{
    mLight_pack_t *lightPack = (mLight_pack_t *)buf;
    
    mousePara.mLightBuf[lightPack->lightMode] = lightPack->lightData;
}


void App_Mouse_Get_Light_Color(mLight_mode_t lightMode, uint8_t colorIndex, color_t *color )
{
    color->red = mousePara.mLightBuf[(uint8_t )lightMode].lightColorBuf[colorIndex].red;
    color->green = mousePara.mLightBuf[(uint8_t )lightMode].lightColorBuf[colorIndex].green;
    color->blue = mousePara.mLightBuf[(uint8_t )lightMode].lightColorBuf[colorIndex].blue;
}

void App_Mouse_Get_Key(uint8_t keyIndex, mKey_t *mKey )
{
    *mKey = keyModeBuf[keyIndex];
}

uint8_t App_Mouse_Get_Dpi_Val(uint8_t dpiIndex )
{    
    return mousePara.dpiValBuf[dpiIndex-1];
}

uint8_t App_Mouse_Get_Dpi_Index(void )
{
    return mousePara.dpiIndex;
}

uint8_t App_Mouse_Get_Dpi_Total_Num(void )
{
    return mousePara.dpiTotalNum;
}

void App_Mouse_Set_Dpi_Index(uint8_t dpiIndex )
{
    mousePara.dpiIndex = dpiIndex;
}

void App_Mouse_Get_Dpi_Color(uint8_t dpiIndex, color_t *dpiColor )
{
    *dpiColor = mousePara.dpiColorBuf[dpiIndex];
}

void App_Mouse_Para_Save(void )
{
    Drv_Flash_Erase(MOUSE_PARA_START_ADDR, MOUSE_PARA_SIZE);

    Drv_Flash_Write(MOUSE_PARA_START_ADDR, (uint8_t *)&mousePara, sizeof(mouse_para_t));
}


