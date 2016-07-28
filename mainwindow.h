#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>

class QTcpSocket;
class QTime;
class QStringListModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QStringList list;
    QStringListModel* model;


public slots:

private slots:
    void on_pushButton_clicked();
    void on_editFinished();
};

#endif // MAINWINDOW_H
