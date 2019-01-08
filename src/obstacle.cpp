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
    this->setRect(x, -y, size_x, size_y);
    this->setBrush(QBrush(Qt::green));

}
