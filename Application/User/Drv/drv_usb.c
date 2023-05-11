/********************************************************
* @file       drv_usb.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_usb.h"
#include "drv_usb_descriptor.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Usb_Rst_Handler(void );
static void Drv_Usb_Suspend_Handler(void );
static void Drv_Usb_Resume_Handler(void );
static void Drv_Usb_Fldet_Handler(void );
static void Drv_Usb_Setup_Handler(void );
static void Drv_Usb_Ep0_Handler(void );
static void Drv_Usb_Ep1_Handler(void );
static void Drv_Usb_Ep2_Handler(void );
static void Drv_Usb_Ep3_Handler(void );

static void Drv_Usb_Req_Standard(void );
static void Drv_Usb_Req_Class(void );
static void Drv_Usb_Req_Vendor(void );

static void Drv_Usb_Set_Address(void );
static void Drv_Usb_Get_Descriptor(void );


/* Private variables ------------------------------------*/
usb_ctrl_block_t usbCtrl;
usb_isr_callback_t usbIsrCallback = 
{
    .usb_rst_callback = Drv_Usb_Rst_Handler,
    .usb_suspend_callback = Drv_Usb_Suspend_Handler,
    .usb_resume_callback = Drv_Usb_Resume_Handler,
    .usb_fldet_callabck = Drv_Usb_Fldet_Handler,
    .usb_setup_callback = Drv_Usb_Setup_Handler,
    .usb_ep0_callback = Drv_Usb_Ep0_Handler,
    .usb_ep1_callback = Drv_Usb_Ep1_Handler,
    .usb_ep2_callback = Drv_Usb_Ep2_Handler,
    .usb_ep3_callbacK = Drv_Usb_Ep3_Handler
};


void Drv_Usb_Init(void )
{
    Hal_Usb_Init();

    Hal_Usb_Regist_Isr_Hanlder(&usbIsrCallback);
}

static void Drv_Usb_Rst_Handler(void )
{
    usbCtrl.pData = NULL;
    usbCtrl.dataLen = 0;
    usbCtrl.configVal = 0;
    usbCtrl.dsqSync = 0;
    usbCtrl.suspendFlag = 0;
    usbCtrl.resumeFlag = 0;
}

static void Drv_Usb_Suspend_Handler(void )
{
    usbCtrl.suspendFlag = 1;
    usbCtrl.resumeFlag = 0;
}

static void Drv_Usb_Resume_Handler(void )
{
    usbCtrl.suspendFlag = 0;
    usbCtrl.resumeFlag = 1;
}

static void Drv_Usb_Fldet_Handler(void )
{
    
}

static void Drv_Usb_Setup_Handler(void )
{
    uint8_t *pSetupBuf = (uint8_t *)(USB_SRAM + USBD->STBUFSEG);

    USBD->EP[0].CFGP |= USBD_CFGP_CLRRDY_Msk;
    USBD->EP[1].CFGP |= USBD_CFGP_CLRRDY_Msk;

    usbCtrl.bmRequestType = pSetupBuf[0];
    usbCtrl.bRequest =      pSetupBuf[1];
    usbCtrl.wValue_l =      pSetupBuf[2];
    usbCtrl.wValue_h =      pSetupBuf[3];
    usbCtrl.wIndex_l =      pSetupBuf[4];
    usbCtrl.wIndex_h =      pSetupBuf[5];
    usbCtrl.wLength_l =     pSetupBuf[6];
    usbCtrl.wLength_h =     pSetupBuf[7];

    switch(usbCtrl.bmRequestType & 0x60)
    {
        case USB_REQ_STANDARD:
        {
            Drv_Usb_Req_Standard();
            break;
        }
        case USB_REQ_CLASS:
        {
            Drv_Usb_Req_Class();
            break;
        }
        case USB_REQ_VENDOR:
        {
            Drv_Usb_Req_Vendor();
            break;
        }
        default:
        {
            Hal_Usb_Clr_Stall();
            break;
        }
    }
}

static void Drv_Usb_Ep0_Handler(void )
{
    
}

static void Drv_Usb_Ep1_Handler(void )
{
    
}

static void Drv_Usb_Ep2_Handler(void )
{
    
}

static void Drv_Usb_Ep3_Handler(void )
{
    
}

static void Drv_Usb_Req_Standard(void )
{
    switch(usbCtrl.bRequest)
    {
        case USB_SET_ADDRESS:
        {
            Drv_Usb_Set_Address();
            break;
        }
        case USB_GET_DESCRIPTOR:
        {
            Drv_Usb_Get_Descriptor();
            break;
        }
        case USB_SET_CONFIGURATION:
        {
            break;
        }
    }
}

static void Drv_Usb_Req_Class(void )
{
    
}

static void Drv_Usb_Req_Vendor(void )
{
    
}

static void Drv_Usb_Set_Address(void )
{
    
}

static void Drv_Usb_Get_Descriptor(void )
{
    uint8_t *u8Ep0Ptr = (uint8_t *)(USB_SRAM + USBD->EP[0].BUFSEG);
        
    usbCtrl.dataLen = (uint16_t )usbCtrl.wLength_h << 8 | usbCtrl.wLength_l;

    switch(usbCtrl.wValue_h)
    {
        case USB_DESC_DEVICE:
        {
            usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, gu8DeviceDescriptor[0]);
            Drv_Usb_Mem_Copy_In(u8Ep0Ptr, gu8DeviceDescriptor, usbCtrl.dataLen);
            Hal_Usb_Set_In_Out_Rdy(EP0);
            break;
        }
        case USB_DESC_CONFIG:
        {
            break;
        }
        case USB_DESC_STRING:
        {
            break;
        }
        default:
        {
            Hal_Usb_Clr_Stall();
        }
    }
}

