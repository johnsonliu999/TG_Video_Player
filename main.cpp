#include <QCoreApplication>
#include <QApplication>
#include <QDebug>

#include "mainwindow.h"

//#include "b.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWindow;

    mainWindow.show();

    return a.exec();
}
