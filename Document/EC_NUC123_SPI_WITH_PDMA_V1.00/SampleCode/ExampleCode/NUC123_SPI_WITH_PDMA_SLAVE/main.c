/******************************************************************************
 * @file     main.c
 * @brief
 *           Configure SPI0 as slave mode and demonstrate how to communicate
 *           with an off-chip SPI Slave device with PDMA. This sample
 *           code needs to work with SPI_MASTER_WITH_PDMA sample code.
 *
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include "NUC123.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Define                                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/

#define TEST_COUNT             64
#define DMA_SPI1_TX 2
#define DMA_SPI1_RX 3

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

uint32_t g_au32SourceData[TEST_COUNT];
uint32_t g_au32DestinationData[TEST_COUNT];
volatile uint32_t PDMA_TEST_LENGTH = TEST_COUNT;
volatile uint8_t SrcArray[TEST_COUNT];
volatile uint8_t DestArray[TEST_COUNT];
uint32_t volatile u32IsTestOver = 0xFF;
unsigned char u32RXFinFlag;

/* Function prototype declaration */
void SYS_Init(void);
void SPI_Init(void);

int main(void)
{
    uint32_t u32DataCount, u32ErrCount = 0;

    /* Unlock protected registers */
    SYS_UnlockReg();

    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Configure UART0: 115200, 8-bit word, no parity bit, 1 stop bit. */
    UART_Open(UART0, 115200);

    printf("\n\n");
    printf("+--------------------------------------------------------------+\n");
    printf("|                  SPI Slave with PDMA Sample Code            |\n");
    printf("+--------------------------------------------------------------+\n");
    printf("\n");
    printf("Configure SPI1 as a slave.\n");
    printf("Bit length of a transaction: 32\n");
    printf("The I/O connection for SPI1:\n");
    printf("    SPI1_SS(PB12)\n    SPI1_CLK(PA11)\n");
    printf("    SPI1_MISO(PA10)\n    SPI1_MOSI(PD8)\n\n");
    printf("SPI controller will enable FIFO mode and transfer %d data to a off-chip master device.\n", TEST_COUNT);
    printf("In the meanwhile the SPI controller will receive %d data from the off-chip master device.\n", TEST_COUNT);

    /* Init parameter */
    u32RXFinFlag = 0;

    /* Source data initiation */
    for (u32DataCount = 0; u32DataCount < TEST_COUNT; u32DataCount++)
    {
        SrcArray[u32DataCount] = u32DataCount;
        DestArray[u32DataCount] = 0;
    }

    /* Set PA15 in output mode*/
    GPIO_SetMode(PA, BIT15, GPIO_PMD_OUTPUT);

    /* Init SPI */
    SPI_Init();
    SPI1->CNTRL |= SPI_CNTRL_FIFO_Msk;

    printf("Press any key to let slave configuration be ready.\n") ;
    getchar() ;

    /* Set SPI TX with PDMA configuration */
    PDMA_Open(1 << DMA_SPI1_TX);
    PDMA_SetTransferCnt(DMA_SPI1_TX, PDMA_WIDTH_8, PDMA_TEST_LENGTH);
    PDMA_SetTransferAddr(DMA_SPI1_TX, (uint32_t)SrcArray, PDMA_SAR_INC, (uint32_t)&SPI1->TX[0], PDMA_DAR_FIX);
    PDMA_EnableInt(DMA_SPI1_TX, PDMA_IER_BLKD_IE_Msk);
    PDMA2->CSR = ((PDMA2->CSR & ~(PDMA_CSR_MODE_SEL_Msk)) | (0x2 << PDMA_CSR_MODE_SEL_Pos));

    /* Set SPI RX with PDMA configuration */
    PDMA_Open(1 << DMA_SPI1_RX);
    PDMA_SetTransferCnt(DMA_SPI1_RX, PDMA_WIDTH_8, PDMA_TEST_LENGTH);
    PDMA_SetTransferAddr(DMA_SPI1_RX, (uint32_t)&SPI1->RX[0], PDMA_SAR_FIX, (uint32_t)DestArray, PDMA_DAR_INC);
    PDMA_EnableInt(DMA_SPI1_RX, PDMA_IER_BLKD_IE_Msk);
    PDMA3->CSR = ((PDMA3->CSR & ~(PDMA_CSR_MODE_SEL_Msk)) | (0x1 << PDMA_CSR_MODE_SEL_Pos));
    PDMA_GCR->PDSSR0 = 0x0FF2301;

    NVIC_EnableIRQ(PDMA_IRQn);

    /* Enable PDMA trigger function */
    PDMA_Trigger(DMA_SPI1_RX);
    PDMA_Trigger(DMA_SPI1_TX);

    /* Enable SPI slave PDMA function */
    SPI_TRIGGER_RX_PDMA(SPI1);
    SPI_TRIGGER_TX_PDMA(SPI1);

    /* Sync with Master */
    PA15 = 1 ;

    while (u32RXFinFlag == 0) ;

    for (u32DataCount = 0; u32DataCount < TEST_COUNT; u32DataCount++)
    {
        if (SrcArray[u32DataCount] != DestArray[u32DataCount])
            u32ErrCount++;
    }

    printf("\nSPI1 Slave with PDMA test ");

    if (u32ErrCount == 0)
        printf(" [Pass]\n");
    else
        printf(" [Fail]\n");

    while (1) ;

}

void SYS_Init(void)
{

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable XT1_OUT (PF0) and XT1_IN (PF1) */
    SYS->GPF_MFP |= SYS_GPF_MFP_PF0_XT1_OUT | SYS_GPF_MFP_PF1_XT1_IN;

    /* Enable external 12MHz XTAL */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Switch HCLK clock source to HXT and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HXT, CLK_CLKDIV_HCLK(1));

    /* Select HXT as the clock source of UART0 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));

    /* Select HCLK as the clock source of SPI0 */
    CLK_SetModuleClock(SPI1_MODULE, CLK_CLKSEL1_SPI1_S_HCLK, MODULE_NoMsk);

    /* Enable UART peripheral clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Enable SPI0 peripheral clock */
    CLK_EnableModuleClock(SPI1_MODULE);

    /* Enable PDMA module clock */
    CLK->AHBCLK |= CLK_AHBCLK_PDMA_EN_Msk;

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set PB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFP = SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD;

    /* Setup SPI0 multi-function pins */
    //    SYS->GPC_MFP = SYS_GPC_MFP_PC8_SPI1_SS0 | SYS_GPC_MFP_PC9_SPI1_CLK | SYS_GPC_MFP_PC10_SPI1_MISO0 | SYS_GPC_MFP_PC11_SPI1_MOSI0;
    //    SYS->ALT_MFP = SYS_ALT_MFP_PC8_SPI1_SS0 | SYS_ALT_MFP_PC9_SPI1_CLK | SYS_ALT_MFP_PC10_SPI1_MISO0 | SYS_ALT_MFP_PC11_SPI1_MOSI0;

    SYS->GPB_MFP |= SYS_GPB_MFP_PB12_SPI1_SS0 ;
    SYS->GPA_MFP = SYS_GPA_MFP_PA11_SPI1_CLK | SYS_GPA_MFP_PA10_SPI1_MISO0;
    SYS->GPD_MFP = SYS_GPD_MFP_PD8_SPI1_MOSI0 ;
    SYS->ALT_MFP = SYS_ALT_MFP_PB12_SPI1_SS0 | SYS_ALT_MFP_PA11_SPI1_CLK | SYS_ALT_MFP_PA10_SPI1_MISO0 | SYS_ALT_MFP_PD8_SPI1_MOSI0;

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and CyclesPerUs automatically. */
    SystemCoreClockUpdate();
}

void SPI_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init SPI                                                                                                */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Configure as a master, clock idle low, 32-bit transaction, drive output on falling clock edge and latch input on rising edge. */
    /* Set IP clock divider. SPI clock rate = 2MHz */
    SPI_Open(SPI1, SPI_SLAVE, SPI_MODE_0, 8, 36000000);

    /* Enable the automatic hardware slave select function. Select the SS pin and configure as low-active. */
    SPI_EnableAutoSS(SPI1, SPI_SS0, SPI_SS_ACTIVE_LOW);
}


void PDMA_IRQHandler(void)
{
    uint32_t status = PDMA_GET_INT_STATUS();

    if (status & 0x1)   /* CH0 */
    {
        if (PDMA_GET_CH_INT_STS(0) & 0x2)
            PDMA_CLR_CH_INT_FLAG(0, PDMA_ISR_BLKD_IF_Msk);
    }
    else if (status & 0x2)     /* CH1 */
    {
        if (PDMA_GET_CH_INT_STS(1) & 0x2)
            PDMA_CLR_CH_INT_FLAG(1, PDMA_ISR_BLKD_IF_Msk);
    }
    else if (status & 0x4)     /* CH2 */
    {
        if (PDMA_GET_CH_INT_STS(2) & 0x2)
        {
            //            PA15 = 1;
            PDMA_CLR_CH_INT_FLAG(2, PDMA_ISR_BLKD_IF_Msk);
        }
    }
    else if (status & 0x8)     /* CH3 */
    {
        if (PDMA_GET_CH_INT_STS(3) & 0x2)
        {
            u32RXFinFlag = 1;
            PDMA_CLR_CH_INT_FLAG(3, PDMA_ISR_BLKD_IF_Msk);
        }
    }
    else if (status & 0x10)     /* CH4 */
    {
        if (PDMA_GET_CH_INT_STS(4) & 0x2)
            PDMA_CLR_CH_INT_FLAG(4, PDMA_ISR_BLKD_IF_Msk);
    }
    else if (status & 0x20)     /* CH5 */
    {
        if (PDMA_GET_CH_INT_STS(5) & 0x2)
            PDMA_CLR_CH_INT_FLAG(5, PDMA_ISR_BLKD_IF_Msk);
    }
    else
        printf("unknown interrupt !!\n");
}

/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/

