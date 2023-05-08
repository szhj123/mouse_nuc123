#ifndef _HAL_USB_H
#define _HAL_UBS_H

#include "hal_nuc123.h"

#define USB_BA             0x40060000
#define USB_SRAM           (USB_BA+0x100)

#define EP_STATE_IN        0x02
#define EP_STATE_OUT       0x01

#define USB_EP0            0
#define USB_EP1            1
#define USB_EP2            2
#define USB_EP3            3
#define USB_EP4            4

#define USB_SETUP_BUF_ADDR 0
#define USB_SETUP_BUF_LEN  8
#define USB_EP0_BUF_ADDR   (USB_SETUP_BUF_ADDR + USB_SETUP_BUF_LEN)
#define USB_EP0_BUF_LEN    64
#define USB_EP1_BUF_ADDR   (USB_EP0_BUF_ADDR + USB_EP0_BUF_LEN)
#define USB_EP1_BUF_LEN    64
#define USB_EP2_BUF_ADDR   (USB_EP1_BUF_ADDR + USB_EP1_BUF_LEN)
#define USB_EP2_BUF_LEN    8
#define USB_EP3_BUF_ADDR   (USB_EP2_BUF_ADDR + USB_EP2_BUF_LEN)
#define USB_EP3_BUF_LEN    8
#define USB_EP4_BUF_ADDR   (USB_EP3_BUF_ADDR + USB_EP3_BUF_LEN)
#define USB_EP4_BUF_LEN    64


typedef struct _usb_isr_callback_t
{
    void (*usb_rst_callback)(void );
    void (*usb_suspend_callback)(void );
    void (*usb_resume_callback)(void );
    void (*usb_fldet_callabck)(void );
    void (*usb_setup_callback)(void );
    void (*usb_ep0_callback)(void );
    void (*usb_ep1_callback)(void );
    void (*usb_ep2_callback)(void );
    void (*usb_ep3_callbacK)(void );
}usb_isr_callback_t;

void Hal_Usb_Init(void );
void Hal_Usb_Clr_Stall(void );
void Hal_Usb_Regist_Isr_Hanlder(usb_isr_callback_t *pCallback );
void Hal_Usb_Isr_Handler(void );

#endif 

