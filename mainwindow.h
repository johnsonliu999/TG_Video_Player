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
    ~MainWindow();

signals:
    void openFile(const QString &path);
    void startPlay();
    void stopPlay();
    void jumpTo(const quint64 time);

private:

    Ui::MainWindow *ui;
    DrawThread *m_pDrawThread;
    quint64 m_totalTime;

public slots:
    void on_updateTotalTime(const QString &text);
    void on_updateCurTime(const QString &text);
    void on_updateStatusBar(const QString &text);

private slots:
    void on_playButton_clicked();
    void on_openButton_clicked();
    void on_stopButton_clicked();
    void on_sliderMoved(int value);
    void on_sliderReleased();
    void on_sliderPressed();
};

#endif // MAINWINDOW_H
