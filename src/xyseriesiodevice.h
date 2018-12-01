#ifndef XYSERIESIODEVICE_H
#define XYSERIESIODEVICE_H
// izmenjena klasa iz primera https://doc.qt.io/qt-5.11/qtcharts-audio-example.html


#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QtCore/QIODevice>
#include <QtCore/QPointF>
#include <QtCore/QVector>
#include <iostream>

class XYSeriesIODevice : public QIODevice
{
    Q_OBJECT
public:
    explicit XYSeriesIODevice(QObject *parent = nullptr, QGraphicsScene *scene= nullptr);

    static const int sampleCount = 1024;
protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;
private:
    QVector<QPointF> m_buffer;
    QGraphicsScene *scene;

    //funkcija iscrtava linije od x ose do tacaka koje su prosledjene
    void draw_dots(QVector<QPointF> m_buffer);
};

#endif // XYSERIESIODEVICE_H
