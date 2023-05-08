/********************************************************
* @file       hal_interrupt.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_nuc123.h"
#include "hal_task.h"
#include "hal_timer.h"
#include "hal_spi_flash.h"
#include "hal_lcd.h"
#include "hal_usb.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
extern uint8_t testFramMs;

void SysTick_Handler(void )
{
    testFramMs++;
    Hal_Task_Isr_Handler();
}

void TMR0_IRQHandler(void )
{
    TIMER0->TISR |= TIMER_TISR_TIF_Msk;

    Hal_Timer_Isr_Handler();
}

void PDMA_IRQHandler(void)
{
    uint32_t status = PDMA_GET_INT_STATUS();

    if(status & 0x02)
    {
        if(PDMA_GET_CH_INT_STS(1) & 0x2)
        {
            Hal_Lcd_Spi_Isr_Handler();
        }
        
        PDMA_CLR_CH_INT_FLAG(1, PDMA_ISR_BLKD_IF_Msk);
    }
    else if(status & 0x4)    /* CH0 */
    {        
        if(PDMA_GET_CH_INT_STS(2) & 0x2)
        {
            Hal_Spi_Flash_Tx_Isr_Handler();
        }
        
        PDMA_CLR_CH_INT_FLAG(2, PDMA_ISR_BLKD_IF_Msk);
    }
    else if(status & 0x8)      /* CH1 */
    {        
        if(PDMA_GET_CH_INT_STS(3) & 0x2)
        {
            Hal_Spi_Flash_Rx_Isr_Handler();
        }
        
        PDMA_CLR_CH_INT_FLAG(3, PDMA_ISR_BLKD_IF_Msk);
    }
}

void USBD_IRQHandler(void )
{
    Hal_Usb_Isr_Handler();
}


