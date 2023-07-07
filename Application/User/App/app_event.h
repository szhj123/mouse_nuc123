#ifndef _APP_EVENT_H
#define _APP_EVENT_H

#include "drv_task.h"
#include "drv_event.h"
#include "drv_usb_descriptor.h"

typedef enum _app_event_t
{
    APP_EVENT_USB_SET_REPORT = 0,
    APP_EVENT_USB_GET_REPORT
}app_event_t;

void App_Event_Init(void );


#endif 

