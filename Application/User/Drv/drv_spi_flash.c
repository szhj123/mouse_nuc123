/********************************************************
* @file       drv_spi_flash.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_spi_flash.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

static uint8_t spiFlashTxDoneFlag = 0;
static uint8_t spiFlashRxDoneFlag = 0;

void Drv_Spi_Flash_Init(void )
{
    Hal_Spi_Flash_Init();

    #if 0
    static uint8_t txBuf[250];
    static uint8_t rxBuf[250];

    Drv_Spi_Flash_Erase_64k(0x20000*4);

    Drv_Spi_Flash_Erase_64k(0x20000*4+ 0x10000);

    for(uint16_t i=0;i<sizeof(txBuf);i++)
    {
        txBuf[i] = i+1;
    }

    Drv_Spi_Flash_Write(0x20000*4, txBuf, sizeof(txBuf));

    Drv_Spi_Flash_Read(0x20000*4, rxBuf, sizeof(rxBuf));
    #endif 
}

#if 0
void Drv_Spi_Flash_Read_Id(void )
{
    uint8_t i;
    uint8_t flashID[3] = {0};
    
    Hal_Spi_Pin_Reuse();

    Hal_Spi_Flash_Cs_Enable();

    Hal_Spi_Flash_Single_Tx(0x9f);

    for(i=0;i<3;i++)
    {
        flashID[i] = Hal_Spi_Flash_Single_Rx();
    }

    Hal_Spi_Flash_Cs_Disable();
}
#endif 

void Drv_Spi_Flash_Wait_Idle(void )
{
    uint8_t status;
    
    Hal_Spi_Flash_Cs_Enable();

    Hal_Spi_Flash_Single_Tx(FLASH_STATUS_READ);

    do{
        status = Hal_Spi_Flash_Single_Rx();
    }while(status & 0x01);

    Hal_Spi_Flash_Cs_Disable();
}

void Drv_Spi_Flash_Write_Enable(void )
{
    Hal_Spi_Flash_Cs_Enable();
    
    Hal_Spi_Flash_Single_Tx(FLASH_WRITE_ENABLE);
    
    Hal_Spi_Flash_Cs_Disable();
}

void Drv_Spi_Flash_Erase_64k(uint32_t addr )
{
    Hal_Spi_Pin_Reuse();
    
    Drv_Spi_Flash_Wait_Idle();
    
    Drv_Spi_Flash_Write_Enable();

    Hal_Spi_Flash_Cs_Enable();

    Hal_Spi_Flash_Single_Tx(FLASH_BLOCK_64K_ERASE);

    Hal_Spi_Flash_Single_Tx((uint8_t )(addr>>16));
    Hal_Spi_Flash_Single_Tx((uint8_t )(addr>>8));
    Hal_Spi_Flash_Single_Tx((uint8_t )addr);

    Hal_Spi_Flash_Cs_Disable();
}


static void Drv_Spi_Flash_Tx_Callback(void )
{
    spiFlashTxDoneFlag = 1;
}

static void Drv_Spi_Flash_Rx_Callback(void )
{
    spiFlashRxDoneFlag = 1;
}

void Drv_Spi_Flash_Loop_Write(uint32_t addr, uint8_t *buf, uint32_t length )
{
    uint32_t i;
    
    Hal_Spi_Pin_Reuse();
    
    Drv_Spi_Flash_Wait_Idle();
    
    Drv_Spi_Flash_Write_Enable();

    Hal_Spi_Flash_Cs_Enable();
    
    Hal_Spi_Flash_Single_Tx(FLASH_PAGE_PROGRAM);

    Hal_Spi_Flash_Single_Tx((uint8_t )(addr>>16));
    Hal_Spi_Flash_Single_Tx((uint8_t )(addr>>8));
    Hal_Spi_Flash_Single_Tx((uint8_t )addr);

    for(i=0;i<length;i++)
    {
        Hal_Spi_Flash_Single_Tx(buf[i]);
    }
    
    Hal_Spi_Flash_Cs_Disable();
}


void Drv_Spi_Flash_Dma_Write(uint32_t addr, uint8_t *buf, uint32_t length )
{
    Hal_Spi_Pin_Reuse();
    
    Drv_Spi_Flash_Wait_Idle();
    
    Drv_Spi_Flash_Write_Enable();

    Hal_Spi_Flash_Cs_Enable();
    
    Hal_Spi_Flash_Single_Tx(FLASH_PAGE_PROGRAM);

    Hal_Spi_Flash_Single_Tx((uint8_t )(addr>>16));
    Hal_Spi_Flash_Single_Tx((uint8_t )(addr>>8));
    Hal_Spi_Flash_Single_Tx((uint8_t )addr);

    Hal_Spi_Flash_Dma_Tx(buf, length, Drv_Spi_Flash_Tx_Callback);

    while(spiFlashTxDoneFlag == 0);
    spiFlashTxDoneFlag = 0;
    
    Hal_Spi_Flash_Cs_Disable();
}

void Drv_Spi_Flash_Write(uint32_t addr, uint8_t *buf, uint32_t length )
{
    uint32_t i;
    uint32_t startPageRemainSize;
    uint32_t lastPageRemainSize;
    uint32_t pageSize;
    
    Hal_Spi_Pin_Reuse();

    startPageRemainSize = SPI_FLASH_PAGE_SIZE - (addr % SPI_FLASH_PAGE_SIZE);
    
    if(length <= startPageRemainSize)
    {
        startPageRemainSize = length;
        pageSize = 0;
        lastPageRemainSize = 0;
    }
    else
    {
        pageSize = (length-startPageRemainSize) / SPI_FLASH_PAGE_SIZE;
        lastPageRemainSize = (length-startPageRemainSize) % SPI_FLASH_PAGE_SIZE;
    }

    if(startPageRemainSize)
    {
        #if 1
        Drv_Spi_Flash_Dma_Write(addr, buf, startPageRemainSize);   
        #else
        Drv_Spi_Flash_Loop_Write(addr, buf, startPageRemainSize);
        #endif 

        addr += startPageRemainSize;

        buf += startPageRemainSize;
    }

    for(i=0;i<pageSize;i++)
    {
        #if 1
        Drv_Spi_Flash_Dma_Write(addr, buf, SPI_FLASH_PAGE_SIZE);    
        #else        
        Drv_Spi_Flash_Loop_Write(addr, buf, SPI_FLASH_PAGE_SIZE);
        #endif 
        addr += SPI_FLASH_PAGE_SIZE;

        buf += SPI_FLASH_PAGE_SIZE;
    }

    if(lastPageRemainSize)
    {
        #if 1
        Drv_Spi_Flash_Dma_Write(addr, buf, lastPageRemainSize);
        #else
        Drv_Spi_Flash_Loop_Write(addr, buf, lastPageRemainSize);
        #endif 
    }
}

void Drv_Spi_Flash_Read(uint32_t addr, uint8_t *buf, uint32_t length )
{
    Hal_Spi_Pin_Reuse();
    
    Drv_Spi_Flash_Wait_Idle();
    
    Hal_Spi_Flash_Cs_Enable();

    Hal_Spi_Flash_Single_Tx(FLASH_READ);

    Hal_Spi_Flash_Single_Tx((uint8_t )(addr>>16));
    Hal_Spi_Flash_Single_Tx((uint8_t )(addr>>8));
    Hal_Spi_Flash_Single_Tx((uint8_t )addr);

    Hal_Spi_Flash_Dma_Rx(buf, 2, Drv_Spi_Flash_Rx_Callback);
    while(spiFlashRxDoneFlag == 0);
    spiFlashRxDoneFlag = 0;
    
    Hal_Spi_Flash_Dma_Rx(&buf[2], length-2, Drv_Spi_Flash_Rx_Callback);
    while(spiFlashRxDoneFlag == 0);
    spiFlashRxDoneFlag = 0;


    Hal_Spi_Flash_Cs_Disable();
}

