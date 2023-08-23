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

    static uint8_t *fwBuf = nullptr;

    if(fwBuf != nullptr)
    {
        delete [] fwBuf;
    }
    fwBuf = new uint8_t[fwInfo.fwSize];

    binFileData.readRawData((char *)fwBuf, static_cast<int>(fwInfo.fwSize));

    fwInfo.fwDataPtr = fwBuf;

    ui->progressBarFwUpg->setValue(0);

    ui->btnFwUpg->setEnabled(false);

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
            if(++fwInfo.fwTxTimeOutCnt > 500)
            {
                fwInfo.fwTxTimeOutCnt = 0;

                uint8_t retVal = 0;

                retVal = myUsb->Usb_Read(rBuf, 2);

                if(retVal == 0)
                {
                    upgStat = UPG_STAT_FW_SIZE_ACK;
                }
                else
                {
                    upgStat = UPG_STAT_FW_DATA;
                }
            }
            break;
        }
        case UPG_STAT_FW_DATA:
        {
            break;
        }
        default: break;
    }
}
