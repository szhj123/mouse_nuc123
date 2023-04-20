/********************************************************
* @file       hal_interrupt.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_nuc123.h"
#include "hal_task.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void SysTick_Handler(void )
{
    Hal_Task_Isr_Handler();
}

