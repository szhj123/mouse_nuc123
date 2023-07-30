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
#include "app_key.h"

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
        case APP_EVENT_KEY:
        {
            App_Key_Handler(msg.msgBuf, msg.msgLen);
            
            break;
        }
        default: break;
    }
}

