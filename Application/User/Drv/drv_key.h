#ifndef _DRV_KEY_H
#define _DRV_KEY_H

#include "drv_timer.h"
#include "hal_key.h"

typedef void (*key_msg_put_callabck_t)(uint8_t *, uint8_t );

#define KEY_DOWN                0x1000
#define KEY_LONG                0x2000
#define KEY_CONT                0x4000
#define KEY_UP                  0x8000

#define KEY_1                   0x0001
#define KEY_2                   0x0002
#define KEY_3                   0x0003
#define KEY_4                   0x0004
#define KEY_5                   0x0005
#define KEY_6                   0x0006
#define KEY_7                   0x0007
#define KEY_8                   0x0008
#define KEY_9                   0x0009

#define KEY_NULL                0x0000

typedef enum _key_stat_t
{
    KEY_STAT_INIT = 0,
    KEY_STAT_PRESS,
    KEY_STAT_LONG_PRESS,
    KEY_STAT_CONT_PRESS,
    KEY_STAT_RELASE
}key_stat_t;

typedef struct _key_t
{
    port_t port1;
    pin_t  pin1;

    port_t port2;
    pin_t  pin2;

    key_stat_t keyStat;
    uint16_t keyName;
    uint16_t delayCnt;
}key_t;

void Drv_Key_Init(void );
void Drv_Key_Regist_MsgPut_Callback(key_msg_put_callabck_t callback );

#endif 

