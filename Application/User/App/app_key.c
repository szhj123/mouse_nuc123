/********************************************************
* @file       app_key.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_key.h"
#include "app_event.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Key_MsgPut(uint8_t *msgBuf, uint8_t msgLen );
/* Private variables ------------------------------------*/

void App_Key_Init(void )
{
    Drv_Key_Init();

    Drv_Key_Regist_MsgPut_Callback(App_Key_MsgPut );
}

static void App_Key_MsgPut(uint8_t *msgBuf, uint8_t msgLen )
{
    Drv_Msg_Put(APP_EVENT_KEY, msgBuf, msgLen);
}


