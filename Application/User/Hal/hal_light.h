#ifndef _HAL_LIGHT_H
#define _HAL_LIGHT_H

#include "hal_nuc123.h"

typedef enum _port_t
{
    PORTA = 0,
    PORTB,
    PORTC,
    PORTD,
    PORTE,
    PORTF
}port_t;


typedef enum _pin_t
{
    PIN0 = 0,
    PIN1,
    PIN2,
    PIN3,
    PIN4,
    PIN5,
    PIN6,
    PIN7,
    PIN8,
    PIN9,
    PIN10,
    PIN11,
    PIN12,
    PIN13,
    PIN14,
    PIN15
}pin_t;

void Hal_Light_Init(void );
void Hal_Light_All_Off(void );
void Hal_Light_All_On(void );
void Hal_Light_On(port_t portx, uint8_t pin );
void Hal_Light_Off(port_t portx, uint8_t pin );
void Hal_Light_Pwm_Duty_Set(uint8_t rVal, uint8_t gVal, uint8_t bVal );

#endif 

