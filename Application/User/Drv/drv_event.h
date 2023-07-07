#ifndef _DRV_MSG_H
#define _DRV_MSG_H

#include "hal_nuc123.h"

#define MSG_MAX_SIZE                      64
#define MSG_QUEUE_MAX_SIZE                8

#define MSG_OK                            0
#define MSG_ERR                           (!MSG_OK)

typedef struct _msg_t
{
    uint8_t msgID;
    uint8_t msgBuf[MSG_MAX_SIZE];
    uint8_t msgLen;
}msg_t;

typedef struct _msg_queue_t
{
    msg_t    qBuf[MSG_QUEUE_MAX_SIZE];
    uint8_t  front;
    uint8_t  rear;
}msg_queue_t;

void Drv_Msg_Put(uint8_t msgID, uint8_t *msgBuf, uint8_t msgLen );
uint8_t Drv_Msg_Get(msg_t *msg );


#endif 

