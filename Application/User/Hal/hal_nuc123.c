/********************************************************
* @file       hal_nuc123.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_nuc123.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Hardware_Init(void )
{
    Clk_Init();

    Systick_Init();

    Gpio_Init();
}

void Clk_Init(void )
{
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    SYS_UnlockReg();
    /* Enable XT1_OUT(PF0) and XT1_IN(PF1) */
    SYS->GPF_MFP |= SYS_GPF_MFP_PF0_XT1_OUT | SYS_GPF_MFP_PF1_XT1_IN;

    /* Enable Internal RC 22.1184MHz clock */
    CLK->PWRCON |= CLK_PWRCON_OSC22M_EN_Msk;

    /* Waiting for Internal RC clock ready */
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_OSC22M_STB_Msk));

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLK_S_Msk)) | CLK_CLKSEL0_HCLK_S_HIRC;
    CLK->CLKDIV = (CLK->CLKDIV & (~CLK_CLKDIV_HCLK_N_Msk)) | CLK_CLKDIV_HCLK(1);

    /* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware */
    CLK->PLLCON |= CLK_PLLCON_PD_Msk;

    /* Enable external XTAL 12MHz clock */
    CLK->PWRCON |= CLK_PWRCON_XTL12M_EN_Msk | CLK_PWRCON_OSC10K_EN_Msk;

    /* Waiting for external XTAL clock ready */
    while((CLK->CLKSTATUS & (CLK_CLKSTATUS_XTL12M_STB_Msk | CLK_CLKSTATUS_OSC10K_STB_Msk)) != (CLK_CLKSTATUS_XTL12M_STB_Msk | CLK_CLKSTATUS_OSC10K_STB_Msk));

    /* Set core clock as PLL_CLOCK from PLL */
    CLK->PLLCON = 0xc28e;
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB_Msk));
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    CLK->CLKDIV = (CLK->CLKDIV & ~CLK_CLKDIV_USB_N_Msk) | (2 << CLK_CLKDIV_USB_N_Pos);
    CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLK_S_Msk)) | CLK_CLKSEL0_HCLK_S_PLL_DIV2;

    CLK->CLKSEL0 = (CLK->CLKSEL0 & ~CLK_CLKSEL0_STCLK_S_Msk) | CLK_CLKSEL0_STCLK_S_HCLK;

    /* Enable FMC ISP functions */
    FMC->ISPCON |=  FMC_ISPCON_ISPEN_Msk | FMC_ISPCON_APUEN_Msk | FMC_ISPCON_LDUEN_Msk | FMC_ISPCON_CFGUEN_Msk;

    SystemCoreClockUpdate();

}

void Systick_Init(void )
{
    SysTick->LOAD = 72000;
    SysTick->VAL  = (0x00);
    SysTick->CTRL |= (1 << 2) | (1 << 1) | (1 << 0);
}

void Gpio_Init(void )
{
    //PA15 : input,mouse DPI- button
    //PA11 : push pull out,LED3 switch
    //PA10 : push pull out,LED4 switch
    PA->PMD = (PA->PMD & ~GPIO_PMD_PMD15_Msk) | (0 << GPIO_PMD_PMD15_Pos);
    PA->PMD = (PA->PMD & ~GPIO_PMD_PMD11_Msk) | (1 << GPIO_PMD_PMD11_Pos);
    PA->PMD = (PA->PMD & ~GPIO_PMD_PMD10_Msk) | (1 << GPIO_PMD_PMD10_Pos);
    PA->DOUT |= (1 << 11);
    PA->DOUT |= (1 << 10);

    //PB14: push pull out,LED2 switch
    //PB9 : input,mouse fire button
    //PB8 : push pull out,LED1 switch
    //PB5 : push pull out,LED6
    //PB4 : push pull out,LED5 switch
    PB->PMD = (PB->PMD & ~GPIO_PMD_PMD14_Msk) | (1 << GPIO_PMD_PMD14_Pos);
    PB->PMD = (PB->PMD & ~GPIO_PMD_PMD9_Msk) | (0 << GPIO_PMD_PMD9_Pos);
    PB->PMD = (PB->PMD & ~GPIO_PMD_PMD8_Msk) | (1 << GPIO_PMD_PMD8_Pos);
    PB->PMD = (PB->PMD & ~GPIO_PMD_PMD5_Msk) | (1 << GPIO_PMD_PMD5_Pos);
    PB->PMD = (PB->PMD & ~GPIO_PMD_PMD4_Msk) | (1 << GPIO_PMD_PMD4_Pos);
    PB->DOUT |= (1 << 14);
    PB->DOUT |= (1 << 8);
    PB->DOUT |= (1 << 5);
    PB->DOUT |= (1 << 4);

    //PC13: input,mouse left button
    //PC12: input,mouse right button
    //PC11: input,mouse middle button
    //PC10: input,mouse forward button
    //PC9 : input,mouse back button
    //PC8 : input,mouse DPI+ button
    //PC5 : push pull out,TFT background led
    //PC4 : input,mouse report rate button
    //PC0 : push pull out,BY25D40 NCS/WP
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD13_Msk) | (0 << GPIO_PMD_PMD13_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD12_Msk) | (0 << GPIO_PMD_PMD12_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD11_Msk) | (0 << GPIO_PMD_PMD11_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD10_Msk) | (0 << GPIO_PMD_PMD10_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD9_Msk) | (0 << GPIO_PMD_PMD9_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD8_Msk) | (0 << GPIO_PMD_PMD8_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD5_Msk) | (1 << GPIO_PMD_PMD5_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD4_Msk) | (0 << GPIO_PMD_PMD4_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD0_Msk) | (1 << GPIO_PMD_PMD0_Pos);
    PC->DOUT |= (1 << 5);
    PC->DOUT |= (1 << 0);

    //PD5: push pull out,TFT CMD/DATA
    //PD4: push pull out,TFT Reset
    PD->PMD = (PD->PMD & ~GPIO_PMD_PMD5_Msk) | (1 << GPIO_PMD_PMD5_Pos);
    PD->PMD = (PD->PMD & ~GPIO_PMD_PMD4_Msk) | (1 << GPIO_PMD_PMD4_Pos);

    //PF3: input,wheel detect A
    //PF2: input,wheel detect B
    PF->PMD = (PF->PMD & ~GPIO_PMD_PMD3_Msk) | (0x00 << GPIO_PMD_PMD3_Pos);
    PF->PMD = (PF->PMD & ~GPIO_PMD_PMD2_Msk) | (0x00 << GPIO_PMD_PMD2_Pos);
}

void Gpio_Intp_Init(void )
{
    //fall edge triggered interrupt to wake-up chip when chip in idle/power down mode
    //PC13: input,mouse left button
    //PC12: input,mouse right button
    //PC11: input,mouse middle button

    PC->IMD = (PC->IMD & ~GPIO_IMD_IMD_Msk) | (0 << 13) | (0 << 12) | (0 << 11);

    PC->IEN = (PC->IEN & ~GPIO_IEN_IF_EN_Msk) | (1 << 13) | (1 << 12) | (1 << 11);

    NVIC_EnableIRQ(GPCDF_IRQn);

    /* Enable interrupt de-bounce function and select de-bounce sampling cycle time is 1024 clocks of LIRC clock */
    GPIO->DBNCECON = (GPIO_DBNCECON_ICLK_ON_Msk | GPIO_DBCLKSRC_LIRC | GPIO_DBCLKSEL_1024);
    PC->DBEN |= (1 << 13) | (1 << 12) | (1 << 11);
}

void Timer0_Init(void )
{
    CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_TMR0_S_Msk) | CLK_CLKSEL1_TMR0_S_HXT;

    CLK->APBCLK |= CLK_APBCLK_TMR0_EN_Msk;

    TIMER0->TCSR = (TIMER0->TCSR & ~TIMER_TCSR_PRESCALE_Msk) | ((12 -1) << TIMER_TCSR_PRESCALE_Pos);

    TIMER0->TCSR = (TIMER0->TCSR & ~TIMER_TCSR_MODE_Msk) | (1 << TIMER_TCSR_MODE_Pos);

    TIMER0->TCMPR = 1000;

    TIMER0->TISR |= TIMER_TISR_TIF_Msk;

    TIMER0->TCSR |= TIMER_TCSR_IE_Msk;

    NVIC_EnableIRQ(TMR0_IRQn);

    TIMER0->TCSR |= TIMER_TCSR_CEN_Msk;
}

void Timer1_Init(void )
{
    CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_TMR1_S_Msk) | CLK_CLKSEL1_TMR1_S_HXT;

    CLK->APBCLK |= CLK_APBCLK_TMR1_EN_Msk;

    TIMER1->TCSR = (TIMER1->TCSR & ~TIMER_TCSR_PRESCALE_Msk) | ((12 -1) << TIMER_TCSR_PRESCALE_Pos);

    TIMER1->TCSR = (TIMER1->TCSR & ~TIMER_TCSR_MODE_Msk) | (1 << TIMER_TCSR_MODE_Pos);

    TIMER1->TCMPR = 1000;

    TIMER1->TISR |= TIMER_TISR_TIF_Msk;

    TIMER1->TCSR |= TIMER_TCSR_IE_Msk;

    NVIC_EnableIRQ(TMR1_IRQn);

    TIMER1->TCSR |= TIMER_TCSR_CEN_Msk;
}

void Spi0_Init(void )
{
    CLK->CLKSEL1 = CLK->CLKSEL1 & (~CLK_CLKSEL1_SPI0_S_Msk) | (0 << CLK_CLKSEL1_SPI0_S_Pos);

    SYS->GPB_MFP = SYS_GPB_MFP_PB10_GPIO;
    SYS->ALT_MFP = SYS_ALT_MFP_PB10_GPIO;
    /* Setup SPI1 multi-function pins */
    SYS->GPC_MFP = SYS_GPC_MFP_PC1_SPI0_CLK | SYS_GPC_MFP_PC2_SPI0_MISO0 | SYS_GPC_MFP_PC3_SPI0_MOSI0;
    SYS->ALT_MFP = SYS_ALT_MFP_PC1_SPI0_CLK | SYS_ALT_MFP_PC2_SPI0_MISO0 | SYS_ALT_MFP_PC3_SPI0_MOSI0;

    CLK->APBCLK |= CLK_APBCLK_SPI0_EN_Msk;

    SPI0->CNTRL = (SPI0->CNTRL & ~SPI_CNTRL_TX_BIT_LEN_Msk) | (8 << SPI_CNTRL_TX_BIT_LEN_Pos);

    SPI0->CNTRL = SPI0->CNTRL & ~SPI_CNTRL_SLAVE_Msk;

    SPI0->CNTRL = (SPI0->CNTRL & ~(SPI_CNTRL_TX_NEG_Msk|SPI_CNTRL_RX_NEG_Msk)) | SPI_CNTRL_TX_NEG_Msk;

    SPI0->CNTRL &= ~SPI_CNTRL_LSB_Msk;

    SPI0->SSR = SPI0->SSR & ~(SPI_SSR_SS_LVL_Msk | SPI_SSR_AUTOSS_Msk);

    SPI0->SSR &= ~(1 << 0);

    SPI0->CNTRL2 |= SPI_CNTRL2_BCn_Msk;

    SPI0->DIVIDER = (SPI0->DIVIDER & (~SPI_DIVIDER_DIVIDER_Msk)) | 2; //SPI0_CLK = 144Mhz / 3 = 48Mhz
}

void Spi2_Init(void )
{
    CLK->CLKSEL1 = CLK->CLKSEL1 & (~CLK_CLKSEL1_SPI2_S_Msk) | (0 << CLK_CLKSEL1_SPI2_S_Pos);

    /* Setup SPI2 multi-function pins */
    SYS->GPD_MFP = SYS_GPD_MFP_PD0_SPI2_SS0 | SYS_GPD_MFP_PD1_SPI2_CLK | SYS_GPD_MFP_PD2_SPI2_MISO0 | SYS_GPD_MFP_PD3_SPI2_MOSI0;
    SYS->ALT_MFP1 = SYS_ALT_MFP1_PD0_SPI2_SS0 | SYS_ALT_MFP1_PD1_SPI2_CLK | SYS_ALT_MFP1_PD2_SPI2_MISO0 | SYS_ALT_MFP1_PD3_SPI2_MOSI0;

    CLK->APBCLK |= CLK_APBCLK_SPI2_EN_Msk;

    SPI2->CNTRL = (SPI2->CNTRL & ~SPI_CNTRL_TX_BIT_LEN_Msk) | (8 << SPI_CNTRL_TX_BIT_LEN_Pos);

    SPI2->CNTRL = SPI2->CNTRL & ~SPI_CNTRL_SLAVE_Msk;

    SPI2->CNTRL = (SPI2->CNTRL & ~(SPI_CNTRL_TX_NEG_Msk|SPI_CNTRL_RX_NEG_Msk)) | SPI_CNTRL_TX_NEG_Msk;

    SPI2->CNTRL &= ~SPI_CNTRL_LSB_Msk;

    #if 1

    SPI2->SSR = SPI2->SSR & ~(SPI_SSR_SS_LVL_Msk | SPI_SSR_AUTOSS_Msk);

    SPI2->SSR &= ~(1 << 0);

    #else

    SPI0->SSR |= SPI_SSR_AUTOSS_Msk;

    SPI2->SSR &= ~SPI_SSR_SS_LVL_Msk;

    SPI2->SSR |= (1 << 0);


    #endif 

    SPI2->CNTRL2 |= SPI_CNTRL2_BCn_Msk;

    SPI2->DIVIDER = (SPI2->DIVIDER & (~SPI_DIVIDER_DIVIDER_Msk)) | 2; //SPI2_CLK = 144Mhz / 3 = 48Mhz
}



void Pwm_Init(void )
{
    /* select PWM clock source */
    CLK->CLKSEL1 = (CLK->CLKSEL1 & ~(CLK_CLKSEL1_PWM01_S_Msk | CLK_CLKSEL1_PWM23_S_Msk)) | (CLK_CLKSEL1_PWM01_S_HCLK | CLK_CLKSEL1_PWM23_S_HCLK);
    CLK->CLKSEL2 = (CLK->CLKSEL2 & ~(CLK_CLKSEL2_PWM01_S_EXT_Msk | CLK_CLKSEL2_PWM23_S_EXT_Msk)) | (CLK_CLKSEL2_PWM01_EXT_HCLK | CLK_CLKSEL2_PWM23_EXT_HCLK);

    CLK->APBCLK |= (CLK_APBCLK_PWM01_EN_Msk | CLK_APBCLK_PWM23_EN_Msk);

    /* Setup SPI2 multi-function pins */
    SYS->GPA_MFP = SYS_GPA_MFP_PA12_PWM0 | SYS_GPA_MFP_PA13_PWM1 | SYS_GPA_MFP_PA14_PWM2;

    PWMA->PPR = (PWMA->PPR & ~PWM_PPR_CP01_Msk) | ((300-1) << PWM_PPR_CP01_Pos);//72000khz / 600 = 120khz
    PWMA->PPR = (PWMA->PPR & ~PWM_PPR_CP23_Msk) | ((300-1) << PWM_PPR_CP23_Pos);

    PWMA->CSR = (PWMA->CSR & ~PWM_CSR_CSR0_Msk) | (4 << PWM_CSR_CSR0_Pos);//pwm_clk = 120khz / 1 = 120khz
    PWMA->CSR = (PWMA->CSR & ~PWM_CSR_CSR1_Msk) | (4 << PWM_CSR_CSR1_Pos);
    PWMA->CSR = (PWMA->CSR & ~PWM_CSR_CSR2_Msk) | (4 << PWM_CSR_CSR2_Pos);

    PWMA->PCR |= PWM_PCR_CH0MOD_Msk | PWM_PCR_CH1MOD_Msk | PWM_PCR_CH2MOD_Msk ;
    PWMA->PCR |= PWM_PCR_CH0PINV_Msk | PWM_PCR_CH1PINV_Msk | PWM_PCR_CH2PINV_Msk;

    PWMA->CNR0 = 0xff; //PWM period is 1kHZ
    PWMA->CNR1 = 0xff;
    PWMA->CNR2 = 0xff;

    PWMA->CMR0 = 0x0;
    PWMA->CMR1 = 0x0;
    PWMA->CMR2 = 0x0;


    PWMA->POE |= PWM_POE_PWM0_Msk | PWM_POE_PWM1_Msk | PWM_POE_PWM2_Msk;

    PWMA->PCR |= PWM_PCR_CH0EN_Msk | PWM_PCR_CH1EN_Msk | PWM_PCR_CH2EN_Msk;

}



