/********************************************************
* @file       main.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_nuc123.h"
#include "drv_task.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/


int main(void )
{
    Hardware_Init();

    Drv_Task_Init();
    
	while(1)
	{
        Drv_Task_Run();
	}
}
