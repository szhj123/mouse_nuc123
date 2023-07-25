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
            App_Mouse_Set_Light_Effect(buf, len);
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

void App_Usb_Mouse_Press_Handler(mKey_t mKey )
{
    usbPara.mDataBuf[0].u8Data = RPT_ID_MOUSE;
    
    switch(mKey.func)
    {
        case FUNC_MOUSE_LEFT: usbPara.mDataBuf[1].bit_t.bit0 = 1;break;
        case FUNC_MOUSE_RIGHT: usbPara.mDataBuf[1].bit_t.bit1 = 1; break;
        case FUNC_MOUSE_MIDDLE: usbPara.mDataBuf[1].bit_t.bit2 = 1; break;
        case FUNC_MOUSE_FORWARD: usbPara.mDataBuf[1].bit_t.bit3 = 1; break;
        case FUNC_MOUSE_BACKWARD: usbPara.mDataBuf[1].bit_t.bit4 = 1; break;
        default: break;
    }
    
    usbPara.mDataBuf[6].u8Data = 0;

    usbPara.mDataUpdateFlag = 1;
}

void App_Usb_Mouse_Relase_Handler(mKey_t mKey )
{
    usbPara.mDataBuf[0].u8Data = RPT_ID_MOUSE;
    
    switch(mKey.func)
    {
        case FUNC_MOUSE_LEFT: usbPara.mDataBuf[1].bit_t.bit0 = 0;break;
        case FUNC_MOUSE_RIGHT: usbPara.mDataBuf[1].bit_t.bit1 = 0; break;
        case FUNC_MOUSE_MIDDLE: usbPara.mDataBuf[1].bit_t.bit2 = 0; break;
        case FUNC_MOUSE_FORWARD: usbPara.mDataBuf[1].bit_t.bit3 = 0; break;
        case FUNC_MOUSE_BACKWARD: usbPara.mDataBuf[1].bit_t.bit4 = 0; break;
        default: break;
    }

    usbPara.mDataBuf[6].u8Data = 0;

    usbPara.mDataUpdateFlag = 1;
}

void App_Usb_Mouse_Wheel_Handler(mouse_wheel_direction mWheelDirection )
{
    usbPara.mDataBuf[0].u8Data = RPT_ID_MOUSE;

    if(mWheelDirection == MOUSE_WHEEL_UP)
    {
        usbPara.mDataBuf[6].u8Data = 0x01;
    }
    else if(mWheelDirection == MOUSE_WHEEL_DOWM)
    {
        usbPara.mDataBuf[6].u8Data = 0xff;
    }

    usbPara.mDataUpdateFlag = 1;
}

static void App_Usb_Handler(void *arg )
{
    if(usbPara.mDataUpdateFlag)
    {
        Drv_Usb_Ep_In(EP2, (uint8_t *)usbPara.mDataBuf, sizeof(usbPara.mDataBuf));

        usbPara.mDataUpdateFlag = 0;
    }
}

