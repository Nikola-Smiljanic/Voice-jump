#include "xyseriesiodevice.h"
#include "mainwindow.h"


XYSeriesIODevice::XYSeriesIODevice(QObject *parent, QGraphicsScene *scene) :
    QIODevice(parent), scene(scene)
{
}

qint64 XYSeriesIODevice::readData(char *data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)
    return -1;
}

qint64 XYSeriesIODevice::writeData(const char *data, qint64 maxSize)
{
    //sadrzaj se menja samo kad se ponovo pozove funckija start (dugme record2)
    //inace se ponavljalju isti podaci

    static const int resolution = 4;

        if (m_buffer.isEmpty()) {
            m_buffer.reserve(sampleCount);
            for (int i = 0; i < sampleCount; ++i)
                m_buffer.append(QPointF(i, 0));
        }

        int start = 0;
        const int availableSamples = int(maxSize) / resolution;
        // stari podaci se pomeraju ulevo da naprave mesta novim (za br availableSamples)
        if (availableSamples < sampleCount) {
            start = sampleCount - availableSamples;
            for (int s = 0; s < start; ++s)
                m_buffer[s].setY(m_buffer.at(s + availableSamples).y());
        }
        //novi podaci se upisuju na kraj m_buffer-a
        for (int s = start; s < sampleCount; ++s, data += resolution)
            m_buffer[s].setY(qreal(uchar(*data) -128) / qreal(128));

        draw_dots(m_buffer);

        return (sampleCount - start) * resolution;
}

void XYSeriesIODevice::draw_dots(QVector<QPointF> m_buffer){
    scene->clear();

    for(int i=0; i< m_buffer.size(); i++){
            QPointF p = m_buffer[i];
            std::cout << "x: " << p.x() << " y: " << p.y() << std::endl;
            //uspravna linija
            scene->addLine( p.x(), 0, p.x(), p.y()*100, QPen(Qt::blue));
    }

    scene->views()[0]->update();
}

