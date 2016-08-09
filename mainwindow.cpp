#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drawthread.h"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pDrawThread(new DrawThread(this)),
    m_totalTime(0)
{
    ui->setupUi(this);
    m_pDrawThread->m_dispHandle = ui->widget->winId();

    connect(this, &MainWindow::openFile, m_pDrawThread, &DrawThread::on_openFile);
    connect(this, &MainWindow::startPlay, m_pDrawThread, &DrawThread::on_startPlay);
    connect(this, &MainWindow::stopPlay, m_pDrawThread, &DrawThread::on_stopPlay);
    connect(this, &MainWindow::jumpTo, m_pDrawThread, &DrawThread::on_jumpTo);
    connect(m_pDrawThread, &DrawThread::updateTotalTime, this, &MainWindow::on_updateTotalTime);
    connect(m_pDrawThread, &DrawThread::updateCurTime, this, &MainWindow::on_updateCurTime);
    connect(m_pDrawThread, &DrawThread::updateStatusBar, this, &MainWindow::on_updateStatusBar);
    connect(m_pDrawThread, &DrawThread::updateSlider, ui->horizontalSlider, &QSlider::setValue);
    connect(ui->horizontalSlider, &QSlider::sliderMoved, this, &MainWindow::on_sliderMoved);
    connect(ui->horizontalSlider, &QSlider::sliderReleased, this, &MainWindow::on_sliderReleased);
    connect(ui->horizontalSlider, &QSlider::sliderPressed, this, &MainWindow::on_sliderPressed);
    m_pDrawThread->start();
}

MainWindow::~MainWindow()
{
    m_pDrawThread->quit();
    m_pDrawThread->wait();
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

void MainWindow::on_updateStatusBar(const QString &text)
{
    statusBar()->showMessage(text, 2000);
}

void MainWindow::on_playButton_clicked()
{
    emit startPlay();
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

void MainWindow::on_stopButton_clicked()
{
    emit stopPlay();
}

void MainWindow::on_sliderMoved(int value)
{
    quint64 curTime = value*m_totalTime/ui->horizontalSlider->maximum();
    int sec = curTime/1000%60;
    int min = curTime/1000/60%60;
    QString str = QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    ui->curTimeLabel->setText(str);
}

void MainWindow::on_sliderReleased()
{
    qDebug() << "Released :" << ui->horizontalSlider->value();
    quint64 curTime = ui->horizontalSlider->value()*m_totalTime/ui->horizontalSlider->maximum();
    qDebug() << "curTime :" << curTime;
    emit jumpTo(curTime);
    connect(m_pDrawThread, &DrawThread::updateSlider, ui->horizontalSlider, &QSlider::setValue);
}

void MainWindow::on_sliderPressed()
{
    qDebug() << "Pressed :" << ui->horizontalSlider->value();
    disconnect(m_pDrawThread, &DrawThread::updateSlider, ui->horizontalSlider, &QSlider::setValue);
}
