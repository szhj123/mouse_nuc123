#include "myupgrade.h"

MyUpgrade::MyUpgrade(QWidget *parent, Ui::MainWindow *ui, MyUsb *myUsb) : QWidget(parent)
{
    this->ui = ui;

    this->myUsb = myUsb;

    timer = new QTimer(this);

    timer->setInterval(10);

    ui->progressBarFwUpg->setValue(0);

    connect(ui->btnFwFile, SIGNAL(clicked()), this, SLOT(on_btnFwFile_Clicked()));

    connect(ui->btnFwUpg, SIGNAL(clicked()), this, SLOT(on_btnFwUpg_Clicked()));

    connect(timer, SIGNAL(timeout()), this, SLOT(Upg_Handle()));

}

void InvertUint16(uint16_t *poly )
{
    uint8_t i;
    uint16_t tmp = 0;
    uint16_t polyVal = *poly;

    for(i=0;i<16;i++)
    {
        if(polyVal & (1 << i))
            tmp |= 1 << (15-i);
    }
    *poly = tmp;
}

uint16_t MyUpgrade::Cal_Fw_Checksum(uint8_t *buf, uint16_t length)
{
    uint16_t wCRCin = 0x0000;
    uint16_t wCPoly = 0x8005;
    uint8_t i;

    InvertUint16(&wCPoly);

    while(length--)
    {
        wCRCin ^= *(buf++);
        for(i=0;i<8;i++)
        {

            if(wCRCin & 0x0001)
                wCRCin = (wCRCin >> 1) ^ wCPoly;
            else
                wCRCin >>= 1;
        }
    }

    return wCRCin;
}

void MyUpgrade::on_btnFwFile_Clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("Open Upgrade Firmware"),QDir::homePath(),tr("(*.bin)"));

    ui->lineEditFwPath->setText(filePath);

    ui->lineEditFwPath->setReadOnly(true);

}

void MyUpgrade::on_btnFwUpg_Clicked()
{
    QFile readFile(ui->lineEditFwPath->text());

    if(!readFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("文件读取"), tr("打开文件失败：\n%1").arg(ui->lineEditFwPath->text()));

        return ;
    }

    QDataStream binFileData(&readFile);

    fwInfo.fwSize = readFile.size();

    fwInfo.fwSendBytes = fwInfo.fwSize;

    fwInfo.fwOffset = 0;

    static uint8_t *fwBuf = nullptr;

    if(fwBuf != nullptr)
    {
        delete [] fwBuf;
    }
    fwBuf = new uint8_t[fwInfo.fwSize];

    binFileData.readRawData((char *)fwBuf, static_cast<int>(fwInfo.fwSize));

    fwInfo.fwDataPtr = fwBuf;

    fwInfo.fwChecksum = Cal_Fw_Checksum(fwInfo.fwDataPtr, fwInfo.fwSize);

    ui->progressBarFwUpg->setValue(0);

    //ui->btnFwUpg->setEnabled(false);

    if(!myUsb->Usb_Find())
    {
        QMessageBox::warning(this, tr("Usb设备"), tr("没有发现USB设备，请重新插入USB设备！！！"));

        return ;
    }

    if(!myUsb->Usb_Open())
    {
        QMessageBox::warning(this, tr("Usb设备"), tr("打开失败！"));

        return ;
    }

    upgStat = UPG_STAT_FW_SIZE;

    timer->start();
}

void MyUpgrade::Upg_Handle()
{
    switch(upgStat)
    {
        case UPG_STAT_FW_SIZE:
        {
            wBuf[0] = RPT_ID_UPG_FW_SIZE;
            wBuf[1] = (uint8_t )fwInfo.fwSize;
            wBuf[2] = (uint8_t )(fwInfo.fwSize >> 8);

            myUsb->Usb_Write(wBuf, 3);

            fwInfo.fwTxTimeOutCnt = 0;

            upgStat = UPG_STAT_FW_SIZE_ACK;
            break;
        }
        case UPG_STAT_FW_SIZE_ACK:
        {
            rBuf[0] = RPT_ID_UPG_FW_ACK;
            rBuf[1] = 0x0;

            if(myUsb->Usb_Read(rBuf, 2) > 0)
            {
                fwInfo.fwTxTimeOutCnt = 0;

                if(rBuf[1] == UPG_FW_ACK)
                {
                    fwInfo.fwTxTimeOutCnt = 0;

                    fwInfo.fwTxErrCnt = 0;

                    upgStat = UPG_STAT_FW_DATA;
                }
            }

            if(++fwInfo.fwTxTimeOutCnt > 1000)
            {
                fwInfo.fwTxTimeOutCnt = 0;

                upgStat = UPG_STAT_FW_SIZE_ACK;
            }
            break;
        }
        case UPG_STAT_FW_DATA:
        {
            uint8_t i;
            wBuf[0] = RPT_ID_UPG_FW_DATA;
            wBuf[1] = (uint8_t )fwInfo.fwOffset;
            wBuf[2] = (uint8_t )(fwInfo.fwOffset >> 8);

            if(fwInfo.fwSendBytes > UPG_FW_PACK_MAX_SIZE)
            {
                wBuf[3] = UPG_FW_PACK_MAX_SIZE;

                for(i=0;i<UPG_FW_PACK_MAX_SIZE;i++)
                {
                    wBuf[4+i] = fwInfo.fwDataPtr[i];
                }

                myUsb->Usb_Write(wBuf, UPG_FW_PACK_MAX_SIZE+4);
            }
            else
            {
                wBuf[3] = fwInfo.fwSendBytes;

                for(i=0;i<fwInfo.fwSendBytes;i++)
                {
                    wBuf[4+i] = fwInfo.fwDataPtr[i];
                }

                myUsb->Usb_Write(wBuf, UPG_FW_PACK_MAX_SIZE+4);
            }

            upgStat = UGP_STAT_FW_DATA_ACK;

            break;
        }
        case UGP_STAT_FW_DATA_ACK:
        {
            rBuf[0] = RPT_ID_UPG_FW_ACK;
            rBuf[1] = 0x0;

            if(myUsb->Usb_Read(rBuf, 2) > 0)
            {
                if(rBuf[1] == UPG_FW_ACK)
                {
                    if(fwInfo.fwSendBytes > UPG_FW_PACK_MAX_SIZE)
                    {
                        fwInfo.fwSendBytes -= UPG_FW_PACK_MAX_SIZE;
                        fwInfo.fwOffset += UPG_FW_PACK_MAX_SIZE;
                        fwInfo.fwDataPtr += UPG_FW_PACK_MAX_SIZE;

                        upgStat = UPG_STAT_FW_DATA;

                        ui->progressBarFwUpg->setValue((uint32_t)fwInfo.fwOffset * 100 / fwInfo.fwSize);
                    }
                    else
                    {
                        fwInfo.fwSendBytes = 0;
                        fwInfo.fwOffset = 0;
                        fwInfo.fwDataPtr = nullptr;

                        upgStat = UPG_STAT_FW_CHECKSUM;
                    }
                }

                return ;
            }

            if(++fwInfo.fwTxTimeOutCnt > 50)
            {
                fwInfo.fwTxTimeOutCnt = 0;

                if(++fwInfo.fwTxErrCnt >= 10)
                {
                    fwInfo.fwTxErrCnt = 0;

                    upgStat = UPG_STAT_FW_ERROR;
                }
                else
                {
                    upgStat = UPG_STAT_FW_DATA;
                }
            }

            break;
        }
        case UPG_STAT_FW_CHECKSUM:
        {
            wBuf[0] = RPT_ID_UPG_FW_CHECKSUM;
            wBuf[1] = (uint8_t )fwInfo.fwChecksum;
            wBuf[2] = (uint8_t )(fwInfo.fwChecksum >> 8);

            myUsb->Usb_Write(wBuf, 3);

            fwInfo.fwTxTimeOutCnt = 0;

            fwInfo.fwTxErrCnt = 0;

            upgStat = UPG_STAT_FW_CHECKSUM_ACK;

            break;
        }
        case UPG_STAT_FW_CHECKSUM_ACK:
        {
            rBuf[0] = RPT_ID_UPG_FW_ACK;
            rBuf[1] = 0x0;

            if(myUsb->Usb_Read(rBuf, 2) > 0)
            {
                if(rBuf[1] == UPG_FW_ACK)
                {
                    fwInfo.fwTxTimeOutCnt = 0;

                    fwInfo.fwTxErrCnt = 0;

                    upgStat = UPG_STAT_FW_GET_VER;
                }
                else
                {
                    upgStat = UPG_STAT_FW_ERROR;
                }

                return ;
            }

            if(++fwInfo.fwTxTimeOutCnt > 50)
            {
                fwInfo.fwTxTimeOutCnt = 0;

                if(++fwInfo.fwTxErrCnt >= 10)
                {
                    fwInfo.fwTxErrCnt = 0;

                    upgStat = UPG_STAT_FW_ERROR;
                }
                else
                {
                    upgStat = UPG_STAT_FW_CHECKSUM;
                }
            }
            break;
        }
        case UPG_STAT_FW_GET_VER:
        {
            rBuf[0] = RPT_ID_UPG_FW_ACK;
            rBuf[1] = 0x0;
            rBuf[2] = 0x0;
            rBuf[3] = 0x0;

            if(myUsb->Usb_Read(rBuf, 4) > 0)
            {
                if(rBuf[1] == UPG_FW_ACK)
                {
                    upgStat = UPG_STAT_FW_SUCCESS;

                    return ;
                }
            }

            if(++fwInfo.fwTxTimeOutCnt > 50)
            {
                fwInfo.fwTxTimeOutCnt = 0;

                if(++fwInfo.fwTxErrCnt >= 10)
                {
                    fwInfo.fwTxErrCnt = 0;

                    upgStat = UPG_STAT_FW_ERROR;
                }
            }
            break;
        }
        case UPG_STAT_FW_SUCCESS:
        {
            ui->progressBarFwUpg->setValue(100);

            timer->stop();

            QMessageBox::warning(this, tr("Usb设备"), tr("USB升级成功！！！"));
            break;
        }
        case UPG_STAT_FW_ERROR:
        {
            timer->stop();

            QMessageBox::warning(this, tr("Usb设备"), tr("USB升级失败，请重新升级！！！"));
            break;
        }
        default: break;
    }
}



