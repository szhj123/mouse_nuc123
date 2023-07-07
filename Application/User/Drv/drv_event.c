/********************************************************
* @file       drv_event.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_event.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
msg_queue_t msgQueue;

void Drv_Msg_Put(uint8_t msgID, uint8_t *msgBuf, uint8_t msgLen )
{
    uint8_t i;
    
    if(msgLen > MSG_MAX_SIZE)
    {
        msgLen = MSG_MAX_SIZE;
    }
    __disable_irq();
    
    msgQueue.qBuf[msgQueue.front].msgID = msgID;

    msgQueue.qBuf[msgQueue.front].msgLen = msgLen;

    for(i=0;i<msgLen;i++)
    {
        msgQueue.qBuf[msgQueue.front].msgBuf[i] = msgBuf[i];
    }

    msgQueue.front = (msgQueue.front + 1) % MSG_QUEUE_MAX_SIZE;

    __enable_irq();
}

uint8_t Drv_Msg_Get(msg_t *msg )
{
    uint8_t retVal = MSG_ERR;
    
    if(msgQueue.rear != msgQueue.front)
    {
        *msg = msgQueue.qBuf[msgQueue.rear];

        msgQueue.rear = (msgQueue.rear + 1) % MSG_QUEUE_MAX_SIZE;

        retVal = MSG_OK;
    }

    return retVal;
}

