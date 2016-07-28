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
    long handle = TGClient_AsyncLogin((char *)"121.201.5.141", 13000, "IPC_demo", "123445", 0, MsgCallBackFun, this);
    if (handle)
        qDebug() << "Login succeed";
    else {
        qDebug() << "Login failed";
        return;
    }
    TGClient_GetDeviceList(handle);
    if(!TGClient_Logout(handle))
        qDebug() << "Log out succeed";
    else
    {
        qDebug() << "Log out failed";
    }

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


