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
#include "drv_timer.h"
#include "drv_spi_flash.h"
#include "drv_usb.h"

#include "app_event.h"
#include "app_calendar.h"
#include "app_mouse_protocol.h"
#include "app_mouse_sensor.h"
#include "app_lcd.h"
#include "app_light.h"
#include "app_key.h"
#include "app_usb.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

int main(void )
{
    Hardware_Init();
		
    Drv_Task_Init();

    Drv_Timer_Init();

    Drv_Spi_Flash_Init();

    App_Event_Init();

    App_Mouse_Para_Init();

    App_Sensor_Init();

    App_Lcd_Init();

    App_Light_Init();

    App_Key_Init();

    App_Usb_Init();

    App_Calendar_Init();
    
	while(1)
	{
        Drv_Task_Run();
	}
}

