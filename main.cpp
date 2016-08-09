#include <QCoreApplication>
#include <QApplication>

#include "mainwindow.h"

#include "tgfile.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWindow;

    mainWindow.show();

    return a.exec();
}
