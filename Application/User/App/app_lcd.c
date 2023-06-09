/********************************************************
* @file       app_lcd.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_lcd.h"

/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void App_Lcd_Init(void )
{
    Drv_Lcd_Init();    
    
    App_Lcd_Clr(BLUE);
}

void App_Lcd_Clr(uint16_t color )
{
    Drv_Lcd_Bg_Led_On();
    
    Drv_Lcd_Clear(color);
}

