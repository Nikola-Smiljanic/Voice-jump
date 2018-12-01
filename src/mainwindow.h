#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "xyseriesiodevice.h"

#include <QMainWindow>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QAudioBuffer>
#include <QAudioProbe>
#include <QAudioRecorder>
#include <QMediaRecorder>
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QGraphicsScene *scene;
    QAudioRecorder *recorder;
    int x_count = 0;
public slots:
    void processBuffer(const QAudioBuffer &buffer);

private slots:

    void on_cleanPushButton_clicked();

    void on_rec1PushButton_clicked();

    void on_stop1PushButton_clicked();

    void on_rec2PushButton_clicked();

    void on_stop2PushButton_clicked();

private:
    Ui::MainWindow *ui;

    XYSeriesIODevice *m_device = nullptr;
    QAudioInput *m_audioInput = nullptr;

};

#endif // MAINWINDOW_H
