#ifndef _APP_MOUSE_PROTOCOL_H
#define _APP_MOUSE_PROTOCOL_H

#include "drv_flash.h"
#include "hal_nuc123.h"

typedef struct _mColor_t
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}mColor_t;

typedef enum _mKey_mode_t
{
    KEY_MODE_OFFICE = 0x01,
    KEY_MODE_MULTIMEDIA = 0x02,
    KEY_MODE_GAME = 0x03
}mKey_mode_t;

typedef enum _mKey_name_t
{
    KEY_NAME_MOUSE = 0x01,
    KEY_NAME_KEYBOARD = 0x02,
    KEY_NAME_MULTIMEDIA = 0x04,
    KEY_NAME_DPI = 0x05,
    KEY_NAME_MACRO = 0x06,
    KEY_NAME_OTHER = 0x07,
}mKey_name_t;

typedef enum _mouse_func_t
{
    FUNC_MOUSE_LEFT = 0xf0,
    FUNC_MOUSE_RIGHT = 0xf1,
    FUNC_MOUSE_MIDDLE = 0xf2,
    FUNC_MOUSE_FORWARD = 0xf3,
    FUNC_MOUSE_BACKWARD = 0xf4
}mKey_mouse_func_t;

typedef enum _keyboard_func_t
{
    FUNC_KEYBOARD_L_CTRL = 0x01,
    FUNC_KEYBOARD_L_SHIFT = 0x02,
    FUNC_KEYBOARD_L_ALT = 0x04,
    FUNC_KEYBOARD_L_WIN = 0x08,
    FUNC_KEYBOARD_R_CTRL = 0x10,
    FUNC_KEYBOARD_R_SHIFT = 0x20,
    FUNC_KEYBOARD_R_ALT = 0x40,
    FUNC_KEYBOARD_R_WIN = 0x80
}keyboard_func_t;

typedef enum _mSensor_t
{
    PMW3050 = 0x01,
    PMW3325 = 0x02,
    PMW3360 = 0x03,
    PMW3330 = 0x04
}mSensor_t;

typedef enum _mRate_t
{
    RATE_125HZ = 0x01,
    RATE_250HZ = 0x02,
    RATE_500HZ = 0x03,
    RATE_1000HZ = 0x04
}mRate_t;

typedef enum _mLight_mode_t
{
    LIGHT_MODE_OFF = 0,
    LIGHT_MODE_COLOR_STREAM,
    LIGHT_MODE_SOLID,
    LIGHT_MODE_BREATH,
    LIGHT_MODE_NEON,
    LIGHT_MODE_BLINK,
    LIGHT_MODE_TRAILER,
    LIGHT_MODE_PINGPANG,
    LIGHT_MODE_COLOR_TRAILER
}mLight_mode_t;

typedef struct _mKey_value_t
{
    mKey_name_t name;
    uint8_t func;
    uint8_t val_l;
    uint8_t val_h;
}mKey_value_t;

typedef struct _mKey_pack_t
{
    uint8_t rptID;
    mKey_mode_t keyMode;
    mKey_value_t keyVal[15];
    uint8_t reserve[2];
}mKey_pack_t;

typedef struct _mLdr_pack_t
{
    uint8_t rptID;
    uint8_t mRate;
    uint8_t mSensor;
    uint8_t mLightMode;
    uint8_t reserve0;
    uint8_t dpiIndex;
    uint8_t dpiTotalNum;
    uint8_t dpiValBuf[16];
    mColor_t dpiColorBuf[8];
    uint8_t reserve1[8];
    uint8_t picShowMask_l;
    uint8_t picShowMask_h;
    uint8_t picIndex;
    uint8_t reserve2[6];
}mLdr_pack_t;

typedef struct _mLight_data_t
{
    uint8_t brightness;
    uint8_t speed;
    uint8_t direction;
    mColor_t lightColorBuf[7];
}mLight_data_t;

typedef struct _mLight_pack_t
{
    uint8_t rptID;
    uint8_t lightMode;
    mLight_data_t lightData;
    uint8_t reserve[38];
}mLight_pack_t;

typedef struct _mMacro_Val_t
{
    uint8_t delayTime_h;
    uint8_t delayTime_l;
    uint8_t hidVal;
}mMacro_Val_t;

typedef struct _mMacro_pack_t
{
    uint8_t rptID;
    uint8_t macroID;
    uint8_t macroOffsetAddr;
    uint8_t macroDataLen;
    mMacro_Val_t macroValBuf[20];
}mMacro_pack_t;

typedef struct _pic_pack_t
{
    uint8_t rptID;
    uint8_t picID;
    uint8_t picOffsetAddr;
    uint8_t picDataLen;
    uint8_t picDataBuf[60];
}pic_pack_t;

typedef struct _mouse_ctrl_block_t
{
    mKey_mode_t keyMode;
    mKey_value_t keyModeOffice[15];
    mKey_value_t keyModeMultimedia[15];
    mKey_value_t keyModeGame[15];
    mRate_t mRate;
    mSensor_t mSensor;
    mLight_mode_t mLightMode;
    uint8_t dpiIndex;
    uint8_t dpiTotalNum;
    uint8_t dpiValBuf[16];
    mColor_t dpiColorBuf[8];
    uint8_t picShowMask_l;
    uint8_t picShowMask_h;
    uint8_t picIndex;
    mLight_data_t mLightBuf[10];
}mouse_ctrl_block_t;

void App_Mouse_Para_Init(void );
void App_Mouse_Para_Save(void );


#endif 

