#ifndef _DRV_USB_H
#define _DRV_USB_H

#include "hal_usb.h"

#define USB_REQ_STANDARD        0x00
#define USB_REQ_CLASS           0x20
#define USB_REQ_VENDOR          0x40

#define USB_GET_STATUS          0x00
#define USB_CLEAR_FEATURE       0x01
#define USB_SET_FEATURE         0x03
#define USB_SET_ADDRESS         0x05
#define USB_GET_DESCRIPTOR      0x06
#define USB_SET_DESCRIPTOR      0x07
#define USB_GET_CONFIGURATION   0x08
#define USB_SET_CONFIGURATION   0x09
#define USB_GET_INTERFACE       0x0a
#define USB_SET_INTERFACE       0x0b
#define USB_SYNCH_FRAME         0x0c


#define USB_DESC_DEVICE         0x01
#define USB_DESC_CONFIG         0x02
#define USB_DESC_STRING         0x03
#define USB_DESC_INTERFACE      0x04
#define USB_DESC_ENDPOINT       0x05


/*!<USB HID Descriptor Type */
#define USB_DESC_HID            0x21
#define USB_DESC_HID_RPT        0x22



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

             uint8_t configVal;
             uint8_t dsqSync;

             uint8_t *pData;
             uint16_t dataLen;
             
             uint8_t suspendFlag;
             uint8_t resumeFlag;
}usb_ctrl_block_t;

void Drv_Usb_Init(void );


#endif 

