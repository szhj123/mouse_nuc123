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

#include "app_mouse_sensor.h"
#include "app_lcd.h"
#include "app_light.h"
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

    Drv_Usb_Init();

    App_Sensor_Init();

    App_Lcd_Init();

    App_Light_Init();
    
	while(1)
	{
        Drv_Task_Run();
	}
}

