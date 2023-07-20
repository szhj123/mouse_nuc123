/********************************************************
* @file       drv_light.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_light.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Drv_Light_Init(void )
{
    Hal_Light_Init();

    Hal_Light_Pwm_Duty_Set(0, 0, 0);
}

void Drv_Light_All_Off(void )
{
    Hal_Light_All_Off();
}

void Drv_Light_All_On(uint8_t rVal, uint8_t gVal, uint8_t bVal )
{
    Hal_Light_Pwm_Duty_Set(rVal, gVal, bVal);
    
    Hal_Light_All_On();
}

void Drv_Light_Single_On(light_t *light )
{
    Drv_Light_All_Off();

    Hal_Light_Pwm_Duty_Set(light->red, light->green, light->blue);

    Hal_Light_On(light->port, light->pin);
}

void Drv_Light_Single_Off(light_t *light )
{
    Hal_Light_Off(light->port, light->pin);
}



