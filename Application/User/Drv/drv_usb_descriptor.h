#ifndef _DRV_USB_DESCRIPTOR_H
#define _DRV_USB_DESCRIPTOR_H

/* Define the vendor id and product id */
#define USBD_VID                              0x0416
#define USBD_PID                              0x5020

#define RPT_ID_MOUSE                          0x01
#define RPT_ID_KEYBOARD                       0x02
#define RPT_ID_CONSUMER                       0x03
#define RPT_ID_KEY_REUSE                      0x60
#define RPT_ID_LDR                            0x61
#define RPT_ID_KEY_MODE                       0x62
#define RPT_ID_LGT_EFECT                      0x63
#define RPT_ID_MACRO                          0x64
#define RPT_ID_PIC                            0x65
#define RPT_ID_DPI                            0x66


#define USB_CFG_LEN

#define USB_EP_INPUT                          0x80
#define USB_EP_OUTPUT                         0x00

/*!<USB Endpoint Type */
#define USB_EP_ISO                            0x01
#define USB_EP_BULK                           0x02
#define USB_EP_INT                            0x03

/* Define the EP number */
#define HID_MOUSE_EP_NUM                      0x01
#define HID_KB_EP_NUM                         0x02
#define HID_PIC_EP_NUM                        0x03



/* Define Descriptor information */
#define HID_DEFAULT_INT_IN_INTERVAL           1
#define USB_SELF_POWERED                      0
#define USB_REMOTE_WAKEUP                     1
#define USBD_MAX_POWER                        50  /* The unit is in 2mA. ex: 50 * 2mA = 100mA */

/*!<USB HID Interface Class protocol */
#define HID_CLASS_NONE                        0x00
#define HID_CLASS_KEYBOARD                    0x01
#define HID_CLASS_MOUSE                       0x02


extern const uint8_t HID_MouseReportDescriptor[];
extern const uint8_t HID_KeyboardReportDescriptor[];
extern const uint8_t gu8DeviceDescriptor[];
extern const uint8_t gu8ConfigDescriptor[];
extern const uint8_t gu8StringLang[];
extern const uint8_t gu8VendorStringDesc[];
extern const uint8_t gu8ProductStringDesc[];
extern const uint8_t gu8StringSerial[];



#endif 

