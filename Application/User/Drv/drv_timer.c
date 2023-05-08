/********************************************************
* @file       drv_timer.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_timer.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Timer_Isr_Handler(void );
/* Private variables ------------------------------------*/
timer_ctrl_block_t timerList[TIMER_MAX_NUM];

void Drv_Timer_Init(void )
{
    uint8_t i;

    Hal_Timer_Init();
    
    Hal_Timer_Regist_Isr_Callback(Drv_Timer_Isr_Handler);    

    for(i=0;i<TIMER_MAX_NUM;i++)
    {
        timerList[i].handler = NULL;
        timerList[i].arg = NULL;
        timerList[i].ticks = 0;
        timerList[i].period = 0;
        timerList[i].isRegist = 0;
    }
}

uint8_t Drv_Timer_Regist(void (*handler)(void *), uint16_t ticks, uint16_t period, void *arg )
{
    uint8_t timerId;

    for(timerId=0;timerId<TIMER_MAX_NUM;timerId++)
    {
        if(timerList[timerId].isRegist == 0)
        {
            timerList[timerId].handler = handler;
            timerList[timerId].arg = arg;
            timerList[timerId].ticks = ticks;
            timerList[timerId].period = period;
            timerList[timerId].isRegist = 1;

            break;
        }
    }

    return timerId;
}

uint8_t Drv_Timer_Regist_Oneshot(void (*handler)(void *), uint16_t ticks, void *arg )
{
    return Drv_Timer_Regist(handler, ticks, 0, arg );
}

uint8_t Drv_Timer_Regist_Period(void (*handler)(void *), uint16_t ticks, uint16_t period, void *arg )
{
    return Drv_Timer_Regist(handler, ticks, period, arg);
}

uint8_t Drv_Timer_Delete(uint8_t timerId )
{
    if(timerId >= TIMER_MAX_NUM)
    {
        return TIMER_ERR;
    }

    timerList[timerId].handler = NULL;
    timerList[timerId].arg = NULL;
    timerList[timerId].ticks = 0;
    timerList[timerId].period = 0;
    timerList[timerId].isRegist = 0;

    return TIMER_OK;
}

static void Drv_Timer_Isr_Handler(void )
{
    uint8_t timerId;

    for(timerId=0;timerId<TIMER_MAX_NUM;timerId++)
    {
        if(timerList[timerId].ticks > 0)
        {
            timerList[timerId].ticks--;
        }

        if(timerList[timerId].ticks == 0)
        {
            if(timerList[timerId].handler != NULL)
            {
                timerList[timerId].handler(timerList[timerId].arg);
            }

            if(timerList[timerId].period)
            {
                timerList[timerId].ticks = timerList[timerId].period;
            }
            else
            {
                Drv_Timer_Delete(timerId);
            }
        }
    }
}

