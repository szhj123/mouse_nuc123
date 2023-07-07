/********************************************************
* @file       app_event.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_event.h"
#include "app_mouse_protocol.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Event_Handler(void *arg );
/* Private variables ------------------------------------*/

void App_Event_Init(void )
{
    Drv_Task_Regist_Period(App_Event_Handler, 0, 1, NULL);    
}

static void App_Event_Handler(void *arg )
{
    static msg_t msg;

    if(Drv_Msg_Get(&msg) != MSG_OK)
    {
        return ;
    }

    switch((app_event_t )msg.msgID)
    {
        case APP_EVENT_USB_SET_REPORT:
        {
            switch(msg.msgBuf[0])
            {
                case RPT_ID_KEY_MODE:
                {
                    App_Mouse_Set_Key_Mode(msg.msgBuf, msg.msgLen);
                    break;
                }
            }
            break;
        }
        case APP_EVENT_USB_GET_REPORT:
        {
            break;
        }
        default: break;
    }
}

