#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drawthread.h"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pDrawThread(new DrawThread(this))
{
    ui->setupUi(this);
    m_drawHandle = ui->widget->winId();

    connect(this, &MainWindow::openFile, m_pDrawThread, &DrawThread::openFile);
    connect(m_pDrawThread, &DrawThread::updateTotalTime, this, &MainWindow::on_updateTotalTime);
    connect(m_pDrawThread, &DrawThread::updateCurTime, this, &MainWindow::on_updateCurTime);
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

void MainWindow::on_updateTotalTime(const QString &text)
{
    ui->totalTimeLabel->setText(text);
}

void MainWindow::on_updateCurTime(const QString &text)
{
    ui->curTimeLabel->setText(text);
}

void MainWindow::on_playButton_clicked()
{
//    if (!m_pDrawThread) {
//        startDrawThread();
//    } else if (m_pDrawThread->isFinished()) {
//        delete m_pDrawThread;
//        startDrawThread();
//    } else {
//        qDebug() << "Please end first";
//        return ;
//    }
}

void MainWindow::on_endButton_clicked()
{
    m_pDrawThread->wait();
}

void MainWindow::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("Open TG File"), QDir::currentPath(), QStringLiteral("TG File (*.tg)"));
    if (fileName.isNull()) {
        qDebug() << "No file selected";
        return ;
    }
    emit openFile(fileName);
}
