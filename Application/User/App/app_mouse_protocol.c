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
#include "app_mouse_sensor.h"
#include "app_event.h"
#include "app_calendar.h"
#include "app_usb.h"
#include "app_light.h"
#include "app_lcd.h"


#include "hal_spi_flash.h"

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
static mKey_t KeyModeData[15];

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
    
    App_Mouse_Set_Key_Mode_Data(mousePara.keyMode);

    App_Light_Set_Light_Effect(mousePara.mLightMode);

    App_Lcd_Updae_Show_Pic_ID();
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

    for(i=0;i<sizeof(mousePara.mLightData)/sizeof(mLight_data_t);i++)
    {
        mousePara.mLightData[i].brightness = 2;
        mousePara.mLightData[i].speed = 1;
        mousePara.mLightData[i].direction = 1;

        for(j=0;j<7;j++)
        {
            App_Mouse_Set_Default_Color(j, &mColor);

            mousePara.mLightData[i].lightColorBuf[j] = mColor;
        }
    }
}

void App_Mouse_Set_Key_Mode(uint8_t *buf, uint8_t len )
{
    uint8_t modeDataFlag = buf[1];
    
    if(modeDataFlag & 0x01)
    {
        mousePara.keyMode = (mKey_mode_t )buf[2];

        App_Mouse_Set_Key_Mode_Data(mousePara.keyMode);
    }
    else
    {
        App_Calendar_Set_Date(&buf[3], sizeof(date_t));
    }
}

void App_Mouse_Set_Key_Mode_Data(mKey_mode_t keyMode )
{
    uint8_t i;
    
    for(i=0;i<15;i++)
    {
        if(keyMode == KEY_MODE_OFFICE)
        {
            KeyModeData[i] = mousePara.keyModeOffice[i];
        }
        else if(keyMode == KEY_MODE_MULTIMEDIA)
        {
            KeyModeData[i] = mousePara.keyModeMultimedia[i];
        }
        else if(keyMode == KEY_MODE_GAME)
        {
            KeyModeData[i] = mousePara.keyModeGame[i];
        }
    }
}

void App_Mouse_Get_Key_Mode(uint8_t *buf, uint8_t len )
{
    mKey_mode_pack_t *keyModePack = (mKey_mode_pack_t *)buf;
    
    keyModePack->rptID = RPT_ID_KEY_MODE;
    keyModePack->modeDateFlag = 1;
    keyModePack->keyMode = (uint8_t )App_Mouse_Get_Cur_Key_Mode();
}

void App_Mouse_Set_Key_Reuse(uint8_t *buf, uint8_t len )
{
    uint8_t i;
    mKey_pack_t *mKeyPack = (mKey_pack_t *)buf;

    for(i=0;i<15;i++)
    {
        if((mKeyPack->keyMode+1) == KEY_MODE_OFFICE)
        {
            mousePara.keyModeOffice[i] = mKeyPack->keyVal[i];    
        }
        else if((mKeyPack->keyMode+1) == KEY_MODE_MULTIMEDIA)
        {
            mousePara.keyModeMultimedia[i] = mKeyPack->keyVal[i];
        }
        else if((mKeyPack->keyMode+1) == KEY_MODE_GAME)
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

    App_Light_Set_Light_Effect(mousePara.mLightMode);
}

void App_Mouse_Set_Light_Mode(mLight_mode_t lightMode )
{   
    mousePara.mLightMode = lightMode;
}

mLight_mode_t App_Mouse_Get_Light_Mode(void )
{
    return mousePara.mLightMode;
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

void App_Mouse_Set_Light_Data(uint8_t *buf, uint8_t len )
{
    mLight_pack_t *lightPack = (mLight_pack_t *)buf;
    
    mousePara.mLightData[lightPack->lightMode] = lightPack->lightData;
}

void App_Mouse_Set_Macro_Data(uint8_t *buf, uint8_t len )
{
    uint32_t flashStartAddr;
    mMacro_pack_t *macroPack = (mMacro_pack_t *)buf;

    if(macroPack->macroID > 6)
    {
        macroPack->macroID = 6;
    }

    if(macroPack->macroID > 0)
    {
        macroPack->macroID -= 1;
    }

    flashStartAddr = MOUSE_MACRO1_START_ADDR + (uint32_t)macroPack->macroID * 512;

    if(macroPack->macroOffsetAddr == 0)
    {
        Drv_Flash_Erase(flashStartAddr, MOUSE_MACRO1_SIZE);

        Drv_Flash_Write(flashStartAddr, (uint8_t *)&macroPack->macroValBuf[0], macroPack->macroDataLen);
    }
    else
    {
        Drv_Flash_Write(flashStartAddr+macroPack->macroOffsetAddr, (uint8_t *)&macroPack->macroValBuf[0], macroPack->macroDataLen);
    }
}

void App_Mouse_Set_Pic_Data(uint8_t *buf, uint8_t len )
{
    static uint8_t lcdFlashEraseFlag;
    static uint8_t lcdPicID;
    static uint32_t lcdFlashAddr;
    
    pic_pack_t *picPack  = (pic_pack_t *)buf;

    if(picPack->picID < 1)
    {
        return ;
    }
    
    App_Lcd_Set_Rw_Stat(LCD_BUSY);

    if(picPack->picID != 0xff)
    {
        if(!lcdFlashEraseFlag)
        {
            lcdFlashEraseFlag = 1;

            lcdPicID = picPack->picID;
            
            lcdFlashAddr = LCD_PIC_MAX_SIZE * (uint32_t )(picPack->picID-1);

            Drv_Spi_Flash_Erase_64k(lcdFlashAddr);

            lcdFlashAddr += 0x10000;
            
            Drv_Spi_Flash_Erase_64k(lcdFlashAddr);

            lcdFlashAddr -=  0x10000;
        }

        Drv_Spi_Flash_Write(lcdFlashAddr, picPack->picDataBuf, picPack->picDataLen);

        lcdFlashAddr += picPack->picDataLen;
    }
    else
    {
        lcdFlashEraseFlag = 0;

        App_Mouse_Set_Pic_Show_Mask(lcdPicID-5);

        App_Lcd_Updae_Show_Pic_ID();

        App_Lcd_Set_Rw_Stat(LCD_IDLE);
    }
}

void App_Mouse_Get_Light_Color(mLight_mode_t lightMode, uint8_t colorIndex, color_t *color )
{
    color->red = mousePara.mLightData[(uint8_t )lightMode].lightColorBuf[colorIndex].red;
    color->green = mousePara.mLightData[(uint8_t )lightMode].lightColorBuf[colorIndex].green;
    color->blue = mousePara.mLightData[(uint8_t )lightMode].lightColorBuf[colorIndex].blue;
}

void App_Mouse_Get_Key_Data(uint8_t keyIndex, mKey_t *mKey )
{
    *mKey = KeyModeData[keyIndex];
}

void App_Mouse_Get_Light_Data(mLight_mode_t lightMode, mLight_data_t *lightData )
{
    *lightData = mousePara.mLightData[(uint8_t )lightMode];
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

mKey_mode_t App_Mouse_Get_Cur_Key_Mode(void )
{
    return mousePara.keyMode;
}

void App_Mouse_Set_Cur_Key_Mode(mKey_mode_t keyMode )
{
    mousePara.keyMode = keyMode;

    App_Mouse_Para_Save();
}

mRate_t App_Mouse_Get_Rate(void )
{
    return mousePara.mRate;
}

void App_Mouse_Set_Rate(mRate_t rate )
{
    mousePara.mRate = rate;

    App_Mouse_Para_Save();
}

uint16_t App_Mouse_Get_Pic_Show_Mask(void )
{
    return ((uint16_t )mousePara.picShowMask_h << 8 | mousePara.picShowMask_l);
}

void App_Mouse_Set_Pic_Show_Mask(uint8_t picID )
{
    uint16_t mask = 0xffff;

    mask &= ~((uint16_t )1 << picID);

    mousePara.picShowMask_l = (uint8_t )mask;

    mousePara.picShowMask_h = (uint8_t )(mask >>8 );

    App_Mouse_Para_Save();
}

void App_Mouse_Para_Save(void )
{
    Drv_Flash_Erase(MOUSE_PARA_START_ADDR, MOUSE_PARA_SIZE);

    Drv_Flash_Write(MOUSE_PARA_START_ADDR, (uint8_t *)&mousePara, sizeof(mouse_para_t));
}


