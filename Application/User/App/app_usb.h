#ifndef _APP_USB_H
#define _APP_USB_H

#include "app_mouse_protocol.h"

#include "drv_task.h"
#include "drv_usb.h"
#include "drv_usb_descriptor.h"

typedef enum _mouse_wheel_direction
{
    MOUSE_WHEEL_UP = 0,
    MOUSE_WHEEL_DOWM
}mouse_wheel_direction;

typedef struct _usb_para_t
{
    u8Data_t mDataBuf[7];
    u8Data_t kDataBuf[8];

    uint8_t  mDataUpdateFlag;
    uint8_t  kDataUpdateFlag;
    uint8_t  wheelFlag;
}usb_para_t;


void App_Usb_Init(void );
void App_Usb_Mouse_Press_Handler(mKey_t mKey );
void App_Usb_Mouse_Relase_Handler(mKey_t mKey );
void App_Usb_Mouse_Wheel_Handler(mouse_wheel_direction mWheelDirection );
void App_Usb_Mouse_Input(uint8_t *buf, uint8_t len );

#endif 

