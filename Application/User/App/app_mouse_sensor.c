/********************************************************
* @file       app_mouse_sensor.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_mouse_sensor.h"
#include "app_mouse_protocol.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Sensor_Handler(void *arg );
/* Private variables ------------------------------------*/
mSensor_ctrl_block_t mSensorCtrl;

void App_Sensor_Init(void )
{
    uint8_t cnt = 10;

    do{
        mSensorCtrl.productID = Drv_Sensor_Get_Product_ID();
        
    }while(mSensorCtrl.productID == 0 && cnt--);

    if(mSensorCtrl.productID == P3325_ID)
    {
        Drv_Sensor_P3325_Init(&mSensorCtrl.deltaX, &mSensorCtrl.deltaY);
    }

    Drv_Task_Regist_Period(App_Sensor_Handler, 0, 1, NULL);
}

void App_Sensor_Set_Detect_Time(mRate_t mRate )
{
    switch(mRate)
    {
        case RATE_125HZ: mSensorCtrl.delayTime = 1000/125; break;
        case RATE_250HZ: mSensorCtrl.delayTime = 1000/250; break;
        case RATE_500HZ: mSensorCtrl.delayTime = 1000/500; break;
        case RATE_1000HZ: mSensorCtrl.delayTime = 1000/1000;break;
        default: mSensorCtrl.delayTime = 1000/500;break;
    }
}

static void App_Sensor_Handler(void *arg )
{
    
}

