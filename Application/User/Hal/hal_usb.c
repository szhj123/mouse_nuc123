/********************************************************
* @file       ha_usb.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_usb.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
usb_isr_callback_t *usb_callback = NULL;


void Hal_Usb_Init(void )
{
    CLK->APBCLK |= CLK_APBCLK_USBD_EN_Msk;

    USBD->ATTR |= USBD_ATTR_BYTEM_Msk | USBD_ATTR_PWRDN_Msk  | USBD_ATTR_DPPU_EN_Msk |\
                  USBD_ATTR_PHY_EN_Msk;

    USBD->INTEN |= USBD_INTEN_WAKEUP_EN_Msk | USBD_INTEN_WAKEUP_IE_Msk | USBD_INTEN_FLDET_IE_Msk |\
                   USBD_INTEN_USB_IE_Msk | USBD_INTEN_BUS_IE_Msk;
                   
    USBD->ATTR |= USBD_ATTR_USB_EN_Msk;
                   
    USBD->DRVSE0 |= USBD_DRVSE0_DRVSE0_Msk;

    USBD->FADDR = 0x0;

    USBD->STBUFSEG = 0;

    USBD->INTSTS |= USBD_INTSTS_SETUP_Msk | USBD_INTSTS_WAKEUP_STS_Msk | USBD_INTSTS_FLDET_STS_Msk |\
                    USBD_INTSTS_USB_STS_Msk | USBD_INTSTS_BUS_STS_Msk;

    USBD->DRVSE0 &= ~USBD_DRVSE0_DRVSE0_Msk;

    NVIC_SetPriority(USBD_IRQn, 1);
    
    NVIC_EnableIRQ(USBD_IRQn);
}

void Hal_Usb_Clr_Stall(void )
{
    USBD->EP[0].CFGP |= USBD_CFGP_SSTALL_Msk;
    USBD->EP[1].CFGP |= USBD_CFGP_SSTALL_Msk;
}

void Hal_Usb_Regist_Isr_Hanlder(usb_isr_callback_t *pCallback )
{
    usb_callback = pCallback;
}

void Hal_Usb_Ep_Config(void )
{
    USBD->STBUFSEG = 0;

    USBD->EP[0].CFG |= USBD_CFG_CSTALL_Msk;
    USBD->EP[0].CFG = (USBD->EP[0].CFG & ~USBD_CFG_STATE_Msk) | (EP_STATE_IN << USBD_CFG_STATE_Pos);
    USBD->EP[0].CFG = (USBD->EP[0].CFG & ~USBD_CFG_EP_NUM_Msk) | (0 << USBD_CFG_EP_NUM_Pos);
    USBD->EP[0].BUFSEG = USB_EP0_BUF_ADDR;

    USBD->EP[1].CFG |= USBD_CFG_CSTALL_Msk;
    USBD->EP[1].CFG = (USBD->EP[1].CFG & ~USBD_CFG_STATE_Msk) | (EP_STATE_OUT << USBD_CFG_STATE_Pos);
    USBD->EP[1].CFG = (USBD->EP[1].CFG & ~USBD_CFG_EP_NUM_Msk) | (0 << USBD_CFG_EP_NUM_Pos);
    USBD->EP[1].BUFSEG = USB_EP1_BUF_ADDR;

    USBD->EP[2].CFG = (USBD->EP[2].CFG & ~USBD_CFG_STATE_Msk) | (EP_STATE_IN << USBD_CFG_STATE_Pos);
    USBD->EP[2].CFG = (USBD->EP[2].CFG & ~USBD_CFG_EP_NUM_Msk) | (1 << USBD_CFG_EP_NUM_Pos);
    USBD->EP[2].BUFSEG = USB_EP2_BUF_ADDR;

    USBD->EP[3].CFG = (USBD->EP[3].CFG & ~USBD_CFG_STATE_Msk) | (EP_STATE_IN << USBD_CFG_STATE_Pos);
    USBD->EP[3].CFG = (USBD->EP[3].CFG & ~USBD_CFG_EP_NUM_Msk) | (2 << USBD_CFG_EP_NUM_Pos);
    USBD->EP[3].BUFSEG = USB_EP3_BUF_ADDR;

    USBD->EP[4].CFG = (USBD->EP[4].CFG & ~USBD_CFG_STATE_Msk) | (EP_STATE_OUT << USBD_CFG_STATE_Pos);
    USBD->EP[4].CFG = (USBD->EP[4].CFG & ~USBD_CFG_EP_NUM_Msk) | (3 << USBD_CFG_EP_NUM_Pos);
    USBD->EP[4].BUFSEG = USB_EP4_BUF_ADDR;
}

void Hal_Usb_Bus_Handler(uint32_t u32BusSts )
{
    if(u32BusSts  & USBD_ATTR_USBRST_Msk)
    {
        USBD->FADDR = 0x0;

        Hal_Usb_Ep_Config();

        usb_callback->usb_rst_callback();
    }

    if(u32BusSts  & USBD_ATTR_SUSPEND_Msk)
    {
        USBD->ATTR &= ~USBD_ATTR_PHY_EN_Msk;

        usb_callback->usb_suspend_callback();
    }

    if(u32BusSts & USBD_ATTR_RESUME_Msk)
    {
        USBD->ATTR |= USBD_ATTR_PHY_EN_Msk;
        USBD->ATTR |= USBD_ATTR_USB_EN_Msk;

        usb_callback->usb_resume_callback();
    }
}

void Hal_Usb_Isr_Handler(void )
{
    uint32_t u32BusSts;
    uint32_t u32IntSts;

    u32IntSts = USBD->INTSTS;

    u32BusSts = USBD->ATTR;

    if(u32IntSts & USBD_INTSTS_BUS_STS_Msk)
    {
        USBD->INTSTS |= USBD_INTSTS_BUS_STS_Msk;

        Hal_Usb_Bus_Handler(u32BusSts);
    }

    if(u32IntSts & USBD_INTSTS_FLDET_STS_Msk)
    {
        USBD->INTSTS |= USBD_INTSTS_FLDET_STS_Msk;

        usb_callback->usb_fldet_callabck();
    }

    if(u32IntSts & USBD_INTSTS_USB_STS_Msk)
    {
        if(u32IntSts & USBD_INTSTS_SETUP_Msk)
        {
            USBD->INTSTS |= USBD_INTSTS_SETUP_Msk;

            usb_callback->usb_setup_callback();
        }

        if(u32IntSts & USBD_INTSTS_EPEVT0_Msk)
        {
            USBD->INTSTS |= USBD_INTSTS_EPEVT0_Msk;

            usb_callback->usb_ep0_callback();
        }

        if(u32IntSts & USBD_INTSTS_EPEVT1_Msk)
        {
            USBD->INTSTS |= USBD_INTSTS_EPEVT1_Msk;

            usb_callback->usb_ep1_callback();
        }

        if(u32IntSts & USBD_INTSTS_EPEVT2_Msk)
        {
            USBD->INTSTS |= USBD_INTSTS_EPEVT2_Msk;

            usb_callback->usb_ep2_callback();
        }

        if(u32IntSts & USBD_INTSTS_EPEVT3_Msk)
        {
            USBD->INTSTS |= USBD_INTSTS_EPEVT3_Msk;

            usb_callback->usb_ep3_callbacK();
        }
    }
}

