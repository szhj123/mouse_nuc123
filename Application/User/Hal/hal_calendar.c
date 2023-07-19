/********************************************************
* @file       hal_calendar.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_calendar.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
hal_isr_callback_t hal_calendar_isr_callback = NULL;

void Hal_Calendar_Init(hal_isr_callback_t callback )
{
    Timer1_Init();
    
    hal_calendar_isr_callback = callback;
}

void Hal_Calendar_Isr_Handler(void )
{
    if(hal_calendar_isr_callback != NULL)
    {
        hal_calendar_isr_callback();
    }
}

