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

    ui->tabWidget->setTabText(0, "固件更新");

    connect(ui->btnMinimize, SIGNAL(clicked()), this, SLOT(btn_minimized_clicked()));

    connect(ui->btnClose, SIGNAL(clicked()), qApp, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int height = ui->widgetLogo->height();

    if(event->y() < height)
    {
        lastPoint = event->globalPos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int height = ui->widgetLogo->height();

    if(event->y() < height)
    {
        int dx = event->globalX() - lastPoint.x();
        int dy = event->globalY() - lastPoint.y();

        lastPoint = event->globalPos();
        this->move(this->x()+dx, this->y()+dy);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    int height = ui->widgetLogo->height();

    if(event->y() < height)
    {
        int dx = event->globalX() - lastPoint.x();
        int dy = event->globalY() - lastPoint.y();

        this->move(this->x()+dx, this->y()+dy);
    }
}

void MainWindow::btn_minimized_clicked()
{
    this->showMinimized();
}

