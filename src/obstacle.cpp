#include "obstacle.h"

Obstacle::Obstacle(int x, int y)
{
    qDebug() << "prepreka";
    int size_x = rand() % MIN_WIDTH_SIZE + MAX_WIDTH_SIZE;
    int size_y = rand() % MIN_HEIGHT_SIZE + MAX_HEIGHT_SIZE;
    this->setRect(x, -y, size_x, size_y);
    this->setBrush(QBrush(Qt::green));

}
