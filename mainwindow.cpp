#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drawthread.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pDrawThread(new DrawThread(this))
{
    ui->setupUi(this);
    m_drawHandle = ui->widget->winId();
    m_pDrawThread->start();
}

quint64 MainWindow::getDrawHandle()
{
    return ui->widget->winId();
}

MainWindow::~MainWindow()
{
    delete ui;
}

