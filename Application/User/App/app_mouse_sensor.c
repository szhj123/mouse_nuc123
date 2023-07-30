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
#include "app_usb.h"
#include "app_lcd.h"
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

void App_Sensor_Set_Dpi(uint8_t dpiIndex )
{
    uint8_t dpiVal = App_Mouse_Get_Dpi_Val(dpiIndex);
    
    if(dpiVal != 0x80)
    {
        Drv_Sensor_Set_Dpi(dpiVal & 0x7f);

        if(dpiVal & 0x80)
        {
            mSensorCtrl.deltaXYDoubleFlag = 1;
        }
        else
        {
            mSensorCtrl.deltaXYDoubleFlag = 0;
        }
    }
}

static void App_Sensor_Handler(void *arg )
{
    uint8_t burstMotionBuf[6] = {0};
    
    if(++mSensorCtrl.delayCnt >= mSensorCtrl.delayTime)
    {
        if(App_Lcd_Get_Rw_Stat() != LCD_IDLE)
        {
            return ;
        }
        
        Drv_Sensor_Get_Burst_Motion(burstMotionBuf, sizeof(burstMotionBuf));

        if(burstMotionBuf[0] & 0x80)
        {
            if(burstMotionBuf[1] == 0x0)
            {
                Drv_Sensor_P3325_Init(&mSensorCtrl.deltaX, &mSensorCtrl.deltaY);
            }
            else
            {
                mSensorCtrl.deltaX = (int16_t )burstMotionBuf[3] << 8 | burstMotionBuf[2];
                mSensorCtrl.deltaY = (int16_t )burstMotionBuf[5] << 8 | burstMotionBuf[4];

                #if 1
                if(mSensorCtrl.deltaXYDoubleFlag)
                {
                    mSensorCtrl.deltaX *= 2;
                    mSensorCtrl.deltaY *= 2;
                }
                #endif 

                #if 1
                if(mSensorCtrl.deltaX == 0x8000)
        	        mSensorCtrl.deltaX = 0x8001;

                if(mSensorCtrl.deltaX == 0x7fff)
                    mSensorCtrl.deltaX = 0x0001;

        	    if(mSensorCtrl.deltaY == 0x8000)
        	        mSensorCtrl.deltaY = 0x8001;

                if(mSensorCtrl.deltaY == 0x7fff)
                    mSensorCtrl.deltaY = 0x0001;
                #endif 

                App_Usb_Mouse_Motion_Handler(mSensorCtrl.deltaX, mSensorCtrl.deltaY);
            }
        }
        
        mSensorCtrl.delayCnt = 0;
    }
}

