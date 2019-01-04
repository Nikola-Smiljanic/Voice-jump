#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>

#define MIN_WIDTH_SIZE 20
#define MAX_WIDTH_SIZE 40

#define MIN_HEIGHT_SIZE 50
#define MAX_HEIGHT_SIZE 70

class Obstacle : public QGraphicsRectItem
{
public:
    Obstacle(int x, int y);
};

#endif // OBSTACLE_H
