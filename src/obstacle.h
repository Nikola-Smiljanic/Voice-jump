#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>

#define MIN_WIDTH_SIZE 50
#define MAX_WIDTH_SIZE 120

#define MIN_HEIGHT_SIZE 50
#define MAX_HEIGHT_SIZE 160

class Obstacle : public QGraphicsRectItem
{
public:
    Obstacle(int x, int y);
};

#endif // OBSTACLE_H
