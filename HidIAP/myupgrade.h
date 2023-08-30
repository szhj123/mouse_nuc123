#ifndef MYUPGRADE_H
#define MYUPGRADE_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myusb.h"

#include <QObject>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QTimer>

#define RPT_ID_UPG_FW_SIZE                    0x70
#define RPT_ID_UPG_FW_DATA                    0x71
#define RPT_ID_UPG_FW_CHECKSUM                0x72
#define RPT_ID_UPG_FW_ACK                     0x73

#define UPG_FW_NAK                            0x00
#define UPG_FW_ACK                            0x01
typedef enum
{
    UPG_STAT_FW_SIZE = 0,
    UPG_STAT_FW_SIZE_ACK,
    UPG_STAT_FW_DATA,
    UGP_STAT_FW_DATA_ACK,
    UPG_STAT_FW_CHECKSUM,
    UPG_STAT_FW_CHECKSUM_ACK,
}upg_stat_t;

typedef struct _fw_info_t
{
    int fwSize;
    int fwOffset;
    uint8_t *fwDataPtr;
    int fwTxLen;
    int fwTxTimeOutCnt;
    int fwTxErrCnt;

    uint8_t fwVerBuild;
    uint8_t fwVerMinor;
    uint8_t fwVerMajor;
}fw_info_t;

Q_DECLARE_METATYPE(fw_info_t);

class MyUpgrade : public QWidget
{
    Q_OBJECT
public:
    explicit MyUpgrade(QWidget *parent = nullptr, Ui::MainWindow *ui = nullptr, MyUsb *myUsb = nullptr);

private:
    Ui::MainWindow *ui;
    MyUsb *myUsb;
    QTimer *timer;
    fw_info_t fwInfo;
    upg_stat_t upgStat;
    uint8_t progressVal;
    uint8_t wBuf[64];
    uint8_t rBuf[64];
private slots:
    void on_btnFwFile_Clicked(void );
    void on_btnFwUpg_Clicked(void );
    void Upg_Handle(void );
signals:

};

#endif // MYUPGRADE_H
