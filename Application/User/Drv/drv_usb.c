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

void Drv_Usb_Rst_Handler(void )
{
    usbCtrl.pData = NULL;
    usbCtrl.dataLen = 0;
    usbCtrl.configVal = 0;
    usbCtrl.dsqSync = 0;
    usbCtrl.suspendFlag = 0;
    usbCtrl.resumeFlag = 0;
}

void Drv_Usb_Suspend_Handler(void )
{
    usbCtrl.suspendFlag = 1;
    usbCtrl.resumeFlag = 0;
}

void Drv_Usb_Resume_Handler(void )
{
    usbCtrl.suspendFlag = 0;
    usbCtrl.resumeFlag = 1;
}

void Drv_Usb_Fldet_Handler(void )
{
    
}

void Drv_Usb_Setup_Handler(void )
{
    uint8_t *stpBuf = (uint8_t *)Hal_Usb_Get_Setup_Buf_Addr();

    Hal_Usb_ClrRdy(EP0);

    Hal_Usb_ClrRdy(EP1);

    usbCtrl.bmRequestType = stpBuf[0];
    usbCtrl.bRequest      = stpBuf[1];
    usbCtrl.wValue_l      = stpBuf[2];
    usbCtrl.wValue_h      = stpBuf[3];
    usbCtrl.wIndex_l      = stpBuf[4];
    usbCtrl.wIndex_h      = stpBuf[5];
    usbCtrl.wLength_l     = stpBuf[6];
    usbCtrl.wLength_h     = stpBuf[7];

    switch(usbCtrl.bmRequestType & 0x60)
    {
        case REQ_STANDARD:
        {
            Drv_Usb_Req_Standard();
            break;
        }
        case REQ_CLASS:
        {
            Drv_Usb_Req_Class();
            break;
        }
        case REQ_VENDOR:
        {
            Drv_Usb_Req_Vendor();
            break;
        }
        default:
        {
            Hal_Usb_ClrStall(EP0);
            Hal_Usb_ClrStall(EP1);
            break;
        }
    }
}

void Drv_Usb_Ep0_Handler(void )
{
    uint8_t *u8Ep0Ptr = (uint8_t *)Hal_Usb_Get_Ep_Buf_Addr(EP0);

    switch(usbCtrl.ep0IntpState)
    {
        case DATA_OUT:
        {
            usbCtrl.dsqSync = 0;
            usbCtrl.dataLen = 0;
            usbCtrl.pData = NULL;

            Hal_Usb_InOut_Ready(EP1, 0);
            
            break;
        }
        case DATA_IN:
        {
            usbCtrl.dsqSync ^= 1;
            
            Hal_Usb_Set_Dsq_Sync(EP0, usbCtrl.dsqSync);

            Drv_Usb_Data_InReady(u8Ep0Ptr, usbCtrl.pData);
            
            break;
        }
        case SET_ADDR:
        {
            Hal_Usb_Set_Dev_Addr(usbCtrl.devAddr);
            
            break;
        }
        default: 
        {
            Hal_Usb_ClrStall(EP0);
            Hal_Usb_ClrStall(EP1);
            break;
        }
    }
}

void Drv_Usb_Ep1_Handler(void )
{
    
}

void Drv_Usb_Ep2_Handler(void )
{
    
}

void Drv_Usb_Ep3_Handler(void )
{
    
}

void Drv_Usb_Req_Standard(void )
{
    switch(usbCtrl.bRequest)
    {
        case SET_ADDRESS:
        {
            Drv_Usb_Set_Address();
            break;
        }
        case GET_DESCRIPTOR:
        {
            Drv_Usb_Get_Descriptor();
            break;
        }
        case SET_CONFIGURATION:
        {
            Drv_Usb_Set_Config();
            break;
        }
        default: 
        {
            Hal_Usb_ClrStall(EP0);
            Hal_Usb_ClrStall(EP1);
            break;
        }
    }
}

void Drv_Usb_Req_Class(void )
{
    switch(usbCtrl.bRequest)
    {
        case SET_IDLE:
        {
            Hal_Usb_Set_Dsq_Sync(EP0, 1);

            Hal_Usb_InOut_Ready(EP0, 0);
            
            break;
        }
        case SET_REPORT:
        {
            
        }
        default: 
        {
            Hal_Usb_ClrStall(EP0);
            Hal_Usb_ClrStall(EP1);
            break;
        }
    }
}

void Drv_Usb_Req_Vendor(void )
{
    
}




void Drv_Usb_Set_Address(void )
{
    usbCtrl.devAddr = usbCtrl.wValue_l;

    Hal_Usb_Set_Dsq_Sync(EP0, 1);

    Hal_Usb_InOut_Ready(EP0, 0);

    usbCtrl.ep0IntpState = SET_ADDR;
}

void Drv_Usb_Get_Descriptor(void )
{
    uint8_t *u8Ep0Ptr = (uint8_t *)Hal_Usb_Get_Ep_Buf_Addr(EP0);
        
    usbCtrl.dataLen = (uint16_t )usbCtrl.wLength_h << 8 | usbCtrl.wLength_l;

    usbCtrl.dsqSync = 1;

    Hal_Usb_Set_Dsq_Sync(EP0, usbCtrl.dsqSync);

    switch(usbCtrl.wValue_h)
    {
        case DESC_DEVICE:
        {
            usbCtrl.pData = (uint8_t *)&gu8DeviceDescriptor[0];
            
            usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, gu8DeviceDescriptor[0]);
            
            Drv_Usb_Data_InReady(u8Ep0Ptr, usbCtrl.pData);

            Hal_Usb_InOut_Ready(EP1, 0);
            
            break;
        }
        case DESC_CONFIG:
        {
            usbCtrl.pData = (uint8_t *)&gu8ConfigDescriptor[0];
            
            usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, (uint16_t )gu8ConfigDescriptor[3]<<8|gu8ConfigDescriptor[2]);

            Drv_Usb_Data_InReady(u8Ep0Ptr, usbCtrl.pData);
            
            break;
        }
        case DESC_STRING:
        {
            if(usbCtrl.wValue_l == 0x00)
            {
                usbCtrl.pData = (uint8_t *)&gu8StringLang[0];
                           
                usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, gu8StringLang[0]);
            }
            else if(usbCtrl.wValue_l == 0x01)
            {
                usbCtrl.pData = (uint8_t *)&gu8VendorStringDesc[0];
                           
                usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, gu8VendorStringDesc[0]);
            }
            else if(usbCtrl.wValue_l == 0x02)
            {
                usbCtrl.pData = (uint8_t *)&gu8ProductStringDesc[0];
                           
                usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, gu8ProductStringDesc[0]);
            }
            else 
            {
                usbCtrl.pData = (uint8_t *)&gu8StringSerial[0];
                           
                usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, gu8StringSerial[0]);
            }

            Drv_Usb_Data_InReady(u8Ep0Ptr, usbCtrl.pData);
            break;
        }
        case DESC_HID_RPT:
        {
            if(usbCtrl.wIndex_l == 0x00)
            {
                usbCtrl.pData = (uint8_t *)&HID_MouseReportDescriptor[0];

                usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, HID_MOUSE_RPT_LEN);
            }
            else if(usbCtrl.wIndex_l == 0x01)
            {
                usbCtrl.pData = (uint8_t *)&HID_KeyboardReportDescriptor[0];

                usbCtrl.dataLen = Minimum(usbCtrl.dataLen, HID_KEY_RPT_LEN);
            }

            Drv_Usb_Data_InReady(u8Ep0Ptr, usbCtrl.pData);
            break;
        }
        default:
        {
            Hal_Usb_ClrStall(EP0);
            Hal_Usb_ClrStall(EP1);
        }
    }
}

void Drv_Usb_Set_Config(void )
{
    usbCtrl.configVal = usbCtrl.wValue_l;

    Hal_Usb_Set_Dsq_Sync(EP0, 1);

    Hal_Usb_InOut_Ready(EP0, 0);
}

void Drv_Usb_Data_InReady(uint8_t *epPtr, uint8_t *descPtr )
{
    if(usbCtrl.dataLen > USB_EP0_BUF_LEN)
    {
        Drv_Usb_Memcpy(epPtr, descPtr, USB_EP0_BUF_LEN);

        usbCtrl.dataLen -= USB_EP0_BUF_LEN;

        usbCtrl.pData += USB_EP0_BUF_LEN;
        
        Hal_Usb_InOut_Ready(EP0, USB_EP0_BUF_LEN);

        usbCtrl.ep0IntpState = DATA_IN;
    }
    else
    {
        Drv_Usb_Memcpy(epPtr, descPtr, usbCtrl.dataLen);
        
        Hal_Usb_InOut_Ready(EP0, usbCtrl.dataLen);
        
        usbCtrl.ep0IntpState = DATA_OUT;
    }
}

void Drv_Usb_Memcpy(uint8_t *pDst, uint8_t *pSrc, uint16_t length )
{

    while(length--)
    {
        *pDst++ = *pSrc++;
    }
}

