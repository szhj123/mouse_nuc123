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
#include "app_mouse_sensor.h"
#include "app_lcd.h"
#include "app_light.h"
#include "app_flash.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
#define UPG_FW_NAK                0x0
#define UPG_FW_ACK                0x1
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Usb_Set_Report(uint8_t *buf, uint8_t len );
static void App_Usb_Get_Report(uint8_t *buf, uint8_t len );
static void App_Usb_Handler(void *arg );
static void Bootloader_Run(void *arg );
/* Private variables ------------------------------------*/
static usb_report_callback_t appUsbCallback = 
{
    .usb_set_report_callback = App_Usb_Set_Report,
    .usb_get_report_callback = App_Usb_Get_Report,
};
    
usb_para_t usbPara;

void App_Usb_Init(void )
{
    Drv_Usb_Init(&appUsbCallback);

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
        case RPT_ID_UPG_FW_SIZE:
        {
            uint16_t fwSize = (uint16_t )buf[2] << 8 | buf[1];
            
            App_Flash_Set_Fw_Size(fwSize);
            
            App_Flash_Fw_Erase(fwSize);
            
            usbPara.fwAck = 1;
            
            break;
        }
        case RPT_ID_UPG_FW_DATA:
        {
            uint16_t offset = (uint16_t )buf[2] << 8 | buf[1];
            uint8_t length = buf[3];

            App_Flash_Write_Fw_Data(offset, &buf[4], length);
            
            usbPara.fwAck = 1;
            
            break;
        }
        case RPT_ID_UPG_FW_CHECKSUM:
        {
            uint16_t recvCrc = (uint16_t )buf[2] << 8 | buf[1];
            uint16_t calCrc = App_Flash_Get_Fw_Checksum(App_Flash_Get_Fw_Size());
            
            if(recvCrc == calCrc)
            {
                App_Flash_Fw_Upg_Enable();

                App_Flash_Fw_Info_Save();
                
                Drv_Task_Regist_Oneshot(Bootloader_Run, 500, NULL);
            }
            
            usbPara.fwAck = 1;
            
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
        case RPT_ID_UPG_FW_ACK:
        {
            App_Usb_Get_Fw_Ack(buf, len);

            break;
        }
        case RPT_ID_UPG_FW_VER:
        {
            App_Usb_Set_Fw_Ver(buf, len);
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
    if(Drv_Usb_Get_Config_Val())
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

    
    switch(usbPara.mSleepStat)
    {
        case MOUSE_SLEEP_IN:
        {
            if(Drv_Usb_Get_Config_Val())
            {
                if(Drv_Usb_Get_Suspend_Flag())
                {            
                    Drv_Usb_Clr_Gpio_Wakeup_Flag();
                    
        		    App_Usb_Suspend_Handler();    

                    usbPara.mSleepStat = MOUSE_WAKEUP;
                }
            }
            break;
        }
        case MOUSE_WAKEUP:
        {
            if(Drv_Usb_Get_Gpio_Wakeup_Flag())
            {
                Drv_Usb_Wakeup();

                usbPara.delayCnt = 0;
                
                usbPara.mSleepStat = MOUSE_SLEEP_OUT;
            }
            break;
        }
        case MOUSE_SLEEP_OUT:
        {
            if(++usbPara.delayCnt > 1000)
            {
                usbPara.delayCnt = 0;
                
                if(Drv_Usb_Get_Resume_Flag())
                {
                    Drv_Usb_Clr_Resume_Flag();

                    App_Usb_Resume_Handler();

                    Drv_Usb_Clr_Gpio_Wakeup_Flag();
                    
                    usbPara.mSleepStat = MOUSE_SLEEP_IN;
                }
                else
                {
                    usbPara.mSleepStat = MOUSE_WAKEUP;
                }
            }
            break;
        }
        default: break;
    }
}

void App_Usb_Wakeup_Host(void )
{
    Drv_Usb_Wakeup();
}

void App_Usb_Suspend_Handler(void )
{
    App_Light_Set_Light_Effect(LIGHT_MODE_OFF);

    App_Lcd_Sleep();

    App_Sensor_Sleep();

    Power_Down();
}

void App_Usb_Resume_Handler(void )
{
    App_Sensor_Wakeup();
    
    App_Light_Set_Light_Effect(App_Mouse_Get_Light_Mode());
    
    App_Lcd_Wakeup();
}

void App_Usb_Get_Fw_Ack(uint8_t *buf, uint8_t len )
{
    buf[0] = RPT_ID_UPG_FW_ACK;
    
    if(usbPara.fwAck)
    {
        buf[1] = UPG_FW_ACK;
    }
    else
    {
        buf[1] = UPG_FW_NAK;
    }

    usbPara.fwAck = 0;
}

void App_Usb_Set_Fw_Ver(uint8_t *buf, uint8_t len )
{
    buf[0] = RPT_ID_UPG_FW_VER;

    buf[1] = FW_VER_BUILD;

    buf[2] = FW_VER_MINOR;

    buf[3] = FW_VER_MAJOR;
}

void Bootloader_Run(void *arg )
{
    Aprom_To_Ldrom();
}


