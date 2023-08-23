#ifndef MYUSB_H
#define MYUSB_H

#include "hidapi-win/include/hidapi.h"

#include <QWidget>

#define VID                0x0416
#define PID                0x5020

class MyUsb : public QWidget
{
    Q_OBJECT
public:
    explicit MyUsb(QWidget *parent = nullptr);

    bool Usb_Find(void );
    bool Usb_Open(void );
    void Usb_Close(void );
    int16_t Usb_Write(uint8_t *buf, uint16_t length );
    int16_t Usb_Read(uint8_t *buf, uint16_t length );
private:
    hid_device *handle;

signals:

};

#endif // MYUSB_H
