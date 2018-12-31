#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QDebug>

#include <fftw3.h>
#include <vector>
#include <string>
#include <iostream>
#include <cfloat>

#define REAL 0
#define IMAG 1
#define SCALING_MAG 10


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

    //  brojac poziva funkcije processAudioBuffer
    unsigned int buffer_count = 0;
    std::vector<double> recent_magnitudes;
public slots:
    void processAudioBuffer(QAudioBuffer buffer);

private slots:

    void on_cleanPushButton_clicked();

    void on_recordPushButton_clicked();

    void on_stopPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QAudioInput *m_audioInput = nullptr;

};

#endif // MAINWINDOW_H
