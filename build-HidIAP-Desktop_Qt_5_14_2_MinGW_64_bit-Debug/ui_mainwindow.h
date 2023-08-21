/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *widgetLogo;
    QLabel *labelLogoLine;
    QLabel *labelLogoPic;
    QLabel *labelLogoText;
    QLabel *labelLogoTitle;
    QPushButton *btnMinimize;
    QPushButton *btnClose;
    QTabWidget *tabWidget;
    QWidget *tabFwUpgrade;
    QLabel *labelFwInfo;
    QFrame *lineFw;
    QLabel *labelFwVer;
    QLabel *labelFwVerVal;
    QLabel *label;
    QLineEdit *lineEditFwPath;
    QPushButton *btnFwFile;
    QProgressBar *progressBarFwUpg;
    QPushButton *btnFwUpg;
    QLabel *labelHwInfo;
    QFrame *lineHw;
    QLabel *labelHw;
    QLabel *labelHwVerVal;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(640, 400);
        MainWindow->setMinimumSize(QSize(640, 400));
        MainWindow->setMaximumSize(QSize(640, 400));
        QFont font;
        font.setPointSize(14);
        font.setBold(false);
        font.setWeight(50);
        MainWindow->setFont(font);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        widgetLogo = new QWidget(centralwidget);
        widgetLogo->setObjectName(QString::fromUtf8("widgetLogo"));
        widgetLogo->setGeometry(QRect(0, 0, 640, 45));
        labelLogoLine = new QLabel(widgetLogo);
        labelLogoLine->setObjectName(QString::fromUtf8("labelLogoLine"));
        labelLogoLine->setGeometry(QRect(0, 0, 641, 45));
        QFont font1;
        font1.setFamily(QString::fromUtf8("MV Boli"));
        labelLogoLine->setFont(font1);
        labelLogoPic = new QLabel(widgetLogo);
        labelLogoPic->setObjectName(QString::fromUtf8("labelLogoPic"));
        labelLogoPic->setGeometry(QRect(5, 5, 35, 35));
        labelLogoText = new QLabel(widgetLogo);
        labelLogoText->setObjectName(QString::fromUtf8("labelLogoText"));
        labelLogoText->setGeometry(QRect(40, 9, 101, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("MV Boli"));
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setWeight(50);
        labelLogoText->setFont(font2);
        labelLogoText->setStyleSheet(QString::fromUtf8("color: rgb(50,254,255)"));
        labelLogoTitle = new QLabel(widgetLogo);
        labelLogoTitle->setObjectName(QString::fromUtf8("labelLogoTitle"));
        labelLogoTitle->setGeometry(QRect(210, 5, 231, 21));
        QFont font3;
        font3.setFamily(QString::fromUtf8("MV Boli"));
        font3.setPointSize(13);
        labelLogoTitle->setFont(font3);
        labelLogoTitle->setStyleSheet(QString::fromUtf8("color: rgb(50,254,255)"));
        btnMinimize = new QPushButton(widgetLogo);
        btnMinimize->setObjectName(QString::fromUtf8("btnMinimize"));
        btnMinimize->setGeometry(QRect(580, 10, 21, 20));
        btnClose = new QPushButton(widgetLogo);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setGeometry(QRect(610, 10, 21, 20));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 50, 641, 331));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Microsoft YaHei UI"));
        font4.setPointSize(12);
        font4.setBold(false);
        font4.setWeight(50);
        tabWidget->setFont(font4);
        tabFwUpgrade = new QWidget();
        tabFwUpgrade->setObjectName(QString::fromUtf8("tabFwUpgrade"));
        labelFwInfo = new QLabel(tabFwUpgrade);
        labelFwInfo->setObjectName(QString::fromUtf8("labelFwInfo"));
        labelFwInfo->setGeometry(QRect(40, 10, 80, 30));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font5.setPointSize(12);
        font5.setBold(false);
        font5.setWeight(50);
        labelFwInfo->setFont(font5);
        labelFwInfo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lineFw = new QFrame(tabFwUpgrade);
        lineFw->setObjectName(QString::fromUtf8("lineFw"));
        lineFw->setGeometry(QRect(40, 40, 640, 3));
        lineFw->setStyleSheet(QString::fromUtf8("color: rgb(218, 218, 218);"));
        lineFw->setFrameShape(QFrame::HLine);
        lineFw->setFrameShadow(QFrame::Sunken);
        labelFwVer = new QLabel(tabFwUpgrade);
        labelFwVer->setObjectName(QString::fromUtf8("labelFwVer"));
        labelFwVer->setGeometry(QRect(75, 60, 81, 30));
        QFont font6;
        font6.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font6.setPointSize(12);
        labelFwVer->setFont(font6);
        labelFwVer->setStyleSheet(QString::fromUtf8("color: rgb(200, 200, 200);"));
        labelFwVerVal = new QLabel(tabFwUpgrade);
        labelFwVerVal->setObjectName(QString::fromUtf8("labelFwVerVal"));
        labelFwVerVal->setGeometry(QRect(160, 60, 470, 30));
        QFont font7;
        font7.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font7.setPointSize(10);
        labelFwVerVal->setFont(font7);
        labelFwVerVal->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label = new QLabel(tabFwUpgrade);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(75, 120, 81, 30));
        label->setFont(font6);
        label->setStyleSheet(QString::fromUtf8("color: rgb(200, 200, 200);"));
        lineEditFwPath = new QLineEdit(tabFwUpgrade);
        lineEditFwPath->setObjectName(QString::fromUtf8("lineEditFwPath"));
        lineEditFwPath->setGeometry(QRect(160, 125, 351, 21));
        btnFwFile = new QPushButton(tabFwUpgrade);
        btnFwFile->setObjectName(QString::fromUtf8("btnFwFile"));
        btnFwFile->setGeometry(QRect(530, 125, 70, 22));
        QFont font8;
        font8.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font8.setPointSize(9);
        btnFwFile->setFont(font8);
        btnFwFile->setStyleSheet(QString::fromUtf8("color: rgb(85, 0, 255);"));
        progressBarFwUpg = new QProgressBar(tabFwUpgrade);
        progressBarFwUpg->setObjectName(QString::fromUtf8("progressBarFwUpg"));
        progressBarFwUpg->setGeometry(QRect(160, 170, 351, 23));
        QFont font9;
        font9.setFamily(QString::fromUtf8("Microsoft YaHei UI"));
        font9.setPointSize(10);
        progressBarFwUpg->setFont(font9);
        progressBarFwUpg->setStyleSheet(QString::fromUtf8(""));
        progressBarFwUpg->setValue(24);
        progressBarFwUpg->setAlignment(Qt::AlignCenter);
        btnFwUpg = new QPushButton(tabFwUpgrade);
        btnFwUpg->setObjectName(QString::fromUtf8("btnFwUpg"));
        btnFwUpg->setGeometry(QRect(530, 170, 70, 22));
        btnFwUpg->setFont(font8);
        btnFwUpg->setStyleSheet(QString::fromUtf8("color: rgb(85, 0, 255);"));
        labelHwInfo = new QLabel(tabFwUpgrade);
        labelHwInfo->setObjectName(QString::fromUtf8("labelHwInfo"));
        labelHwInfo->setGeometry(QRect(40, 210, 80, 30));
        labelHwInfo->setFont(font6);
        labelHwInfo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lineHw = new QFrame(tabFwUpgrade);
        lineHw->setObjectName(QString::fromUtf8("lineHw"));
        lineHw->setGeometry(QRect(40, 240, 640, 3));
        lineHw->setStyleSheet(QString::fromUtf8("color: rgb(218, 218, 218);"));
        lineHw->setFrameShape(QFrame::HLine);
        lineHw->setFrameShadow(QFrame::Sunken);
        labelHw = new QLabel(tabFwUpgrade);
        labelHw->setObjectName(QString::fromUtf8("labelHw"));
        labelHw->setGeometry(QRect(75, 260, 81, 30));
        labelHw->setFont(font5);
        labelHw->setStyleSheet(QString::fromUtf8("color: rgb(200, 200, 200);"));
        labelHwVerVal = new QLabel(tabFwUpgrade);
        labelHwVerVal->setObjectName(QString::fromUtf8("labelHwVerVal"));
        labelHwVerVal->setGeometry(QRect(160, 260, 470, 30));
        labelHwVerVal->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        tabWidget->addTab(tabFwUpgrade, QString());
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        labelLogoLine->setText(QString());
        labelLogoPic->setText(QString());
        labelLogoText->setText(QCoreApplication::translate("MainWindow", "Cmsemicon", nullptr));
        labelLogoTitle->setText(QCoreApplication::translate("MainWindow", "Usb Hid Upgrade Tool", nullptr));
        btnMinimize->setText(QString());
        btnClose->setText(QString());
        labelFwInfo->setText(QCoreApplication::translate("MainWindow", "\345\233\272\344\273\266\344\277\241\346\201\257", nullptr));
        labelFwVer->setText(QCoreApplication::translate("MainWindow", "\345\233\272\344\273\266\347\211\210\346\234\254", nullptr));
        labelFwVerVal->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "\345\233\272\344\273\266\345\215\207\347\272\247", nullptr));
        btnFwFile->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        btnFwUpg->setText(QCoreApplication::translate("MainWindow", "\345\233\272\344\273\266\346\233\264\346\226\260", nullptr));
        labelHwInfo->setText(QCoreApplication::translate("MainWindow", "\347\241\254\344\273\266\344\277\241\346\201\257", nullptr));
        labelHw->setText(QCoreApplication::translate("MainWindow", "\347\241\254\344\273\266\347\211\210\346\234\254", nullptr));
        labelHwVerVal->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabFwUpgrade), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
