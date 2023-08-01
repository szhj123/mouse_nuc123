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
static usb_ctrl_block_t usbCtrl;

static usb_report_callback_t *appUsbCallback;

static usb_isr_callback_t usbIsrCallback = 
{
    .usb_rst_callback = Drv_Usb_Rst_Handler,
    .usb_suspend_callback = Drv_Usb_Suspend_Handler,
    .usb_resume_callback = Drv_Usb_Resume_Handler,
    .usb_fldet_callabck = Drv_Usb_Fldet_Handler,
    .usb_setup_callback = Drv_Usb_Setup_Handler,
    .usb_ep0_callback = Drv_Usb_Ep0_Handler,
    .usb_ep1_callback = Drv_Usb_Ep1_Handler,
    .usb_ep2_callback = Drv_Usb_Ep2_Handler,
    .usb_ep3_callback = Drv_Usb_Ep3_Handler,
    .usb_ep4_callback = Drv_Usb_Ep4_Handler,
    .usb_wakeup_callabck = Drv_Usb_Wakeup_Handler,
};

static uint8_t usbEpBuf[USB_EP1_BUF_LEN] = {0};

void Drv_Usb_Init(usb_report_callback_t *callback )
{
    appUsbCallback = callback;
    
    Hal_Usb_Init();

    Hal_Usb_Regist_Isr_Hanlder(&usbIsrCallback);
}

void Drv_Usb_Rst_Handler(void )
{
    usbCtrl.dataPtr = NULL;
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

uint8_t Drv_Usb_Get_Suspend_Flag(void )
{
    return usbCtrl.suspendFlag;
}

uint8_t Drv_Usb_Get_Config_Val(void )
{
    return usbCtrl.configVal;
}

uint8_t Drv_Usb_Get_Resume_Flag(void )
{
    return usbCtrl.resumeFlag;
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
            usbCtrl.dataPtr = NULL;

            Hal_Usb_InOut_Ready(EP1, 0);
            
            break;
        }
        case DATA_IN:
        {
            usbCtrl.dsqSync ^= 1;
            
            Hal_Usb_Set_Dsq_Sync(EP0, usbCtrl.dsqSync);

            Drv_Usb_Data_InReady(u8Ep0Ptr, usbCtrl.dataPtr);
            
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
    uint8_t *u8Ep1Ptr = (uint8_t *)Hal_Usb_Get_Ep_Buf_Addr(EP1);

    uint8_t ep1OutSize = Hal_Usb_Get_Ep_InOut_Size(EP1);

    if(usbCtrl.dataLen > 0)
    {
        Drv_Usb_Memcpy(usbCtrl.dataPtr, u8Ep1Ptr, ep1OutSize);

        usbCtrl.dataPtr += ep1OutSize;

        usbCtrl.dataLen -= ep1OutSize;

        if(usbCtrl.dataLen == 0)
        {
            appUsbCallback->usb_set_report_callback(usbEpBuf, sizeof(usbEpBuf));
            
            Hal_Usb_Set_Dsq_Sync(EP0, 1);

            Hal_Usb_InOut_Ready(EP0, 0);
        }
        else
        {
            Hal_Usb_InOut_Ready(EP1, USB_EP1_BUF_LEN);
        }
    }    
}

void Drv_Usb_Ep2_Handler(void )
{
    usbCtrl.epInDoneFlag = 1;
}

void Drv_Usb_Ep3_Handler(void )
{
    usbCtrl.epInDoneFlag = 1;
}

void Drv_Usb_Ep4_Handler(void )
{
    uint8_t *u8Ep4Ptr = (uint8_t *)Hal_Usb_Get_Ep_Buf_Addr(EP4);
    
    uint8_t ep4OutSize = Hal_Usb_Get_Ep_InOut_Size(EP4);

    if(ep4OutSize > USB_EP4_BUF_LEN)
    {
        ep4OutSize = USB_EP4_BUF_LEN;
    }

    appUsbCallback->usb_set_report_callback(u8Ep4Ptr, ep4OutSize);       
}

void Drv_Usb_Wakeup_Handler(void )
{
    if(usbCtrl.suspendFlag)
    {
        Hal_Usb_Wakeup();
    }
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
        case GET_REPORT:
        {
            uint8_t i;
            uint8_t *u8Ep0Ptr = (uint8_t *)Hal_Usb_Get_Ep_Buf_Addr(EP0);
            
            usbCtrl.dataLen = (uint16_t )usbCtrl.wLength_h << 8 | usbCtrl.wLength_l;

            usbCtrl.dataLen = Minimum(usbCtrl.dataLen, USB_EP0_BUF_LEN);

            usbCtrl.dataPtr = (uint8_t *)&usbEpBuf[0];

            for(i=0;i<sizeof(usbEpBuf);i++)
            {
                usbEpBuf[i] = 0x0;
            }

            usbEpBuf[0] = usbCtrl.wValue_l;

            appUsbCallback->usb_get_report_callback(usbCtrl.dataPtr, usbCtrl.dataLen);

            usbCtrl.dsqSync = 1;

            Hal_Usb_Set_Dsq_Sync(EP0, usbCtrl.dsqSync);

            Drv_Usb_Data_InReady(u8Ep0Ptr, usbCtrl.dataPtr);
            
            break;
        }
        case SET_REPORT:
        {
            /* Request Type = Feature */
            if(usbCtrl.wValue_h == 0x03) 
            {
                usbCtrl.dataLen = (uint16_t )usbCtrl.wLength_h << 8 | usbCtrl.wLength_l;

                usbCtrl.dataLen = Minimum(usbCtrl.dataLen, USB_EP1_BUF_LEN);

                usbCtrl.dataPtr = (uint8_t *)&usbEpBuf[0];
                
                Hal_Usb_Set_Dsq_Sync(EP1, 1);

                Hal_Usb_InOut_Ready(EP1, USB_EP1_BUF_LEN);
            }
            /* Request Type = Output */
            else if(usbCtrl.wValue_h == 0x02)
            {
                Hal_Usb_Set_Dsq_Sync(EP1, 1);

                Hal_Usb_InOut_Ready(EP1, USB_EP1_BUF_LEN);

                Hal_Usb_Set_Dsq_Sync(EP0, 1);

                Hal_Usb_InOut_Ready(EP0, 0);
            }

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
            usbCtrl.dataPtr = (uint8_t *)&gu8DeviceDescriptor[0];
            
            usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, gu8DeviceDescriptor[0]);
            
            Drv_Usb_Data_InReady(u8Ep0Ptr, usbCtrl.dataPtr);

            Hal_Usb_InOut_Ready(EP1, 0);
            
            break;
        }
        case DESC_CONFIG:
        {
            usbCtrl.dataPtr = (uint8_t *)&gu8ConfigDescriptor[0];
            
            usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, (uint16_t )gu8ConfigDescriptor[3]<<8|gu8ConfigDescriptor[2]);

            Drv_Usb_Data_InReady(u8Ep0Ptr, usbCtrl.dataPtr);
            
            break;
        }
        case DESC_STRING:
        {
            if(usbCtrl.wValue_l == 0x00)
            {
                usbCtrl.dataPtr = (uint8_t *)&gu8StringLang[0];
                           
                usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, gu8StringLang[0]);
            }
            else if(usbCtrl.wValue_l == 0x01)
            {
                usbCtrl.dataPtr = (uint8_t *)&gu8VendorStringDesc[0];
                           
                usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, gu8VendorStringDesc[0]);
            }
            else if(usbCtrl.wValue_l == 0x02)
            {
                usbCtrl.dataPtr = (uint8_t *)&gu8ProductStringDesc[0];
                           
                usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, gu8ProductStringDesc[0]);
            }
            else 
            {
                usbCtrl.dataPtr = (uint8_t *)&gu8StringSerial[0];
                           
                usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, gu8StringSerial[0]);
            }

            Drv_Usb_Data_InReady(u8Ep0Ptr, usbCtrl.dataPtr);
            break;
        }
        case DESC_HID_RPT:
        {
            if(usbCtrl.wIndex_l == 0x00)
            {
                usbCtrl.dataPtr = (uint8_t *)&HID_MouseReportDescriptor[0];

                usbCtrl.dataLen  = Minimum(usbCtrl.dataLen, HID_MOUSE_RPT_LEN);
            }
            else if(usbCtrl.wIndex_l == 0x01)
            {
                usbCtrl.dataPtr = (uint8_t *)&HID_KeyboardReportDescriptor[0];

                usbCtrl.dataLen = Minimum(usbCtrl.dataLen, HID_KEY_RPT_LEN);
            }

            Drv_Usb_Data_InReady(u8Ep0Ptr, usbCtrl.dataPtr);
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

    Hal_Usb_InOut_Ready(EP4, USB_EP4_BUF_LEN);
}

void Drv_Usb_Clr_Ep_In_Flag(uint8_t ep )
{
    if(ep == EP2 || ep == EP3)
    {
        usbCtrl.epInDoneFlag = 0;
    }
}

uint8_t Drv_Usb_Get_Ep_In_Flag(uint8_t ep )
{
    uint8_t retFlag = 0;
	
    if(ep == EP2 || ep == EP3)
    {
        retFlag = usbCtrl.epInDoneFlag;
    }
		
    return retFlag;
}

void Drv_Usb_Ep4_Out_Enable(void )
{    
    Hal_Usb_InOut_Ready(EP4, USB_EP4_BUF_LEN);
}

void Drv_Usb_Ep_In(uint8_t ep, uint8_t *buf, uint8_t len )
{
    uint8_t *u8EpPtr = (uint8_t *)Hal_Usb_Get_Ep_Buf_Addr(ep);

    Drv_Usb_Memcpy(u8EpPtr, buf, len);

    Hal_Usb_InOut_Ready(ep, len);
}

void Drv_Usb_Data_InReady(uint8_t *epPtr, uint8_t *descPtr )
{
    if(usbCtrl.dataLen > USB_EP0_BUF_LEN)
    {
        Drv_Usb_Memcpy(epPtr, descPtr, USB_EP0_BUF_LEN);

        usbCtrl.dataLen -= USB_EP0_BUF_LEN;

        usbCtrl.dataPtr += USB_EP0_BUF_LEN;
        
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

