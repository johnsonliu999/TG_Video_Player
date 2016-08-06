#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TGClientSDK.h"

#include <QStringListModel>
#include <QCompleter>


void MsgCallBackFun(const char* jsonStr, void* context)
{
    QString str(jsonStr);
    qDebug() << str;

    QRegExp rx("\"nid\":\"(\\d{18})\"");
    int pos = rx.indexIn(str);
    while(pos != -1)
    {
        qDebug() << rx.cap(1);
        pos += rx.matchedLength();
        pos = rx.indexIn(str, pos);
    }

}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TGClient_Init();
    long loginHandle = TGClient_AsyncLogin((char *)"121.201.5.141", 13000, "IPC_demo", "123445", 0, MsgCallBackFun, this);
    if (loginHandle)
        qDebug() << "Login succeed";
    else {
        qDebug() << "Login failed";
        return;
    }

    quint64 playHandle = TGClient_StartRealPlay(loginHandle, (void*)ui->widget->winId(), 144115188075856027, 1, nullptr, nullptr, nullptr, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
}



void MainWindow::on_editFinished()
{

}


