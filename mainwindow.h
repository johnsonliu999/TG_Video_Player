#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DrawThread;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class DrawThread;
public:
    explicit MainWindow(QWidget *parent = 0);
    quint64 getDrawHandle();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    quint64 m_drawHandle;
    DrawThread *m_pDrawThread;

public slots:

private slots:
};

#endif // MAINWINDOW_H
