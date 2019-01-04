#ifndef RECORDER_H
#define RECORDER_H

#include <QMainWindow>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QObject>
#include <QAudioBuffer>
#include <QAudioProbe>
#include <QAudioRecorder>
#include <QMediaRecorder>
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QPoint>

#include <fftw3.h>
#include <vector>
#include <string>
#include <iostream>

#define REAL 0
#define IMAG 1
#define SCALING_MAG 20
#define LINE_EPOCH 31

class Player;

extern Player *player;

class Recorder : public QObject
{
    Q_OBJECT
public:
    Recorder(QObject *parent, QGraphicsScene *scene);
    void startRecording();
    void stopRecording();
    bool get_is_recording();
    void set_is_recording(bool b);
public slots:
    void processAudioBuffer(QAudioBuffer buffer);
private:
    QAudioRecorder *recorder;
    bool is_recording = false;
    //  brojac poziva funkcije processAudioBuffer
    unsigned int buffer_count = 0;
    std::vector<double> recent_magnitudes;
    std::vector<double> recent_avg_magn;
    std::vector<QPoint> line_dots;
    QGraphicsScene *m_scene;
    double start_magnitude;
};

#endif // RECORDER_H
