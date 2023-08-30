#include "myusb.h"

MyUsb::MyUsb(QWidget *parent) : QWidget(parent)
{
    hid_init();
}

bool MyUsb::Usb_Find(void )
{
    bool retVal = false;

    hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(0x0, 0x0);

    cur_dev = devs;
    while(cur_dev)
    {
        qDebug("device found\n, vid:0x%x, pid:0x%x\n", cur_dev->vendor_id, cur_dev->product_id);

        if(cur_dev->vendor_id == VID && cur_dev->product_id == PID)
        {
            retVal = true;
            break;
        }

        cur_dev = cur_dev->next;
    }

    hid_free_enumeration(devs);

    return retVal;
}

bool MyUsb::Usb_Open()
{
    bool retVal = false;

    handle = hid_open(VID, PID, NULL);
    if(!handle)
    {
        retVal = false;
        qDebug("unable to open device");
    }
    else
    {
        retVal = true;

        hid_set_nonblocking(handle, 1);
    }

    return retVal;
}

void MyUsb::Usb_Close(void )
{
    hid_close(handle);

    hid_exit();
}

int16_t MyUsb::Usb_Write(uint8_t *buf, uint16_t length)
{
    int16_t retVal = 0;

    retVal = hid_send_feature_report(handle, buf, length);

    return retVal;
}

int16_t MyUsb::Usb_Read(uint8_t *buf, uint16_t length)
{
    int16_t retVal = 0;

    retVal = hid_get_feature_report(handle, buf, length);

    return retVal;
}


