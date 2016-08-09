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
public:
    explicit MainWindow(QWidget *parent = 0);
    quint64 getDrawHandle();
    ~MainWindow();

signals:
    void openFile(const QString &path);
    void startPlay();

private:

    Ui::MainWindow *ui;
    DrawThread *m_pDrawThread;

public slots:
    void on_updateTotalTime(const QString &text);
    void on_updateCurTime(const QString &text);
    void on_updateStatusBar(const QString &text);

private slots:
    void on_playButton_clicked();
    void on_endButton_clicked();
    void on_openButton_clicked();
};

#endif // MAINWINDOW_H
