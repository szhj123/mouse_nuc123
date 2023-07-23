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


void Drv_Key_Init(void )
{
    Hal_Key_Init();

    Drv_Timer_Regist_Period(Drv_Key_Handler, 0, 1, NULL);
}

static void Drv_Key_Handler(void *arg )
{
    uint8
}

