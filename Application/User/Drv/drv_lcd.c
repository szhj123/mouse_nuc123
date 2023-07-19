/********************************************************
* @file       drv_lcd.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_lcd.h"

/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
#define LCD_COLOR_BUF_MAX_SIZE                 4800
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Lcd_Spi_Tx_Callback(void );
/* Private variables ------------------------------------*/
static uint8_t lcdColorBuf[LCD_COLOR_BUF_MAX_SIZE];

static uint8_t lcdSpiTxDoneFlag;

void Drv_Lcd_Init(void )
{
    Hal_Lcd_Init();

    LCD_RST_SET;
    Drv_Lcd_Delay_Us(1000);
    LCD_RST_CLR;
    Drv_Lcd_Delay_Us(10*1000);
    LCD_RST_SET;
    Drv_Lcd_Delay_Us(120*1000);

    Drv_Lcd_Wr_Cmd(0x11);
    Drv_Lcd_Delay_Us(120*1000);

    Drv_Lcd_Wr_Cmd(0xB0);
    Drv_Lcd_Wr_Data(0x00);
    Drv_Lcd_Wr_Data(0x00);

    Drv_Lcd_Wr_Cmd(0xB1);
    Drv_Lcd_Wr_Data(0x00);//0x40
    Drv_Lcd_Wr_Data(0x04);
    Drv_Lcd_Wr_Data(0x0a);
    //-----------------------------------------RGB seting  END--------------------------------------//
    Drv_Lcd_Wr_Cmd(0xB2);
    Drv_Lcd_Wr_Data(0x0C);   //porch setting
    Drv_Lcd_Wr_Data(0x0C);
    Drv_Lcd_Wr_Data(0x00);
    Drv_Lcd_Wr_Data(0x33);
    Drv_Lcd_Wr_Data(0x33);

    Drv_Lcd_Wr_Cmd(0xB7);
    Drv_Lcd_Wr_Data(0x02);   //64;VGH&VGL,12V&-8V

    Drv_Lcd_Wr_Cmd(0xC2);
    Drv_Lcd_Wr_Data(0x01);

    Drv_Lcd_Wr_Cmd(0xBB);
    Drv_Lcd_Wr_Data(0x35);   // 0x3C 28;VCOM

    Drv_Lcd_Wr_Cmd(0xC3);
    Drv_Lcd_Wr_Data(0x19);  //0x27

    Drv_Lcd_Wr_Cmd(0xC4);
    Drv_Lcd_Wr_Data(0x20);  //0x3F

    Drv_Lcd_Wr_Cmd(0xC5);
    Drv_Lcd_Wr_Data(0x20);   //vcom shift.0V  20

    Drv_Lcd_Wr_Cmd(0xD0);
    Drv_Lcd_Wr_Data(0xA4);
    Drv_Lcd_Wr_Data(0xA2);   //AVDD&AVCL,6.8v&-4.8v

    Drv_Lcd_Wr_Cmd(0xD2);
    Drv_Lcd_Wr_Data(0x4C);

    Drv_Lcd_Wr_Cmd(0xE8);
    Drv_Lcd_Wr_Data(0x83);   //Booster CLK Select

    Drv_Lcd_Wr_Cmd(0xE9);
    Drv_Lcd_Wr_Data(0x09);   //EQ
    Drv_Lcd_Wr_Data(0x09);
    Drv_Lcd_Wr_Data(0x08);

    Drv_Lcd_Wr_Cmd(0x36);
    Drv_Lcd_Wr_Data(0x00);   //ACCESS

    Drv_Lcd_Wr_Cmd(0x35);
    Drv_Lcd_Wr_Data(0x00);   //TEWrite

    Drv_Lcd_Wr_Cmd(0x3A);
    Drv_Lcd_Wr_Data(0x55);   //18bit

    Drv_Lcd_Wr_Cmd(0xC6);
    Drv_Lcd_Wr_Data(0x0F);   //0x09orig [DB7-5] 0forDotInv,1forColumnInv ,0xEF  �з�

    Drv_Lcd_Wr_Cmd(0xE0);
    Drv_Lcd_Wr_Data(0xD0);   //V0[7-4] & V63[3-0]
    Drv_Lcd_Wr_Data(0x06);   //V62[5-0]
    Drv_Lcd_Wr_Data(0x0C);   //V61[5-0]
    Drv_Lcd_Wr_Data(0x09);   //V59[4-0]
    Drv_Lcd_Wr_Data(0x09);   //V57[4-0]
    Drv_Lcd_Wr_Data(0x25);   //J1[5-4] & V50[3-0]
    Drv_Lcd_Wr_Data(0x2E);   //V43[6-0]
    Drv_Lcd_Wr_Data(0x33);   //V27[6-4] & V36[2-0]
    Drv_Lcd_Wr_Data(0x45);   //V20[6-0]
    Drv_Lcd_Wr_Data(0x36);   //J0[5-4] & V13[3-0]
    Drv_Lcd_Wr_Data(0x12);   //V6[4-0]
    Drv_Lcd_Wr_Data(0x12);   //V4[4-0]
    Drv_Lcd_Wr_Data(0x2E);   //V2[5-0]
    Drv_Lcd_Wr_Data(0x34);   //V1[5-0]

    Drv_Lcd_Wr_Cmd(0xE1);
    Drv_Lcd_Wr_Data(0xD0);   //V0[7-4] & V63[3-0]
    Drv_Lcd_Wr_Data(0x06);   //V62[5-0]
    Drv_Lcd_Wr_Data(0x0C);   //V61[5-0]
    Drv_Lcd_Wr_Data(0x09);   //V59[4-0]
    Drv_Lcd_Wr_Data(0x09);   //V57[4-0]
    Drv_Lcd_Wr_Data(0x25);   //J1[5-4] & V50[3-0]
    Drv_Lcd_Wr_Data(0x2E);   //V43[6-0]
    Drv_Lcd_Wr_Data(0x33);   //V27[6-4] & V36[2-0]
    Drv_Lcd_Wr_Data(0x45);   //V20[6-0]
    Drv_Lcd_Wr_Data(0x36);   //J0[5-4] & V13[3-0]
    Drv_Lcd_Wr_Data(0x12);   //V6[4-0]
    Drv_Lcd_Wr_Data(0x12);   //V4[4-0]
    Drv_Lcd_Wr_Data(0x2E);   //V2[5-0]
    Drv_Lcd_Wr_Data(0x34);   //V1[5-0]

    Drv_Lcd_Wr_Cmd(0x21);

    Drv_Lcd_Wr_Cmd(0xD6);
    Drv_Lcd_Wr_Data(0xA1);

    Drv_Lcd_Wr_Cmd(0x2A);     //Column Address Set
    Drv_Lcd_Wr_Data(0x00);
    Drv_Lcd_Wr_Data(0x00);   //0
    Drv_Lcd_Wr_Data(0x00);
    Drv_Lcd_Wr_Data(0xEF);   //239

    Drv_Lcd_Wr_Cmd(0x2B);     //Row Address Set
    Drv_Lcd_Wr_Data(0x00);
    Drv_Lcd_Wr_Data(0x00);
    Drv_Lcd_Wr_Data(0x00);
    Drv_Lcd_Wr_Data(0xef);

    Drv_Lcd_Wr_Cmd(0x29);

    Drv_Lcd_Wr_Cmd(0x2C);
}

void Drv_Lcd_Clear(uint16_t color )
{
    uint32_t i;
    uint32_t totalLength = LCD_W * LCD_H * 2;
    
    
    Drv_Lcd_Set_Address(0, 0, 239, 239);
    
    LCD_DC_SET;
    
    Hal_Lcd_Spi_Cs_Enable();

    while(totalLength)
    {
        for(i=0;i<LCD_COLOR_BUF_MAX_SIZE/2;i++)
        {
            lcdColorBuf[i*2] = (uint8_t )(color>>8);
            lcdColorBuf[i*2+1] = (uint8_t )color;
        }
        
        Hal_Lcd_Spi_Dma_Tx(lcdColorBuf, LCD_COLOR_BUF_MAX_SIZE, Drv_Lcd_Spi_Tx_Callback);
        
        while(lcdSpiTxDoneFlag == 0);
        lcdSpiTxDoneFlag = 0;

        totalLength -= LCD_COLOR_BUF_MAX_SIZE;
    }
    
    Hal_Lcd_Spi_Cs_Disable();

}

static void Drv_Lcd_Spi_Tx_Callback(void )
{
    lcdSpiTxDoneFlag = 1;
}

void Drv_Lcd_Set_Address(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 )
{
    Drv_Lcd_Wr_Cmd(0x2a);
    Drv_Lcd_Wr_Data(x1 >> 8);
    Drv_Lcd_Wr_Data(x1);
    Drv_Lcd_Wr_Data(x2 >> 8);
    Drv_Lcd_Wr_Data(x2);
    Drv_Lcd_Wr_Cmd(0x2b);
    Drv_Lcd_Wr_Data(y1 >> 8);
    Drv_Lcd_Wr_Data(y1);
    Drv_Lcd_Wr_Data(y2 >> 8);
    Drv_Lcd_Wr_Data(y2);
    Drv_Lcd_Wr_Cmd(0x2c);
}

void Drv_Lcd_Wr_Cmd(uint8_t cmd )
{
    LCD_DC_CLR;
    
    Hal_Lcd_Spi_Cs_Enable();

    Hal_Lcd_Spi_Single_Tx(cmd);

    Hal_Lcd_Spi_Cs_Disable();
}

void Drv_Lcd_Wr_Data(uint8_t dat )
{
    LCD_DC_SET;

    Hal_Lcd_Spi_Cs_Enable();

    Hal_Lcd_Spi_Single_Tx(dat);

    Hal_Lcd_Spi_Cs_Disable();
}

inline void Drv_Lcd_Bg_Led_On(void )
{
    LCD_BG_LED_ON;
}

inline void Drv_Lcd_Bg_Led_Off(void )
{
    LCD_BG_LED_OFF;
}

void Drv_Lcd_Delay_Us(uint32_t us )
{
    uint8_t i;
    
    while(us--)
    {
        for(i=0;i<48;i++);    
    }
}

