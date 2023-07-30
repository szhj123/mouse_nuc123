#ifndef _HAL_USB_H
#define _HAL_UBS_H

#include "hal_nuc123.h"

#define EP_STATE_IN        0x02
#define EP_STATE_OUT       0x01

#define USB_SETUP_BUF_ADDR 0x00
#define USB_SETUP_BUF_LEN  0x08
#define USB_EP0_BUF_ADDR   (USB_SETUP_BUF_ADDR + USB_SETUP_BUF_LEN)
#define USB_EP0_BUF_LEN    0x40
#define USB_EP1_BUF_ADDR   (USB_EP0_BUF_ADDR + USB_EP0_BUF_LEN)
#define USB_EP1_BUF_LEN    0x40
#define USB_EP2_BUF_ADDR   (USB_EP1_BUF_ADDR + USB_EP1_BUF_LEN)
#define USB_EP2_BUF_LEN    0x08
#define USB_EP3_BUF_ADDR   (USB_EP2_BUF_ADDR + USB_EP2_BUF_LEN)
#define USB_EP3_BUF_LEN    0x08
#define USB_EP4_BUF_ADDR   (USB_EP3_BUF_ADDR + USB_EP3_BUF_LEN)
#define USB_EP4_BUF_LEN    0x40


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
    void (*usb_ep3_callback)(void );
    void (*usb_ep4_callback)(void );
}usb_isr_callback_t;


void Hal_Usb_Init(void );
void Hal_Usb_Regist_Isr_Hanlder(usb_isr_callback_t *pCallback );
void Hal_Usb_Isr_Handler(void );

void Hal_Usb_Set_Dev_Addr(uint8_t devAddr );
void Hal_Usb_Set_Dsq_Sync(uint8_t epNum, uint8_t dsqSyncFlag );
uint32_t Hal_Usb_Get_Setup_Buf_Addr(void );
uint32_t Hal_Usb_Get_Ep_Buf_Addr(uint8_t epNum );
void Hal_Usb_ClrRdy(uint8_t epNum );
void Hal_Usb_ClrStall(uint8_t epNum );
void Hal_Usb_InOut_Ready(uint8_t epNum, uint16_t length );
uint8_t Hal_Usb_Get_Ep_InOut_Size(uint8_t epNum );

#endif 

