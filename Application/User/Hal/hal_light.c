/********************************************************
* @file       hal_rgb.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_light.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Hal_Light_Init(void )
{
    Pwm_Init();
}

void Hal_Light_All_Off(void )
{
    PB->DOUT |= (1 << 8);// light1
    PB->DOUT |= (1 << 14);//light2
    PA->DOUT |= (1 << 11);//light3
    PA->DOUT |= (1 << 10);//light4
    PB->DOUT |= (1 << 4); //light5
    PB->DOUT |= (1 << 5); //light6
}

void Hal_Light_All_On(void )
{
    PB->DOUT &= ~(1 << 8);// light1
    PB->DOUT &= ~(1 << 14);//light2
    PA->DOUT &= ~(1 << 11);//light3
    PA->DOUT &= ~(1 << 10);//light4
    PB->DOUT &= ~(1 << 4); //light5
    PB->DOUT &= ~(1 << 5); //light6
}

GPIO_T * Hal_Light_Get_Port(port_t portx )
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


void Hal_Light_On(port_t portx, uint8_t pin )
{
    GPIO_T *port = Hal_Light_Get_Port(portx);

    if(port == NULL)
    {
        return ;
    }

    port->DOUT &= ~(1 << pin );
}

void Hal_Light_Off(port_t portx, uint8_t pin )
{
    GPIO_T *port = Hal_Light_Get_Port(portx);
    
    if(port == NULL)
    {
        return ;
    }

    port->DOUT |= (1 << pin );
}

void Hal_Light_Pwm_Duty_Set(uint8_t rVal, uint8_t gVal, uint8_t bVal )
{
    PWMA->CMR0 = rVal;
    PWMA->CMR1 = gVal;
    PWMA->CMR2 = bVal;
}

