/********************************************************
* @file       hal_key.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_key.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Hal_Key_Init(void )
{
    //K1,PC13: input,mouse left button
    //K2,PC12: input,mouse right button
    //K3,PC11: input,mouse middle button
    //K4,PC10: input,mouse forward button
    //K5,PC9 : input,mouse back button
    //K6,PC8 : input,mouse DPI+ button
    //K7,PA15 : input,mouse DPI- button
    //K8,PB9 : input,mouse fire button
    //K9,PC4 : input,mouse report rate button
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD13_Msk) | (0 << GPIO_PMD_PMD13_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD12_Msk) | (0 << GPIO_PMD_PMD12_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD11_Msk) | (0 << GPIO_PMD_PMD11_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD10_Msk) | (0 << GPIO_PMD_PMD10_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD9_Msk) | (0 << GPIO_PMD_PMD9_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD8_Msk) | (0 << GPIO_PMD_PMD8_Pos);
    PA->PMD = (PA->PMD & ~GPIO_PMD_PMD15_Msk) | (0 << GPIO_PMD_PMD15_Pos);
    PB->PMD = (PB->PMD & ~GPIO_PMD_PMD9_Msk) | (0 << GPIO_PMD_PMD9_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD4_Msk) | (0 << GPIO_PMD_PMD4_Pos);
}


GPIO_T * Hal_Key_Get_Port(port_t portx )
{
    if(portx == PORTA)
    {
        return (GPIO_T *)PA_BASE;
    }
    else if(portx == PORTB)
    {
        return (GPIO_T *)PB_BASE;
    }
    else if(portx == PORTC)
    {
        return (GPIO_T *)PC_BASE;
    }
    else if(portx == PORTD)
    {
        return (GPIO_T *)PD_BASE;
    }
    else
    {
        return NULL;
    }
}


uint8_t Hal_Key_Get_Gpio(port_t portx, pin_t pin )
{
    uint8_t retVal;
    
    GPIO_T *port = Hal_Key_Get_Port(portx);

    if(port->PIN & (1<<(uint8_t )pin))
    {
        retVal = 1;
    }
    else
    {
        retVal = 0;
    }

    return retVal;
}

