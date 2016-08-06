#include <QCoreApplication>
#include <QApplication>

#include "mainwindow.h"

#include "tgfile.h"

using namespace std;

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);

//    MainWindow mainWindow;

//    mainWindow.show();

    TGFile f("0_005_0_20160728152348_940_2_1.tg");
    f.open(AbstractTGFile::MODE_READ);
    return 0;
//    return a.exec();
}
