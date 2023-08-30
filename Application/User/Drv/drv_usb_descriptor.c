/********************************************************
* @file       drv_usb_descriptor.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_usb_descriptor.h"
#include "hal_usb.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

/*!<USB HID Report Descriptor */
const uint8_t HID_MouseReportDescriptor[] =
{
    0x05, 0x01,			// Usage Page (Generic Desktop Control)
	0x09, 0x02,			// Usage (Mouse)
	0xA1, 0x01,			// Collection (Application)
	0x85, RPT_ID_MOUSE, // Report ID

    0x09, 0x01,			// Usage (Pointer)
	0xA1, 0x00,			// Collection (Physical)
	0x05, 0x09,			// Usage Page (Button)
	0x19, 0x01,			// Usage Minimum (1)
	0x29, 0x05,			// Usage Maximum (5)
	0x15, 0x00,			// Logical Minimum (0)
	0x25, 0x01,			// Logical Maximum (1)

	0x75, 0x01,			// Report Size (1)
	0x95, 0x05,			// Report Count (5)
	0x81, 0x02,			// Input (Data, Variable, Absolute)

    0x75, 0x03,         /* Report Size(0x3) */
    0x95, 0x01,         /* Report Count(0x1) */
    0x81, 0x01,         /* Input(3 bit padding) */

	0x05, 0x01,			// Usage Page (Generic Desktop Control)
	0x09, 0x30,			// Usage (X)
	0x09, 0x31,			// Usage (Y)
	0x16, 0x00, 0x80,	// Logical Minimum (-32768) LSB first
	0x26, 0xff, 0x7f,	// Logical Maximum (32767)
	0x75, 0x10,			// Report Size (16)	//4BYTE
	0x95, 0x02,			// Report Count (2)
	0x81, 0x06,			// Input (Data, Variable, Relative)
	0x05, 0x01,			// Usage Page (Generic Desktop Control)
	0x09, 0x38,			// Usage (Wheel)
	0x15, 0x80,			// Logical Minimum (-128)
	0x25, 0x7F,			// Logical Maximum (127)
	0x75, 0x08,			// Report Size (8)
	0x95, 0x01,			// Report Count (1)
	0x81, 0x06,			// Input (Data, Variable, Relative)
	0xC0,				// End Collection

    0x06, 0x08, 0xFF,   // usage page(�Զ���)
    0x09, 0x01,         // usage(1)
    0xA1, 0x01,         // Collection(Application)
    0x85, RPT_ID_UPG_FW_SIZE,// REPORT_ID (0x70)
    0x15, 0x00,         // Logical Minimum (0)
    0x26, 0xFF, 0x00,   // Logical Maximum (255)
    0x09, 0x00,
    0x75, 0x08,         // REPORT SIZE (8)
    0x95, 0x02,         // REPORT COUNT(2)
    0xB1, 0x02,         // Output(DATA,VARIABLE,ABSOLUTE)
    0xC0,

    0x06, 0x09, 0xFF,   // usage page(�Զ���)
    0x09, 0x01,         // usage(1)
    0xA1, 0x01,         // Collection(Application)
    0x85, RPT_ID_UPG_FW_DATA,// REPORT_ID (0x71)
    0x15, 0x00,         // Logical Minimum (0)
    0x26, 0xFF, 0x00,   // Logical Maximum (255)
    0x09, 0x00,
    0x75, 0x08,         // REPORT SIZE (8)
    0x95, 0x3F,         // REPORT COUNT(63)
    0xB1, 0x02,         // Output(DATA,VARIABLE,ABSOLUTE)
    0xC0,

    0x06, 0x0a, 0xFF,   // usage page(�Զ���)
    0x09, 0x01,         // usage(1)
    0xA1, 0x01,         // Collection(Application)
    0x85, RPT_ID_UPG_FW_CHECKSUM,// REPORT_ID (0x72)
    0x15, 0x00,         // Logical Minimum (0)
    0x26, 0xFF, 0x00,   // Logical Maximum (255)
    0x09, 0x00,
    0x75, 0x08,         // REPORT SIZE (8)
    0x95, 0x02,         // REPORT COUNT(2)
    0xB1, 0x02,         // Output(DATA,VARIABLE,ABSOLUTE)
    0xC0,

    0x06, 0x0b, 0xFF,   // usage page(�Զ���)
    0x09, 0x01,         // usage(1)
    0xA1, 0x01,         // Collection(Application)
    0x85, RPT_ID_UPG_FW_ACK,// REPORT_ID (0x73)
    0x15, 0x00,         // Logical Minimum (0)
    0x26, 0xFF, 0x00,   // Logical Maximum (255)
    0x09, 0x00,
    0x75, 0x08,         // REPORT SIZE (8)
    0x95, 0x01,         // REPORT COUNT(1)
    0xB1, 0x02,         // Input(DATA,VARIABLE,ABSOLUTE)
    0xC0,
    
	0xC0				// End Collection
};

const uint8_t HID_KeyboardReportDescriptor[] =
{
    0x05,0x01,          /* Usage Page(Generic Desktop Controls) */
    0x09,0x06,          /* Usage(Keyboard) */
    0xa1,0x01,          /* Collection(Application) */
    0x85,RPT_ID_KEYBOARD,  /* REPORT ID 0x02 */
    0x05,0x07,          /* Usage Page(Keyboard/Keypad) */
    0x19,0xe0,          /* Usage Minimum(0xE0) */
    0x29,0xe7,          /* Usage Maximum(0xE7) */
    0x15,0x00,          /* Logical Minimum(0x0) */
    0x25,0x01,          /* Logical Maximum(0x1) */
    0x95,0x08,          /* Report Count(0x8) */
    0x75,0x01,          /* Report Size(0x1) */
    0x81,0x02,          /* Input (Data) => Modifier byte */

    0x95,0x06,          /* Report Count(0x6) */
    0x75,0x08,          /* Report Size(0x8) */
    0x15,0x00,          /* Logical Minimum(0x0) */
    0x25,0xff,          /* Logical Maximum(0xff) */
    0x05,0x07,          /* Usage Page(Keyboard/Keypad) */
    0x19,0x00,          /* Usage Minimum(0x0) */
    0x29,0xff,          /* Usage Maximum(0xff) */
    0x81,0x00,          /* Input (Data) */
    0xc0,               /* End Collection */

    0x05,0x0c,          /* Usage page (Consumer) */
    0x09,0x01,          /* Consumer Control */
    0xa1,0x01,          /* Collection(Application) */
    0x85,RPT_ID_CONSUMER,  /* REPORT ID 0x03 */
    0x0A,0x21,0x02,     // USAGE WWW SEARCH
    0x0A,0x23,0x02,     // USAGE WWW HOME
    0x0A,0x24,0x02,     // USAGE WWW BACKWARD
    0x0A,0x25,0x02,     // USAGE WWW FORWARD
    0x0A,0x26,0x02,     // USAGE WWW STOP
    0x0A,0x27,0x02,     // USAGE WWW REFRESH
    0x0A,0x2A,0x02,     // USAGE MY FAVORITE
    0x0A,0xB1,0x02,     // TURBO
    0x15, 0x00,         // Logical Minimum (0)
    0x25, 0x01,         // Logical Maximum (1)
    0x95,0x08,          /* Report Count(0x8) */
    0x75,0x01,          /* Report Size(0x1) */
    0x81,0x03,          /* Input (Data) */

    0x0A,0xB5,0x00,		// USAGE NEXT_TRACK
	0x0A,0xB6,0x00,		// USAGE PRE_TRACK
	0x0A,0xB7,0x00,		// USAGE STOP
	0x0A,0xCD,0x00,		// USAGE PLAY/PAUSE
	0x0A,0xE2,0x00,		// USAGE MUTE
	0x0A,0xA2,0x00,		// USAGE FLIP 3D
	0x0A,0xE9,0x00,		// USAGE VOL_INC
	0x0A,0xEA,0x00,		// USAGE VOL_DEC
	0x15,0x00,			// Logical Minimum (0)
	0x25,0x01,			// Logical Maximum (1)
    0x95,0x08,          // Report Count(0x8)
    0x75,0x01,          // Report Size(0x1)
	0x81,0x03,			// INPUT()

    0x0A,0x83,0x01,		// USAGE MEDIA SELECT
	0x0A,0x94,0x01,		// USAGE My Computer
	0x0A,0x86,0x01,		// USAGE EXCEL
	0x0A,0x88,0x01,		// USAGE POWER POINT
	0x0A,0x8A,0x01,		// USAGE EMAIL
	0x0A,0x92,0x01,		// USAGE CALCULATOR
	0x0A,0xA8,0x02,		// USAGE PHOTO GALLERY
	0x0A,0x84,0x01,		// USAGE Word
	0x15,0x00,			// Logical Minimum (0)
	0x25,0x01,			// Logical Maximum (1)
    0x95,0x08,          // Report Count(0x8)
    0x75,0x01,          // Report Size(0x1)
	0x81,0x03,			// INPUT()
    0xc0,               /* End Collection */

    0x06, 0x02, 0xFF, 	// usage page(�Զ���)
	0x09, 0x01,			// usage(1)
	0xA1, 0x01,			// Collection(Application)
	0x85, RPT_ID_KEY_REUSE,// REPORT_ID (0x60)
	0x15, 0x00,			// Logical Minimum (0)
	0x26, 0xFF, 0x00,	// Logical Maximum (255)
	0x09, 0x00,
	0x75, 0x08,			// REPORT SIZE (8)
	0x95, 0x3f,			// REPORT COUNT(63)
	0xB1, 0x02,			// FEATURE(DATA,VARIABLE,ABSOLUTE)
	0xC0,

    0x06, 0x03, 0xFF, 	// usage page(�Զ���)
	0x09, 0x01,			// usage(1)
	0xA1, 0x01,			// Collection(Application)
	0x85, RPT_ID_LDR,      // REPORT_ID (0x61)
	0x15, 0x00,			// Logical Minimum (0)
	0x26, 0xFF, 0x00,	// Logical Maximum (255)
	0x09, 0x00,
	0x75, 0x08,			// REPORT SIZE (8)
	0x95, 0x3f,			// REPORT COUNT(63)
	0xB1, 0x02,			// FEATURE(DATA,VARIABLE,ABSOLUTE)
	0xC0,

    0x06, 0x04, 0xFF, 	// usage page(�Զ���)
	0x09, 0x01,			// usage(1)
	0xA1, 0x01,			// Collection(Application)
	0x85, RPT_ID_KEY_MODE, // REPORT_ID (0x62)
	0x15, 0x00,			// Logical Minimum (0)
	0x26, 0xFF, 0x00,	// Logical Maximum (255)
	0x09, 0x00,
	0x75, 0x08,			// REPORT SIZE (8)
	0x95, 0x3f,			// REPORT COUNT(63)
	0xB1, 0x02,			// FEATURE(DATA,VARIABLE,ABSOLUTE)
	0xC0,

    0x06, 0x05, 0xFF, 	// usage page(�Զ���)
	0x09, 0x01,			// usage(1)
	0xA1, 0x01,			// Collection(Application)
	0x85, RPT_ID_LGT_EFECT, // REPORT_ID (0x63)
	0x15, 0x00,			// Logical Minimum (0)
	0x26, 0xFF, 0x00,	// Logical Maximum (255)
	0x09, 0x00,
	0x75, 0x08,			// REPORT SIZE (8)
	0x95, 0x3f,			// REPORT COUNT(63)
	0xB1, 0x02,			// FEATURE(DATA,VARIABLE,ABSOLUTE)
	0xC0,

    0x06,0x06,0xff,     // usage page(�Զ���)
    0x09,0x01,          // usage(1)
    0xa1,0x01,          // Collection(Application)
    0x85,RPT_ID_MACRO,  // REPORT_ID (0x64)
    0x15,0x00,          // Logical Minimum (0)
    0x26,0xff,0x00,     // Logical Maximum (255)
    0x09,0x00,
    0x75,0x08,          // REPORT SIZE (8)
    0x95,0x3f,          // REPORT COUNT(63)
    0xb1,0x02,          // FEATURE(DATA,VARIABLE,ABSOLUTE)
    0xc0,

    0x06,0x01,0xff,     /* Usage page(vendor custom define) */
    0x09,0x01,          /* Consumer Control */
    0xa1,0x01,          /* Collection(Application) */
    0x85,RPT_ID_PIC,    /* REPORT ID 0x65 */
    0x15,0x00,          /* Logical Minimum(0x0) */
    0x26,0xff,0x00,     /* Logical Maximum(0xff) */
    0x09,0x20,
    0x95,0x3f,          /* Report Count(63) */
    0x75,0x08,          /* Report Size(0x08) */
    0x91,0x02,          /* Output (Data) */
    0xc0,

    0x06, 0x07, 0xFF, 	// usage page(�Զ���)
	0x09, 0x01,			// usage(1)
	0xA1, 0x01,			// Collection(Application)
	0x85, RPT_ID_DPI,	// REPORT_ID (0x66)
	0x15, 0x00,			// Logical Minimum (0)
	0x26, 0xFF, 0x00,	// Logical Maximum (255)
	0x09, 0x00,
	0x75, 0x08,			// REPORT SIZE (8)
	0x95, 0x02,			// REPORT COUNT(2)
	0x81, 0x02,			// Input (Data) => Modifier byte
	0xC0,
    
    
    
};

uint16_t HID_MOUSE_RPT_LEN = sizeof(HID_MouseReportDescriptor)/sizeof(uint8_t ) ;
uint16_t HID_KEY_RPT_LEN = sizeof(HID_KeyboardReportDescriptor)/sizeof(uint8_t ) ;



const uint8_t gu8DeviceDescriptor[] =
{
    LEN_DEVICE,         /* bLength */
    DESC_DEVICE,        /* bDescriptorType */
    0x10, 0x01,         /* bcdUSB */
    0x00,               /* bDeviceClass */
    0x00,               /* bDeviceSubClass */
    0x00,               /* bDeviceProtocol */
    USB_EP0_BUF_LEN,    /* bMaxPacketSize0 */
    USBD_VID & 0x00FF,  /* idVendor */
    (USBD_VID & 0xFF00) >> 8,
    USBD_PID & 0x00FF,  /* idProduct */
    (USBD_PID & 0xFF00) >> 8,
    0x00, 0x00,         /* bcdDevice */
    0x01,               /* iManufacture */
    0x02,               /* iProduct */
    0x03,               /* iSerialNumber */
    0x01                /* bNumConfigurations */
};

/*!<USB Configure Descriptor */
const uint8_t gu8ConfigDescriptor[] =
{
    LEN_CONFIG,         /* bLength */
    DESC_CONFIG,        /* bDescriptorType */
                        /* wTotalLength */
    USB_CFG_LEN & 0x00FF,
    (USB_CFG_LEN & 0xFF00) >> 8,
    0x02,               /* bNumInterfaces */
    0x01,               /* bConfigurationValue */
    0x00,               /* iConfiguration */
                        /* bmAttributes */
    0x80 | (USB_SELF_POWERED << 6) | (USB_REMOTE_WAKEUP << 5),
    USBD_MAX_POWER,     /* MaxPower */

    /* I/F descr: HID - Mouse */
    LEN_INTERFACE,      /* bLength */
    DESC_INTERFACE,     /* bDescriptorType */
    0x00,               /* bInterfaceNumber */
    0x00,               /* bAlternateSetting */
    0x01,               /* bNumEndpoints */
    0x03,               /* bInterfaceClass */
    0x00,               /* bInterfaceSubClass */
    HID_CLASS_MOUSE,    /* bInterfaceProtocol */
    0x00,               /* iInterface */

    /* HID Descriptor */
    LEN_HID,            /* Size of this descriptor in UINT8s. */
    DESC_HID,           /* HID descriptor type. */
    0x10, 0x01,         /* HID Class Spec. release number. */
    0x00,               /* H/W target country. */
    0x01,               /* Number of HID class descriptors to follow. */
    DESC_HID_RPT,       /* Descriptor type. */
    /* Total length of report descriptor. */
    sizeof(HID_MouseReportDescriptor) & 0x00FF,
    (sizeof(HID_MouseReportDescriptor) & 0xFF00) >> 8,

    /* EP Descriptor: interrupt in. */
    LEN_ENDPOINT,       /* bLength */
    DESC_ENDPOINT,      /* bDescriptorType */
                        /* bEndpointAddress */
    (HID_MOUSE_EP_NUM | EP_INPUT), 
    EP_INT,             /* bmAttributes */
                        /* wMaxPacketSize */
    (USB_EP2_BUF_LEN & 0x00FF),
    (USB_EP2_BUF_LEN & 0xFF00) >> 8,
    HID_DEFAULT_INT_IN_INTERVAL,     /* bInterval */

    /* I/F descr: HID - Keyboard */
    LEN_INTERFACE,      /* bLength */
    DESC_INTERFACE,     /* bDescriptorType */
    0x01,               /* bInterfaceNumber */
    0x00,               /* bAlternateSetting */
    0x02,               /* bNumEndpoints */
    0x03,               /* bInterfaceClass */
    0x00,               /* bInterfaceSubClass */
    HID_CLASS_KEYBOARD, /* bInterfaceProtocol */
    0x00,               /* iInterface */

    /* HID Descriptor */
    LEN_HID,            /* Size of this descriptor in UINT8s. */
    DESC_HID,           /* HID descriptor type. */
    0x10, 0x01,         /* HID Class Spec. release number. */
    0x00,               /* H/W target country. */
    0x01,               /* Number of HID class descriptors to follow. */
    DESC_HID_RPT,       /* Descriptor type. */
    /* Total length of report descriptor. */
    sizeof(HID_KeyboardReportDescriptor) & 0x00FF,
    (sizeof(HID_KeyboardReportDescriptor) & 0xFF00) >> 8,

    /* EP Descriptor: interrupt in. */
    LEN_ENDPOINT,       /* bLength */
    DESC_ENDPOINT,      /* bDescriptorType */
                        /* bEndpointAddress */
    (HID_KB_EP_NUM | EP_INPUT), 
    EP_INT,             /* bmAttributes */
                        /* wMaxPacketSize */
    USB_EP3_BUF_LEN & 0x00FF,
    (USB_EP3_BUF_LEN & 0xFF00) >> 8,
                        /* bInterval */
    HID_DEFAULT_INT_IN_INTERVAL,     

    /* EP Descriptor: interrupt in. */
    LEN_ENDPOINT,       /* bLength */
    DESC_ENDPOINT,     /* bDescriptorType */
    (HID_PIC_EP_NUM | EP_OUTPUT), /* bEndpointAddress */
    EP_INT,             /* bmAttributes */
                        /* wMaxPacketSize */
    USB_EP4_BUF_LEN & 0x00FF,
    (USB_EP4_BUF_LEN & 0xFF00) >> 8,
                         /* bInterval */
    HID_DEFAULT_INT_IN_INTERVAL    
};

/*!<USB Language String Descriptor */
const uint8_t gu8StringLang[] =
{
    4,              /* bLength */
    0x03,    /* bDescriptorType */
    0x09, 0x04
};

/*!<USB Vendor String Descriptor */
const uint8_t gu8VendorStringDesc[] =
{
    16,
    0x03,
    'T', 0, 'i', 0, 'f', 0, 'f', 0, 'a', 0, 'n', 0, 'y', 0
};

/*!<USB Product String Descriptor */
const uint8_t gu8ProductStringDesc[] =
{
    30,
    0x03,
    'J', 0, '5', 0, '0', 0, '0', 0, ' ', 0, 'U', 0, 'S', 0, 'B', 0, ' ', 0, 'M', 0, 'o', 0, 'u', 0, 's', 0, 'e', 0
};

const uint8_t gu8StringSerial[] =
{
    26,             // bLength
    0x03,    // bDescriptorType
    'A', 0, '0', 0, '2', 0, '0', 0, '2', 0, '3', 0, '0', 0, '6', 0, '2', 0, '4', 0, '0', 0, '1', 0
};


