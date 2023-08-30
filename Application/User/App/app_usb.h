#ifndef _APP_USB_H
#define _APP_USB_H

#include "app_mouse_protocol.h"

#include "drv_task.h"
#include "drv_usb.h"
#include "drv_usb_descriptor.h"
#include "drv_flash.h"

typedef enum _mouse_wheel_direction
{
    MOUSE_WHEEL_UP = 0,
    MOUSE_WHEEL_DOWM
}mouse_wheel_direction;

typedef enum _mSleep_stat_t
{
    MOUSE_SLEEP_IN = 0,
    MOUSE_WAKEUP,
    MOUSE_SLEEP_OUT
}mSleep_stat_t;

typedef struct _usb_para_t
{
    u8Data_t mDataBuf[7];
    u8Data_t kDataBuf[8];
    uint8_t  mDataLen;
    uint8_t  kDataLen;

    uint8_t  mDataUpdateFlag;
    uint8_t  kDataUpdateFlag;
    uint8_t  wheelFlag;

    uint8_t  fwAck;
    uint16_t delayCnt;
    mSleep_stat_t mSleepStat;
}usb_para_t;


void App_Usb_Init(void );
void App_Usb_Mouse_Press_Handler(uint8_t mKeyVal );
void App_Usb_Mouse_Relase_Handler(uint8_t mKeyVal );
void App_Usb_Mouse_Wheel_Handler(mouse_wheel_direction mWheelDirection );
void App_Usb_Mouse_Motion_Handler(int16_t x, int16_t y );
void App_Usb_Mouse_Input(uint8_t *buf, uint8_t len );
void App_Usb_Mouse_Evt_Input(uint8_t evtID, uint8_t evtVal );
void App_Usb_Keyboard_Press_Handler(uint8_t rptID, uint8_t *buf, uint8_t len );
void App_Usb_Keyboard_Release_Handler(uint8_t rptID );

void App_Usb_Pic_Out_Enable(void );

void App_Usb_Clr_Mouse_Key_Send_Flag(void );
uint8_t App_Usb_Get_Mouse_Key_Send_Flag(void );

void App_Usb_Suspend_Handler(void );
void App_Usb_Resume_Handler(void );

void App_Usb_Wakeup_Host(void );

void App_Usb_Get_Fw_Ack(uint8_t *buf, uint8_t len );



#endif 

