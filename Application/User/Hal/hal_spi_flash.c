/********************************************************
* @file       hal_spi_flash.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_spi_flash.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
#define SPI_TX_CHANNEL                2
#define SPI_RX_CHANNEL                3
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
hal_isr_callback_t hal_spi_flash_tx_isr_callback = NULL;
hal_isr_callback_t hal_spi_flash_rx_isr_callback = NULL;

void Hal_Spi_Flash_Init(void )
{
    Spi0_Init();
}

void Hal_Spi_Pin_Reuse(void )
{
    SYS->GPB_MFP = SYS_GPB_MFP_PB10_GPIO;
    SYS->ALT_MFP = SYS_ALT_MFP_PB10_GPIO;
    /* Setup SPI0 multi-function pins */
    SYS->GPC_MFP = SYS_GPC_MFP_PC1_SPI0_CLK | SYS_GPC_MFP_PC2_SPI0_MISO0 | SYS_GPC_MFP_PC3_SPI0_MOSI0;
    SYS->ALT_MFP = SYS_ALT_MFP_PC1_SPI0_CLK | SYS_ALT_MFP_PC2_SPI0_MISO0 | SYS_ALT_MFP_PC3_SPI0_MOSI0;
}

inline void Hal_Spi_Flash_Cs_Enable(void )
{
    PB->DOUT &= ~(1 << 10);

    PC->DOUT |= (1 << 0);
}

inline void Hal_Spi_Flash_Cs_Disable(void )
{
    PB->DOUT |= (1 << 10);
}

void Hal_Spi_Flash_Single_Tx(uint8_t dat )
{
    SPI0->TX[0] = dat;

    SPI0->CNTRL |= SPI_CNTRL_GO_BUSY_Msk;
    while((SPI0->CNTRL & SPI_CNTRL_GO_BUSY_Msk) != 0);
}

uint8_t Hal_Spi_Flash_Single_Rx(void )
{
    SPI0->TX[0] = 0xff;

    SPI0->CNTRL |= SPI_CNTRL_GO_BUSY_Msk;
    while((SPI0->CNTRL & SPI_CNTRL_GO_BUSY_Msk) != 0);

    return SPI0->RX[0];
}

void Hal_Spi_Flash_Dma_Tx(uint8_t *buf, uint32_t length, hal_isr_callback_t callback )
{
    /* Enable PDMA channel 0 clock */  
    PDMA_GCR->GCRCSR |= PDMA_GCRCSR_CLK2_EN_Msk;
    /* set transfer width is 8bit */
    PDMA2->CSR = (PDMA2->CSR & (~PDMA_CSR_APB_TWS_Msk)) | PDMA_WIDTH_8;
    /* set transfer direction */
    PDMA2->CSR = (PDMA2->CSR & (~PDMA_CSR_SAD_SEL_Msk)) | PDMA_SAR_INC;
    PDMA2->CSR = (PDMA2->CSR & (~PDMA_CSR_DAD_SEL_Msk)) | PDMA_DAR_FIX;
    PDMA2->CSR = (PDMA2->CSR & (~PDMA_CSR_MODE_SEL_Msk)) | (0x02 << PDMA_CSR_MODE_SEL_Pos);   

    PDMA_GCR->PDSSR0 = (PDMA_GCR->PDSSR0 & ~PDMA_PDSSR0_SPI0_TXSEL_Msk) | (SPI_TX_CHANNEL << PDMA_PDSSR0_SPI0_TXSEL_Pos);
    /* set transfer byte count(transfer width is 32) */
    PDMA2->BCR = length;

    PDMA2->SAR = (uint32_t)buf;
    PDMA2->DAR = (uint32_t)&SPI0->TX[0];

    /* enable block transfer done interrupt */
    PDMA2->IER |= PDMA_IER_BLKD_IE_Msk;
    NVIC_EnableIRQ(PDMA_IRQn);

    hal_spi_flash_tx_isr_callback  = callback;
    
    SPI0->DMA |= SPI_DMA_TX_DMA_GO_Msk;

    /* trigger transfer */
    PDMA2->CSR |= (PDMA_CSR_TRIG_EN_Msk | PDMA_CSR_PDMACEN_Msk);

}

void Hal_Spi_Flash_Dma_Rx(uint8_t *buf, uint32_t length, hal_isr_callback_t callback )
{
    #if 1
    /* Enable PDMA channel 0 clock */  
    PDMA_GCR->GCRCSR |= PDMA_GCRCSR_CLK3_EN_Msk;
    /* set transfer width is 8bit */
    PDMA3->CSR = (PDMA3->CSR & (~PDMA_CSR_APB_TWS_Msk)) | PDMA_WIDTH_8;
    /* set transfer direction */
    PDMA3->CSR = (PDMA3->CSR & (~PDMA_CSR_SAD_SEL_Msk)) | PDMA_SAR_FIX;
    PDMA3->CSR = (PDMA3->CSR & (~PDMA_CSR_DAD_SEL_Msk)) | PDMA_DAR_INC;
    PDMA3->CSR = (PDMA3->CSR & (~PDMA_CSR_MODE_SEL_Msk)) | (0x01 << PDMA_CSR_MODE_SEL_Pos);   

    PDMA_GCR->PDSSR0 = (PDMA_GCR->PDSSR0 & ~PDMA_PDSSR0_SPI0_RXSEL_Msk) | (SPI_RX_CHANNEL << PDMA_PDSSR0_SPI0_RXSEL_Pos);
    /* set transfer byte count(transfer width is 32) */
    PDMA3->BCR = length;

    PDMA3->SAR = (uint32_t)&SPI0->RX[0];
    PDMA3->DAR = (uint32_t)buf;

    /* enable block transfer done interrupt */
    PDMA3->IER |= PDMA_IER_BLKD_IE_Msk;
    NVIC_EnableIRQ(PDMA_IRQn);

    hal_spi_flash_rx_isr_callback  = callback;

    SPI0->DMA |= SPI_DMA_RX_DMA_GO_Msk;
   
    /* trigger transfer */
    PDMA3->CSR |= (PDMA_CSR_TRIG_EN_Msk | PDMA_CSR_PDMACEN_Msk);

    SPI0->CNTRL |= SPI_CNTRL_GO_BUSY_Msk;
    #else
    uint8_t dummy = 0xff;
    /* Enable PDMA channel 0 clock */  
    PDMA_GCR->GCRCSR |= PDMA_GCRCSR_CLK2_EN_Msk;
    /* set transfer width is 8bit */
    PDMA2->CSR = (PDMA2->CSR & (~PDMA_CSR_APB_TWS_Msk)) | PDMA_WIDTH_8;
    /* set transfer direction */
    PDMA2->CSR = PDMA2->CSR & (~PDMA_CSR_SAD_SEL_Msk) | PDMA_SAR_FIX;
    PDMA2->CSR = PDMA2->CSR & (~PDMA_CSR_DAD_SEL_Msk) | PDMA_DAR_FIX;
    PDMA2->CSR = PDMA2->CSR & (~PDMA_CSR_MODE_SEL_Msk) | (0x02 << PDMA_CSR_MODE_SEL_Pos);   

    PDMA_GCR->PDSSR0 = (PDMA_GCR->PDSSR0 & ~PDMA_PDSSR0_SPI0_TXSEL_Msk) | (SPI_TX_CHANNEL << PDMA_PDSSR0_SPI0_TXSEL_Pos);
    /* set transfer byte count(transfer width is 32) */
    PDMA2->BCR = length;

    PDMA2->SAR = (uint32_t)&dummy;
    PDMA2->DAR = (uint32_t)&SPI0->TX[0];
    /* enable block transfer done interrupt */
    PDMA2->IER |= PDMA_IER_BLKD_IE_Msk;



    /* Enable PDMA channel 0 clock */  
    PDMA_GCR->GCRCSR |= PDMA_GCRCSR_CLK3_EN_Msk;
    /* set transfer width is 8bit */
    PDMA3->CSR = (PDMA3->CSR & (~PDMA_CSR_APB_TWS_Msk)) | PDMA_WIDTH_8;
    /* set transfer direction */
    PDMA3->CSR = PDMA3->CSR & (~PDMA_CSR_SAD_SEL_Msk) | PDMA_SAR_FIX;
    PDMA3->CSR = PDMA3->CSR & (~PDMA_CSR_DAD_SEL_Msk) | PDMA_DAR_INC;
    PDMA3->CSR = PDMA3->CSR & (~PDMA_CSR_MODE_SEL_Msk) | (0x01 << PDMA_CSR_MODE_SEL_Pos);   

    PDMA_GCR->PDSSR0 = (PDMA_GCR->PDSSR0 & ~PDMA_PDSSR0_SPI0_RXSEL_Msk) | (SPI_RX_CHANNEL << PDMA_PDSSR0_SPI0_RXSEL_Pos);
    /* set transfer byte count(transfer width is 32) */
    PDMA3->BCR = length;

    PDMA3->SAR = (uint32_t)&SPI0->RX[0];
    PDMA3->DAR = (uint32_t)buf;

    /* enable block transfer done interrupt */
    PDMA3->IER |= PDMA_IER_BLKD_IE_Msk;
    
    NVIC_EnableIRQ(PDMA_IRQn);

    hal_spi_flash_rx_isr_callback  = callback;

    /* trigger transfer */
    PDMA2->CSR |= (PDMA_CSR_TRIG_EN_Msk | PDMA_CSR_PDMACEN_Msk);
    /* trigger transfer */
    PDMA3->CSR |= (PDMA_CSR_TRIG_EN_Msk | PDMA_CSR_PDMACEN_Msk);

    SPI0->DMA |= SPI_DMA_RX_DMA_GO_Msk;    
    SPI0->DMA |= SPI_DMA_TX_DMA_GO_Msk;
    
    #endif 
}

void Hal_Spi_Flash_Tx_Isr_Handler(void )
{
    if(hal_spi_flash_tx_isr_callback != NULL)
    {
        hal_spi_flash_tx_isr_callback();
    }
}

void Hal_Spi_Flash_Rx_Isr_Handler(void )
{
    if(hal_spi_flash_rx_isr_callback != NULL)
    {
        hal_spi_flash_rx_isr_callback();
    }
}

