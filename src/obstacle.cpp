#include "obstacle.h"

Obstacle::Obstacle(int x, int y)
{
    int size_x = rand() % MAX_WIDTH_SIZE;
    int size_y = rand() % MAX_HEIGHT_SIZE;
    // u slucaju ako je rand dao manju stavi min
    if (size_x < MIN_WIDTH_SIZE){
        size_x = MIN_WIDTH_SIZE;
    }
    // u slucaju ako je rand dao manju stavi min
    if (size_y < MIN_HEIGHT_SIZE){
        size_y = MIN_HEIGHT_SIZE;
    }
    qDebug() << "prepreka";
    qDebug() << "Sirina" << size_x;
    qDebug() << "Visina" << size_y;

    this->setRect(0, 0, size_x, size_y);

    QBrush q;

    //  mali kaktus
    if (size_y + size_x < 150)
        q.setTextureImage(QImage(":/images/cactus5.png").scaled(size_x, size_y, Qt::IgnoreAspectRatio).mirrored());
    //  visoki kaktus
    else if (size_x < 80 &&  size_y >100)
        q.setTextureImage(QImage(":/images/cactus2.png").scaled(size_x, size_y, Qt::IgnoreAspectRatio).mirrored());
    //  siroki mali kaktus
    else if (size_x + size_y < 170 && size_x > 80)
        q.setTextureImage(QImage(":/images/cactus3.png").scaled(size_x, size_y, Qt::IgnoreAspectRatio).mirrored());
    //  siroki veliki kaktus
    else if (size_x/size_y > 1.4 && size_x > 100)
        q.setTextureImage(QImage(":/images/cactus4.png").scaled(size_x, size_y, Qt::IgnoreAspectRatio).mirrored());
    //  ostali slucajevi
    else
        q.setTextureImage(QImage(":/images/cactus1.png").scaled(size_x, size_y, Qt::IgnoreAspectRatio).mirrored());


    this->setBrush(q);

    this->setPen(QPen(Qt::transparent));

    this->setPos(x, y);

}
