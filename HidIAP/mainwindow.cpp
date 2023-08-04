#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "hidapi-win/include/hidapi.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);

    setFixedSize(this->width(),this->height());

}

MainWindow::~MainWindow()
{
    delete ui;
}

