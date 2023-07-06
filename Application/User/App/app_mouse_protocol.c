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
mouse_ctrl_block_t mouseCtrl;

void App_Mouse_Para_Init(void )
{
    Drv_Flash_Read(MOUSE_PARA_START_ADDR, (uint8_t *)&mouseCtrl, sizeof(mouse_ctrl_block_t));

    if(mouseCtrl.keyMode == 0 || mouseCtrl.keyMode > 3)
    {
        mouseCtrl.keyMode = KEY_MODE_OFFICE;

        mouseCtrl.mSensor = PMW3325;

        mouseCtrl.mRate = RATE_1000HZ;

        mouseCtrl.mLightMode = LIGHT_MODE_COLOR_STREAM;

        mouseCtrl.dpiIndex = 1;

        mouseCtrl.dpiTotalNum = 6;

        mouseCtrl.dpiValBuf[0] = 0x16;        //1000
        mouseCtrl.dpiValBuf[1] = 0x2e;        //2000
        mouseCtrl.dpiValBuf[2] = 0x45;        //3000
        mouseCtrl.dpiValBuf[3] = 0x73;        //5000
        mouseCtrl.dpiValBuf[4] = 0x50 | 0x80; //7000
        mouseCtrl.dpiValBuf[5] = 0x73 | 0x80; //10000

        mouseCtrl.picIndex = 0;

        App_Mouse_Dpi_Color_Init();

        App_Mouse_Light_Init();
        
        App_Mouse_Mode_Office_init();

        App_Mouse_Mode_Multimedia_init();

        App_Mouse_Mode_Game_init();

        App_Mouse_Para_Save();
    }
}

static void App_Mouse_Mode_Office_init(void )
{
    mouseCtrl.keyModeOffice[0].name = KEY_NAME_MOUSE;
    mouseCtrl.keyModeOffice[0].func = 0xf0;
    mouseCtrl.keyModeOffice[0].val_l = 0;
    mouseCtrl.keyModeOffice[0].val_h = 0;

    mouseCtrl.keyModeOffice[1].name = KEY_NAME_MOUSE;
    mouseCtrl.keyModeOffice[1].func = 0xf1;
    mouseCtrl.keyModeOffice[1].val_l = 0;
    mouseCtrl.keyModeOffice[1].val_h = 0;

    mouseCtrl.keyModeOffice[2].name = KEY_NAME_MOUSE;
    mouseCtrl.keyModeOffice[2].func = 0xf2;
    mouseCtrl.keyModeOffice[2].val_l = 0;
    mouseCtrl.keyModeOffice[2].val_h = 0;

    mouseCtrl.keyModeOffice[3].name = KEY_NAME_MOUSE;
    mouseCtrl.keyModeOffice[3].func = 0xf3;
    mouseCtrl.keyModeOffice[3].val_l = 0;
    mouseCtrl.keyModeOffice[3].val_h = 0;

    mouseCtrl.keyModeOffice[4].name = KEY_NAME_MOUSE;
    mouseCtrl.keyModeOffice[4].func = 0xf4;
    mouseCtrl.keyModeOffice[4].val_l = 0;
    mouseCtrl.keyModeOffice[4].val_h = 0;

    mouseCtrl.keyModeOffice[5].name = KEY_NAME_DPI;
    mouseCtrl.keyModeOffice[5].func = 0x01;
    mouseCtrl.keyModeOffice[5].val_l = 0;
    mouseCtrl.keyModeOffice[5].val_h = 0;

    mouseCtrl.keyModeOffice[6].name = KEY_NAME_DPI;
    mouseCtrl.keyModeOffice[6].func = 0x02;
    mouseCtrl.keyModeOffice[6].val_l = 0;
    mouseCtrl.keyModeOffice[6].val_h = 0;

    mouseCtrl.keyModeOffice[7].name = KEY_NAME_OTHER;
    mouseCtrl.keyModeOffice[7].func = 0x04;
    mouseCtrl.keyModeOffice[7].val_l = 0;
    mouseCtrl.keyModeOffice[7].val_h = 50;

    mouseCtrl.keyModeOffice[8].name = KEY_NAME_OTHER;
    mouseCtrl.keyModeOffice[8].func = 0x05;
    mouseCtrl.keyModeOffice[8].val_l = 0;
    mouseCtrl.keyModeOffice[8].val_h = 0;
}

static void App_Mouse_Mode_Multimedia_init(void )
{
    mouseCtrl.keyModeMultimedia[0].name = KEY_NAME_MOUSE;
    mouseCtrl.keyModeMultimedia[0].func = 0xf0;
    mouseCtrl.keyModeMultimedia[0].val_l = 0;
    mouseCtrl.keyModeMultimedia[0].val_h = 0;

    mouseCtrl.keyModeMultimedia[1].name = KEY_NAME_MOUSE;
    mouseCtrl.keyModeMultimedia[1].func = 0xf1;
    mouseCtrl.keyModeMultimedia[1].val_l = 0;
    mouseCtrl.keyModeMultimedia[1].val_h = 0;

    mouseCtrl.keyModeMultimedia[2].name = KEY_NAME_MOUSE;
    mouseCtrl.keyModeMultimedia[2].func = 0xf2;
    mouseCtrl.keyModeMultimedia[2].val_l = 0;
    mouseCtrl.keyModeMultimedia[2].val_h = 0;

    mouseCtrl.keyModeMultimedia[3].name = KEY_NAME_MULTIMEDIA;
    mouseCtrl.keyModeMultimedia[3].func = 0x00;
    mouseCtrl.keyModeMultimedia[3].val_l = 0xb6;
    mouseCtrl.keyModeMultimedia[3].val_h = 0x00;

    mouseCtrl.keyModeMultimedia[4].name = KEY_NAME_MULTIMEDIA;
    mouseCtrl.keyModeMultimedia[4].func = 0x00;
    mouseCtrl.keyModeMultimedia[4].val_l = 0xb5;
    mouseCtrl.keyModeMultimedia[4].val_h = 0;

    mouseCtrl.keyModeMultimedia[5].name = KEY_NAME_MULTIMEDIA;
    mouseCtrl.keyModeMultimedia[5].func = 0x00;
    mouseCtrl.keyModeMultimedia[5].val_l = 0xe9;
    mouseCtrl.keyModeMultimedia[5].val_h = 0x00;

    mouseCtrl.keyModeMultimedia[6].name = KEY_NAME_MULTIMEDIA;
    mouseCtrl.keyModeMultimedia[6].func = 0x00;
    mouseCtrl.keyModeMultimedia[6].val_l = 0xea;
    mouseCtrl.keyModeMultimedia[6].val_h = 0x00;

    mouseCtrl.keyModeMultimedia[7].name = KEY_NAME_MULTIMEDIA;
    mouseCtrl.keyModeMultimedia[7].func = 0x00;
    mouseCtrl.keyModeMultimedia[7].val_l = 0xcd;
    mouseCtrl.keyModeMultimedia[7].val_h = 0x00;
}

static void App_Mouse_Mode_Game_init(void )
{
    mouseCtrl.keyModeGame[0].name = KEY_NAME_MOUSE;
    mouseCtrl.keyModeGame[0].func = 0xf0;
    mouseCtrl.keyModeGame[0].val_l = 0;
    mouseCtrl.keyModeGame[0].val_h = 0;

    mouseCtrl.keyModeGame[1].name = KEY_NAME_MOUSE;
    mouseCtrl.keyModeGame[1].func = 0xf1;
    mouseCtrl.keyModeGame[1].val_l = 0;
    mouseCtrl.keyModeGame[1].val_h = 0;

    mouseCtrl.keyModeGame[2].name = KEY_NAME_MOUSE;
    mouseCtrl.keyModeGame[2].func = 0xf2;
    mouseCtrl.keyModeGame[2].val_l = 0;
    mouseCtrl.keyModeGame[2].val_h = 0;

    mouseCtrl.keyModeGame[3].name = KEY_NAME_MOUSE;
    mouseCtrl.keyModeGame[3].func = 0xf3;
    mouseCtrl.keyModeGame[3].val_l = 0;
    mouseCtrl.keyModeGame[3].val_h = 0;

    mouseCtrl.keyModeGame[4].name = KEY_NAME_MOUSE;
    mouseCtrl.keyModeGame[4].func = 0xf4;
    mouseCtrl.keyModeGame[4].val_l = 0;
    mouseCtrl.keyModeGame[4].val_h = 0;

    mouseCtrl.keyModeGame[5].name = KEY_NAME_DPI;
    mouseCtrl.keyModeGame[5].func = 0x01;
    mouseCtrl.keyModeGame[5].val_l = 0;
    mouseCtrl.keyModeGame[5].val_h = 0;

    mouseCtrl.keyModeGame[6].name = KEY_NAME_DPI;
    mouseCtrl.keyModeGame[6].func = 0x02;
    mouseCtrl.keyModeGame[6].val_l = 0;
    mouseCtrl.keyModeGame[6].val_h = 0;

    mouseCtrl.keyModeGame[7].name = KEY_NAME_OTHER;
    mouseCtrl.keyModeGame[7].func = 0x04;
    mouseCtrl.keyModeGame[7].val_l = 0;
    mouseCtrl.keyModeGame[7].val_h = 0;
}

void App_Mouse_Set_Default_Color(uint8_t index, mColor_t *mColor )
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

    mColor_t mColor;
    
    for(i=0;i<mouseCtrl.dpiTotalNum;i++)
    {
        App_Mouse_Set_Default_Color(i, &mColor);

        mouseCtrl.dpiColorBuf[i] = mColor;
    }
}

static void App_Mouse_Light_Init(void )
{
    uint8_t i,j;
    mColor_t mColor;

    for(i=0;i<sizeof(mouseCtrl.mLightBuf)/sizeof(mLight_data_t);i++)
    {
        mouseCtrl.mLightBuf[i].brightness = 2;
        mouseCtrl.mLightBuf[i].speed = 1;
        mouseCtrl.mLightBuf[i].direction = 1;

        for(j=0;j<7;j++)
        {
            App_Mouse_Set_Default_Color(j, &mColor);

            mouseCtrl.mLightBuf[i].lightColorBuf[j] = mColor;
        }
    }
}

void App_Mouse_Para_Save(void )
{
    Drv_Flash_Erase(MOUSE_PARA_START_ADDR, MOUSE_PARA_SIZE);

    Drv_Flash_Write(MOUSE_PARA_START_ADDR, (uint8_t *)&mouseCtrl, sizeof(mouse_ctrl_block_t));
}

