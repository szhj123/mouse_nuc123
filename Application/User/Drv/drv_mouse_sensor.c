/********************************************************
* @file       drv_mouse_sensor.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_mouse_sensor.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
const uint8_t mReg3325Buf[][2] =
{
    //Register Address, Value(Modify) Note, Value(Default)
    {0x78, 0x80},
    {0x79, 0x80},
    {0x14, 0x80},

	{0x20, 0x40},
    {0x1a, 0x40},
    {0x47, 0x00},
    {0x48, 0x01},

	{0x60, 0x01},
    {0x69, 0x03},
    {0x1d, 0x90},

	{0x1b, 0x2e},
    {0x24, 0x05},
    {0x56, 0x00},

	{0x2c, 0x8a},
	{0x2d, 0x58},
    {0x40, 0x80},
    {0x7f, 0x01},
    {0x7a, 0x32},
    {0x6a, 0x93},
    {0x6b, 0x68},
    {0x6c, 0x71},
    {0x6d, 0x50},
    {0x7f, 0x00},
    {0x7f, 0x02},
    {0x29, 0x1c},
    {0x2a, 0x1a},
    {0x2b, 0x90},
    {0x40, 0x80},
    {0x7f, 0x00}
};

void Drv_Sensor_P3325_Init(int16_t *xVal, int16_t *yVal )
{
    uint8_t i;
    uint8_t delta_x_l;
    uint8_t delta_x_h;
    uint8_t delta_y_l;
    uint8_t delta_y_h;

    for(i=0;i<sizeof(mReg3325Buf)/2;i++)
    {
        Drv_Sensor_Write(mReg3325Buf[i][0], mReg3325Buf[i][1]);
    }

    delta_x_l = Drv_Sensor_Single_Read(0x03);
    delta_x_h = Drv_Sensor_Single_Read(0x04);
    delta_y_l = Drv_Sensor_Single_Read(0x05);
    delta_y_h = Drv_Sensor_Single_Read(0x06);

    *xVal = (uint16_t )delta_x_h << 8 | delta_x_l;
    *yVal = (uint16_t )delta_y_h << 8 | delta_y_l;
}

uint8_t Drv_Sensor_Get_Product_ID(void )
{
    uint8_t retVal = 0;
    
    Drv_Sensor_Write(0x3a, 0x5a);
    
    Drv_Sensor_Delay(500);
    
    Drv_Sensor_Write(0x18, 0x39);
    
    Drv_Sensor_Delay(10000);

    retVal = Drv_Sensor_Single_Read(0x00);

    return retVal;
}

void Drv_Sensor_Set_Dpi(uint8_t dpiVal )
{
    Drv_Sensor_Write(0x1b, dpiVal);
}

void Drv_Sensor_Get_Burst_Motion(uint8_t *buf, uint8_t len )
{
    Drv_Sensor_Multiple_Read(0x16, buf, len );
}

void Drv_Sensor_Write(uint8_t addr, uint8_t val )
{
    uint8_t i;

    Hal_Sensor_Pin_Reuse();

    Hal_Sensor_Cs_Enable();

    addr |= 0x80;

    for(i=0;i<8;i++)
    {
        if(addr & 0x80)
        {
            M_SENSOR_SDA_H;
        }
        else
        {
            M_SENSOR_SDA_L;
        }

        M_SENSOR_SCL_L;
        Drv_Sensor_Delay(1);
        M_SENSOR_SCL_H;
        Drv_Sensor_Delay(1);

        addr <<= 1;
    }

    for(i=0;i<8;i++)
    {
        if(val & 0x80)
        {
            M_SENSOR_SDA_H;
        }
        else
        {
            M_SENSOR_SDA_L;
        }

        M_SENSOR_SCL_L;
        Drv_Sensor_Delay(1);
        M_SENSOR_SCL_H;
        Drv_Sensor_Delay(1);

        val <<= 1;
    }

    Hal_Sensor_Cs_Disable();
}

uint8_t Drv_Sensor_Single_Read(uint8_t addr )
{
    uint8_t i;
    uint8_t retVal = 0x0;

    Hal_Sensor_Pin_Reuse();
    
    Hal_Sensor_Cs_Enable();

    addr &= 0x7f;

    for(i=0;i<8;i++)
    {
        if(addr & 0x80)
        {
            M_SENSOR_SDA_H;
        }
        else
        {
            M_SENSOR_SDA_L;
        }

        M_SENSOR_SCL_L;
        Drv_Sensor_Delay(1);
        M_SENSOR_SCL_H;
        Drv_Sensor_Delay(1);

        addr <<= 1;
    }

    for(i=0;i<8;i++)
    {
        retVal <<= 1;

        M_SENSOR_SCL_L;
        Drv_Sensor_Delay(1);
        M_SENSOR_SCL_H;
        Drv_Sensor_Delay(1);

        if(M_SENSOR_SDA_PIN)
        {
            retVal |= 0x01;
        }
    }

    Hal_Sensor_Cs_Disable();

    return retVal;
}

void Drv_Sensor_Multiple_Read(uint8_t addr, uint8_t *buf, uint8_t len )
{
    uint8_t i,j;
    uint8_t retVal = 0x0;

    Hal_Sensor_Pin_Reuse();
    
    Hal_Sensor_Cs_Enable();

    addr &= 0x7f;

    for(i=0;i<8;i++)
    {
        if(addr & 0x80)
        {
            M_SENSOR_SDA_H;
        }
        else
        {
            M_SENSOR_SDA_L;
        }

        M_SENSOR_SCL_L;
        Drv_Sensor_Delay(1);
        M_SENSOR_SCL_H;
        Drv_Sensor_Delay(1);

        addr <<= 1;
    }

    for(j=0;j<len;j++)
    {
        for(i=0;i<8;i++)
        {
            retVal <<= 1;

            M_SENSOR_SCL_L;
            Drv_Sensor_Delay(1);
            M_SENSOR_SCL_H;
            Drv_Sensor_Delay(1);

            if(M_SENSOR_SDA_PIN)
            {
                retVal |= 0x01;
            }
        }

        buf[j] = retVal;
    }
    
    Hal_Sensor_Cs_Disable();
}


void Drv_Sensor_Delay(uint32_t delayCnt )
{
    while(delayCnt--);
}

