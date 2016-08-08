#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drawthread.h"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pDrawThread(nullptr)
{
    ui->setupUi(this);
    m_drawHandle = ui->widget->winId();
}

quint64 MainWindow::getDrawHandle()
{
    return ui->widget->winId();
}

MainWindow::~MainWindow()
{
    delete ui;
}

inline void MainWindow::startDrawThread()
{
    QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("Open TG File"), QDir::currentPath(), QStringLiteral("TG File (*.tg)"));
    if (fileName.isNull()) {
        qDebug() << "No file selected";
        return ;
    }

    m_pDrawThread = new DrawThread(fileName, this);
    m_pDrawThread->start();
}


void MainWindow::on_playButton_clicked()
{
    if (!m_pDrawThread) {
        startDrawThread();
    } else if (m_pDrawThread->isFinished()) {
        delete m_pDrawThread;
        startDrawThread();
    } else {
        qDebug() << "Please end first";
        return ;
    }
}

void MainWindow::on_endButton_clicked()
{
    if (m_pDrawThread) m_pDrawThread->m_stop = true;
    m_pDrawThread->wait();
}
