#include "flower.h"

Flower::Flower(int x, int y)
{
    int size_x = rand() % MAX_WIDTH_SIZE - 50;
    int size_y = rand() % MAX_HEIGHT_SIZE - 1000;
    // u slucaju ako je rand dao manju stavi min
    if (size_x < MIN_WIDTH_SIZE){
        size_x = MIN_WIDTH_SIZE;
    }
    // u slucaju ako je rand dao manju vrednost od 150, stavi 150
    if (size_y < 150){
        size_y = 150;
    }
    qDebug() << "cvet";

   this->setRect(0, 0, size_x, size_x);

    QBrush q;

    q.setTextureImage(QImage(":/images/flower.png").scaled(size_x, size_x, Qt::IgnoreAspectRatio).mirrored());


    this->setBrush(q);

    this->setPen(QPen(Qt::transparent));

    this->setPos(size_x+x, y);

}
