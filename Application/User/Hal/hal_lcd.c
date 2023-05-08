/********************************************************
* @file       hal_lcd.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_lcd.h"

/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
#define LCD_SPI_TX_DMA_CHANNEL              1
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
static hal_isr_callback_t hal_lcd_isr_callback = NULL;
static uint8_t lcdSendCnt;


void Hal_Lcd_Init(void )
{
    Spi2_Init();
}

inline void Hal_Lcd_Spi_Cs_Enable(void )
{
    SPI2->SSR |= (1 << 0);
}

inline void Hal_Lcd_Spi_Cs_Disable(void )
{
    SPI2->SSR &= ~(1 << 0);
}

void Hal_Lcd_Spi_Single_Tx(uint8_t dat )
{
    SPI2->TX[0] = dat;
    SPI2->CNTRL |= SPI_CNTRL_GO_BUSY_Msk;
    while((SPI2->CNTRL & SPI_CNTRL_GO_BUSY_Msk) != 0);
}

void Hal_Lcd_Spi_Dma_Tx(const uint8_t *buf, uint16_t length, hal_isr_callback_t callback )
{
    /* Enable PDMA channel 0 clock */  
    PDMA_GCR->GCRCSR |= PDMA_GCRCSR_CLK1_EN_Msk;
    /* set transfer width is 8bit */
    PDMA1->CSR = (PDMA1->CSR & (~PDMA_CSR_APB_TWS_Msk)) | PDMA_WIDTH_8;
    /* set transfer direction */
    PDMA1->CSR = (PDMA1->CSR & (~PDMA_CSR_SAD_SEL_Msk)) | PDMA_SAR_INC;
    PDMA1->CSR = (PDMA1->CSR & (~PDMA_CSR_DAD_SEL_Msk)) | PDMA_DAR_FIX;
    PDMA1->CSR = (PDMA1->CSR & (~PDMA_CSR_MODE_SEL_Msk)) | (0x02 << PDMA_CSR_MODE_SEL_Pos);   

    PDMA_GCR->PDSSR0 = (PDMA_GCR->PDSSR0 & ~PDMA_PDSSR0_SPI2_TXSEL_Msk) | (LCD_SPI_TX_DMA_CHANNEL << PDMA_PDSSR0_SPI2_TXSEL_Pos);
    /* set transfer byte count(transfer width is 32) */
    PDMA1->BCR = length;

    PDMA1->SAR = (uint32_t)buf;
    PDMA1->DAR = (uint32_t)&SPI2->TX[0];

    /* enable block transfer done interrupt */
    PDMA1->IER |= PDMA_IER_BLKD_IE_Msk;
    NVIC_EnableIRQ(PDMA_IRQn);

    lcdSendCnt = 0;
    hal_lcd_isr_callback  = callback;
    
    SPI2->DMA |= SPI_DMA_TX_DMA_GO_Msk;

    /* trigger transfer */
    PDMA1->CSR |= (PDMA_CSR_TRIG_EN_Msk | PDMA_CSR_PDMACEN_Msk);
}

void Hal_Lcd_Spi_Isr_Handler(void )
{
    lcdSendCnt++;
    if(lcdSendCnt == 0)
    {
        #if 0
        static uint16_t color = 0x07E0;
        /* Enable PDMA channel 0 clock */  
        PDMA_GCR->GCRCSR |= PDMA_GCRCSR_CLK1_EN_Msk;
        /* set transfer width is 8bit */
        PDMA1->CSR = (PDMA1->CSR & (~PDMA_CSR_APB_TWS_Msk)) | PDMA_WIDTH_8;
        /* set transfer direction */
        PDMA1->CSR = (PDMA1->CSR & (~PDMA_CSR_SAD_SEL_Msk)) | PDMA_SAR_FIX;
        PDMA1->CSR = (PDMA1->CSR & (~PDMA_CSR_DAD_SEL_Msk)) | PDMA_DAR_FIX;
        PDMA1->CSR = (PDMA1->CSR & (~PDMA_CSR_MODE_SEL_Msk)) | (0x02 << PDMA_CSR_MODE_SEL_Pos);   

        PDMA_GCR->PDSSR0 = (PDMA_GCR->PDSSR0 & ~PDMA_PDSSR0_SPI2_TXSEL_Msk) | (LCD_SPI_TX_DMA_CHANNEL << PDMA_PDSSR0_SPI2_TXSEL_Pos);
        /* set transfer byte count(transfer width is 32) */
        PDMA1->BCR = 57600;

        PDMA1->SAR = (uint32_t)&color;
        PDMA1->DAR = (uint32_t)&SPI2->TX[0];

        /* enable block transfer done interrupt */
        PDMA1->IER |= PDMA_IER_BLKD_IE_Msk;
        NVIC_EnableIRQ(PDMA_IRQn);
        
        SPI2->DMA |= SPI_DMA_TX_DMA_GO_Msk;

        /* trigger transfer */
        PDMA1->CSR |= (PDMA_CSR_TRIG_EN_Msk | PDMA_CSR_PDMACEN_Msk);
        #else
        //PDMA1->BCR = 57600;
        //SPI2->DMA |= SPI_DMA_TX_DMA_GO_Msk;
        /* trigger transfer */
        //PDMA1->CSR |= (PDMA_CSR_TRIG_EN_Msk | PDMA_CSR_PDMACEN_Msk);
        #endif 

        lcdSendCnt++;
    }
    else
    {
        lcdSendCnt = 0;
        if(hal_lcd_isr_callback != NULL)
        {            
            SPI2->CNTRL = (SPI2->CNTRL & ~SPI_CNTRL_TX_BIT_LEN_Msk) | (8 << SPI_CNTRL_TX_BIT_LEN_Pos);
    
            SPI2->CNTRL &= ~SPI_CNTRL_LSB_Msk;
            
            hal_lcd_isr_callback();
        }
    }
}

