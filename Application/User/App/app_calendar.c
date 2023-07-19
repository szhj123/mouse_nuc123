/********************************************************
* @file       app_calendar.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_calendar.h"
#include "app_mouse_protocol.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Calendar_Handler(void );
/* Private variables ------------------------------------*/
static date_t date;

void App_Calendar_Init(void )
{
    Drv_Calendar_Init(App_Calendar_Handler);
}

void App_Calendar_Set_Date(uint8_t *buf, uint8_t len )
{
    uint8_t i;
    uint8_t *pData = (uint8_t *)&date;

    for(i=0;i<len;i++)
    {
        *pData++ = *buf++;
    }
}

static void App_Calendar_Handler(void )
{
    if(date.sec >= 59)
    {
        date.sec = 0;
        
        if(date.min >= 59)
        {
            date.min = 0;

            if(date.hour >= 23)
            {
                date.hour = 0;

                if(date.weekDay >= 6)
                {
                    date.weekDay = 0;
                }
                else
                {
                    date.weekDay++;
                }

                if((date.year % 400) == 0 || ((date.year % 4 == 0) && (date.year % 100 != 0)))
                {
                    if(date.month == 2)
                    {
                        if(date.day >= 29)
                        {
                            date.day = 0;

                            date.month = 3;
                        }
                        else
                        {
                            date.day++;
                        }
                    }
                }
                else
                {
                    if(date.month == 2)
                    {
                        if(date.day >= 28)
                        {
                            date.day = 0;

                            date.month = 3;
                        }
                        else
                        {
                            date.day++;
                        }
                    }
                }

                switch(date.month)
                {
                    case 1: case 3: case 5: case 7: case 8: case 10:
                    {
                        if(date.day >= 31)
                        {
                            date.day = 0;

                            date.month++;
                        }
                        else
                        {
                            date.day++;
                        }
                        break;
                    }
                    case 4: case 6: case 9: case 11:
                    {
                        if(date.day >= 30)
                        {
                            date.day = 0;

                            date.month++;
                        }
                        else
                        {
                            date.day++;
                        }
                        break;
                    }
                    case 12:
                    {
                        if(date.day >= 31)
                        {
                            date.day = 0;

                            date.month = 1;

                            date.year++;
                        }
                        else
                        {
                            date.day++;
                        }
                        break;
                    }
                    default: break;
                }
            }
            else
            {
                date.hour++;
            }
        }
        else
        {
            date.min++;
        }
    }
    else
    {
        date.sec++;
    }
}


