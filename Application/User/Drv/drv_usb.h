#ifndef _DRV_USB_H
#define _DRV_USB_H

#include "hal_usb.h"

typedef enum _ep0_intp_state_t
{
    DATA_OUT = 0,
    DATA_IN,
    SET_ADDR
}ep0_intp_state_t;



typedef struct _usb_ctrl_block_t
{
    volatile uint8_t bmRequestType;
    volatile uint8_t bRequest;
    volatile uint8_t wValue_l;
    volatile uint8_t wValue_h;
    volatile uint8_t wIndex_l;
    volatile uint8_t wIndex_h;
    volatile uint8_t wLength_l;
    volatile uint8_t wLength_h;

    ep0_intp_state_t ep0IntpState;
             uint8_t devAddr;
             uint8_t configVal;
             uint8_t dsqSync;

             uint8_t *pData;
             uint16_t dataLen;
             
             uint8_t suspendFlag;
             uint8_t resumeFlag;
}usb_ctrl_block_t;

void Drv_Usb_Init(void );
void Drv_Usb_Rst_Handler(void );
void Drv_Usb_Suspend_Handler(void );
void Drv_Usb_Resume_Handler(void );
void Drv_Usb_Fldet_Handler(void );
void Drv_Usb_Setup_Handler(void );
void Drv_Usb_Ep0_Handler(void );
void Drv_Usb_Ep1_Handler(void );
void Drv_Usb_Ep2_Handler(void );
void Drv_Usb_Ep3_Handler(void );

void Drv_Usb_Req_Standard(void );
void Drv_Usb_Req_Class(void );
void Drv_Usb_Req_Vendor(void );

void Drv_Usb_Set_Address(void );
void Drv_Usb_Get_Descriptor(void );
void Drv_Usb_Set_Config(void );

void Drv_Usb_Data_InReady(uint8_t *epPtr, uint8_t *descPtr );

void Drv_Usb_Memcpy(uint8_t *pDst, uint8_t *pSrc, uint16_t length );



#endif 
