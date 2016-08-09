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
    m_pDrawThread->m_dispHandle = ui->widget->winId();

    connect(this, &MainWindow::openFile, m_pDrawThread, &DrawThread::openFile);
    connect(this, &MainWindow::startPlay, m_pDrawThread, &DrawThread::on_startPlay);
    connect(this, &MainWindow::stopPlay, m_pDrawThread, &DrawThread::on_stopPlay);
    connect(m_pDrawThread, &DrawThread::updateTotalTime, this, &MainWindow::on_updateTotalTime);
    connect(m_pDrawThread, &DrawThread::updateCurTime, this, &MainWindow::on_updateCurTime);
    connect(m_pDrawThread, &DrawThread::updateStatusBar, this, &MainWindow::on_updateStatusBar);
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

void MainWindow::on_endButton_clicked()
{
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
