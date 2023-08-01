/********************************************************
* @file       app_usb.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_usb.h"
#include "app_event.h"
#include "app_mouse_protocol.h"
#include "app_lcd.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Usb_Set_Report(uint8_t *buf, uint8_t len );
static void App_Usb_Get_Report(uint8_t *buf, uint8_t len );
static void App_Usb_Handler(void *arg );
/* Private variables ------------------------------------*/
static usb_report_callback_t usbReportCallback = 
{
    .usb_set_report_callback = App_Usb_Set_Report,
    .usb_get_report_callback = App_Usb_Get_Report
};


usb_para_t usbPara;


void App_Usb_Init(void )
{
    Drv_Usb_Init(&usbReportCallback);

    Drv_Task_Regist_Period(App_Usb_Handler, 0, 1, NULL);
}

static void App_Usb_Set_Report(uint8_t *buf, uint8_t len )
{
    uint8_t reportID = buf[0];
    
    switch(reportID)
    {
        case RPT_ID_KEY_REUSE:
        {
            App_Mouse_Set_Key_Reuse(buf, len);            
            break;
        }
        case RPT_ID_LDR:
        {
            App_Mouse_Set_Light_Dpi_Report(buf, len);
            break;
        }
        case RPT_ID_KEY_MODE:
        {
            App_Mouse_Set_Key_Mode(buf, len);
            break;
        }
        case RPT_ID_LGT_EFECT:
        {
            App_Mouse_Set_Light_Data(buf, len);
            break;
        }
        case RPT_ID_MACRO:
        {
            App_Mouse_Set_Macro_Data(buf, len);
            break;
        }
        case RPT_ID_PIC:
        {
            App_Lcd_Pic_Show_Disable();
            
            App_Mouse_Set_Pic_Data(buf, len);
            
            App_Usb_Pic_Out_Enable();
            
            //Drv_Msg_Put(APP_EVENT_PIC_WRITE, (uint8_t *)buf, len);
            break;
        }
        default: break;
    }
}

static void App_Usb_Get_Report(uint8_t *buf, uint8_t len )
{
    uint8_t reportID = buf[0];

    switch(reportID)
    {
        case RPT_ID_LDR:
        {
            App_Mouse_Get_Light_Dpi_Report(buf, len);
            break;
        }
        case RPT_ID_KEY_MODE:
        {
            App_Mouse_Get_Key_Mode(buf, len);
            break;
        }
        default: break;
    }
}

void App_Usb_Mouse_Press_Handler(uint8_t mKeyVal )
{
    usbPara.mDataBuf[0].u8Data = RPT_ID_MOUSE;
    
    usbPara.mDataBuf[1].u8Data = mKeyVal;

    usbPara.mDataBuf[2].u8Data = 0;
    usbPara.mDataBuf[3].u8Data = 0;
    usbPara.mDataBuf[4].u8Data = 0;
    usbPara.mDataBuf[5].u8Data = 0;
    usbPara.mDataBuf[6].u8Data = 0;

    usbPara.mDataLen = 7;

    usbPara.mDataUpdateFlag = 1;
}

void App_Usb_Keyboard_Press_Handler(uint8_t rptID, uint8_t *buf, uint8_t len )
{
    uint8_t i;

    for(i=0;i<sizeof(usbPara.kDataBuf);i++)
    {
        usbPara.kDataBuf[i].u8Data = 0x0;
    }
    
    usbPara.kDataBuf[0].u8Data = rptID;

    for(i=0;i<len;i++)
    {
        usbPara.kDataBuf[i+1].u8Data = buf[i];
    }

    if(rptID == RPT_ID_KEYBOARD)
    {
        usbPara.kDataLen = 8;
    }
    else if(rptID == RPT_ID_CONSUMER)
    {
        usbPara.kDataLen = 4;
    }
    
    usbPara.kDataUpdateFlag = 1;
}

void App_Usb_Mouse_Relase_Handler(uint8_t mKeyVal )
{
    usbPara.mDataBuf[0].u8Data = RPT_ID_MOUSE;
    usbPara.mDataBuf[1].u8Data = mKeyVal;
    usbPara.mDataBuf[2].u8Data = 0;
    usbPara.mDataBuf[3].u8Data = 0;
    usbPara.mDataBuf[4].u8Data = 0;
    usbPara.mDataBuf[5].u8Data = 0;
    usbPara.mDataBuf[6].u8Data = 0;

    usbPara.mDataLen = 7;

    usbPara.mDataUpdateFlag = 1;
}

void App_Usb_Keyboard_Release_Handler(uint8_t rptID )
{
    uint8_t i;

    for(i=0;i<sizeof(usbPara.kDataBuf);i++)
    {
        usbPara.kDataBuf[i].u8Data = 0x0;
    }
    
    usbPara.kDataBuf[0].u8Data = RPT_ID_KEYBOARD;

    if(rptID == RPT_ID_KEYBOARD)
    {
        usbPara.kDataLen = 8;
    }
    else if(rptID == RPT_ID_CONSUMER)
    {
        usbPara.kDataLen = 4;
    }
    
    usbPara.kDataUpdateFlag = 1;
}

void App_Usb_Mouse_Wheel_Handler(mouse_wheel_direction mWheelDirection )
{
    usbPara.mDataBuf[0].u8Data = RPT_ID_MOUSE;

    usbPara.mDataBuf[2].u8Data = 0;
    usbPara.mDataBuf[3].u8Data = 0;
    usbPara.mDataBuf[4].u8Data = 0;
    usbPara.mDataBuf[5].u8Data = 0;

    if(mWheelDirection == MOUSE_WHEEL_UP)
    {
        usbPara.mDataBuf[6].u8Data = 0x01;
    }
    else if(mWheelDirection == MOUSE_WHEEL_DOWM)
    {
        usbPara.mDataBuf[6].u8Data = 0xff;
    }
    
    usbPara.mDataLen = 7;

    usbPara.mDataUpdateFlag = 1;
}

void App_Usb_Mouse_Motion_Handler(int16_t x, int16_t y )
{
    usbPara.mDataBuf[0].u8Data = RPT_ID_MOUSE;

    usbPara.mDataBuf[2].u8Data = (uint8_t )x;
    usbPara.mDataBuf[3].u8Data = (uint8_t )(x >> 8);
    usbPara.mDataBuf[4].u8Data = (uint8_t )y;
    usbPara.mDataBuf[5].u8Data = (uint8_t )(y >> 8);
    
    usbPara.mDataBuf[6].u8Data = 0;

    usbPara.mDataLen = 7;

    usbPara.mDataUpdateFlag = 1;
}

void App_Usb_Mouse_Evt_Input(uint8_t evtID, uint8_t evtVal )
{
    usbPara.kDataBuf[0].u8Data = RPT_ID_DPI;
    usbPara.kDataBuf[1].u8Data = evtID;
    usbPara.kDataBuf[2].u8Data = evtVal;

    usbPara.kDataLen = 3;
    usbPara.kDataUpdateFlag = 1;
}

void App_Usb_Clr_Mouse_Key_Send_Flag(void )
{
    Drv_Usb_Clr_Ep_In_Flag(EP2);
}

uint8_t App_Usb_Get_Mouse_Key_Send_Flag(void )
{
    return Drv_Usb_Get_Ep_In_Flag(EP2);
}

void App_Usb_Pic_Out_Enable(void )
{
    Drv_Usb_Ep4_Out_Enable();
}

static void App_Usb_Handler(void *arg )
{
    if(usbPara.mDataUpdateFlag)
    {
        Drv_Usb_Ep_In(EP2, (uint8_t *)usbPara.mDataBuf, usbPara.mDataLen);

        usbPara.mDataUpdateFlag = 0;
    }

    if(usbPara.kDataUpdateFlag)
    {
        Drv_Usb_Ep_In(EP3, (uint8_t *)usbPara.kDataBuf, usbPara.kDataLen);
        
        usbPara.kDataUpdateFlag = 0;
    }
}

