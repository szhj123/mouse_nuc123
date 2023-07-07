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
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Usb_Msg_Set_Report(uint8_t *buf, uint8_t len );
static void App_Usb_Msg_Get_Report(uint8_t *buf, uint8_t len );
static void App_Usb_Handler(void *arg );
/* Private variables ------------------------------------*/
static usb_msg_callback_t usbMsgCallback = 
{
    .usb_msg_set_report_callback = App_Usb_Msg_Set_Report,
    .usb_msg_get_report_callback = App_Usb_Msg_Get_Report
};


void App_Usb_Init(void )
{
    Drv_Usb_Init(&usbMsgCallback);

    Drv_Task_Regist_Period(App_Usb_Handler, 0, 1, NULL);
}

static void App_Usb_Msg_Set_Report(uint8_t *buf, uint8_t len )
{
    Drv_Msg_Put(APP_EVENT_USB_SET_REPORT, buf, len);
}

static void App_Usb_Msg_Get_Report(uint8_t *buf, uint8_t len )
{
    Drv_Msg_Put(APP_EVENT_USB_GET_REPORT, buf, len);
}



static void App_Usb_Handler(void *arg )
{
    
}

