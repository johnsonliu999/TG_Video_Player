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
    void startDrawThread();

    Ui::MainWindow *ui;
    quint64 m_drawHandle;
    DrawThread *m_pDrawThread;

public slots:

private slots:
    void on_playButton_clicked();
    void on_endButton_clicked();
};

#endif // MAINWINDOW_H
