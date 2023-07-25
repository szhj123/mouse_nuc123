/********************************************************
* @file       drv_key.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_key.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Key_Handler(void *arg );
static void Drv_Key_Detect(key_t *key );
static void Drv_Wheel_Detect(key_t *key );
/* Private variables ------------------------------------*/
static key_t keyBuf[] = 
{
    {
        .port1 = PORTC,
        .pin1  = PIN13,
        .port2 = PORT_NULL,
        .pin2  = PIN_NULL,
        .keyName = KEY_1
    },
    {
        .port1 = PORTC,
        .pin1  = PIN12,
        .port2 = PORT_NULL,
        .pin2  = PIN_NULL,
        .keyName = KEY_2
    },
    {
        .port1 = PORTC,
        .pin1  = PIN11,
        .port2 = PORT_NULL,
        .pin2  = PIN_NULL,
        .keyName = KEY_3
    },
    {
        .port1 = PORTC,
        .pin1  = PIN10,
        .port2 = PORT_NULL,
        .pin2  = PIN_NULL,
        .keyName = KEY_4
    },
    {
        .port1 = PORTC,
        .pin1  = PIN9,
        .port2 = PORT_NULL,
        .pin2  = PIN_NULL,
        .keyName = KEY_5
    },
    {
        .port1 = PORTC,
        .pin1  = PIN8,
        .port2 = PORT_NULL,
        .pin2  = PIN_NULL,
        .keyName = KEY_6
    },
    {
        .port1 = PORTA,
        .pin1  = PIN15,
        .port2 = PORT_NULL,
        .pin2  = PIN_NULL,
        .keyName = KEY_7
    },
    {
        .port1 = PORTB,
        .pin1  = PIN9,
        .port2 = PORT_NULL,
        .pin2  = PIN_NULL,
        .keyName = KEY_8
    },
    {
        .port1 = PORTC,
        .pin1  = PIN4,
        .port2 = PORT_NULL,
        .pin2  = PIN_NULL,
        .keyName = KEY_9
    },
};

static key_t wheel = 
{
    .port1 = PORTF,
    .pin1  = PIN2,
    .port2 = PORTF,
    .pin2  = PIN3,
    .keyName = KEY_10
};


key_msg_put_callabck_t keyMsgPutCallbck = NULL;


void Drv_Key_Init(void )
{
    Hal_Key_Init();

    Drv_Timer_Regist_Period(Drv_Key_Handler, 0, 1, NULL);
}

void Drv_Key_Regist_MsgPut_Callback(key_msg_put_callabck_t callback )
{
    keyMsgPutCallbck = callback;
}

static void Drv_Key_Handler(void *arg )
{
    uint8_t i;

    for(i=0;i<sizeof(keyBuf)/sizeof(key_t);i++)
    {
        Drv_Key_Detect(&keyBuf[i]);
    }

    Drv_Wheel_Detect(&wheel);
}

uint16_t Drv_Key_Get_Name(key_t *key )
{
    uint16_t retName = KEY_NULL;
    
    if(key->port2 != PORT_NULL && key->port1 != PORT_NULL)
    {
        if(!Hal_Key_Get_Gpio(key->port1, key->pin1) && !Hal_Key_Get_Gpio(key->port2, key->pin2))
        {
            retName = key->keyName;
        }
    }
    else if(key->port1 != NULL) 
    {
        if(!Hal_Key_Get_Gpio(key->port1, key->pin1))
        {
            retName = key->keyName;
        }
    }

    return retName;    
}

static void Drv_Key_Detect(key_t *key )
{
    uint16_t keyName = KEY_NULL;
    
    if(key->delayCnt < 0xffff)
    {
        key->delayCnt++;
    }

    keyName = Drv_Key_Get_Name(key);

    switch(key->keyStat)
    {
        case KEY_STAT_INIT:
        {
            if(keyName != KEY_NULL)
            {
                key->delayCnt = 0;
                key->keyStat = KEY_STAT_PRESS;
            }
            break;
        }
        case KEY_STAT_PRESS:
        {
            if(keyName != KEY_NULL)
            {
                if(key->delayCnt >20)
                {
                    key->keyNameSave = keyName;
                    keyName |= KEY_DOWN;
                    keyMsgPutCallbck((uint8_t *)&keyName, 2);

                    key->delayCnt = 0;
                    key->keyStat = KEY_STAT_LONG_PRESS;
                }
            }
            else
            {
                key->keyStat = KEY_STAT_INIT;
            }
            break;
        }
        case KEY_STAT_LONG_PRESS:
        {
            if(keyName != KEY_NULL)
            {
                if(key->delayCnt >= 2000)
                {
                    key->delayCnt = 0;
                }
            }
            else
            {
                key->keyStat = KEY_STAT_RELASE;
            }
            break;
        }
        case KEY_STAT_RELASE:
        {
            keyName = key->keyNameSave;
            keyName |= KEY_UP;
            
            keyMsgPutCallbck((uint8_t *)&keyName, 2);
            key->keyStat = KEY_STAT_INIT;
            break;
        }
        default: break;
    }
}

static void Drv_Wheel_Detect(key_t *key )
{    
    static uint8_t saveKnob_A;
    static uint8_t saveKnob_B;
    
    uint16_t keyVal = KEY_NULL;
    
    if(KNOB_A ^ KNOB_B)
    {
        if(saveKnob_A && saveKnob_B)
        {
            if(!KNOB_A && KNOB_B)
            {
                keyVal = key->keyName | WHEEL_DONW;

                keyMsgPutCallbck((uint8_t *)&keyVal, 2);
            }
            else if(KNOB_A && !KNOB_B)
            {
                keyVal = key->keyName | WHEEL_UP;

                keyMsgPutCallbck((uint8_t *)&keyVal, 2);
            }
        }
        else if(!saveKnob_A && !saveKnob_B)
        {
            if(KNOB_A && !KNOB_B)
            {
                keyVal = key->keyName | WHEEL_DONW;

                keyMsgPutCallbck((uint8_t *)&keyVal, 2);
            }
            else if(!KNOB_A && KNOB_B)
            {
                keyVal = key->keyName | WHEEL_UP;

                keyMsgPutCallbck((uint8_t *)&keyVal, 2);
            }
        }
        saveKnob_A = KNOB_A;
        saveKnob_B = KNOB_B;
    }
    else
    {
        saveKnob_A = KNOB_A;
        saveKnob_B = KNOB_B;
    }
}

