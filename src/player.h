#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <QKeyEvent>
#include "recorder.h"
#include "obstacle.h"

#define VIEW_WIDTH 1000



extern Recorder *recorder;
//extern QGraphicsScene scene;

class Player : public QGraphicsRectItem
{
public:
    Player(QGraphicsView *view, QGraphicsScene *scene);
    void keyPressEvent(QKeyEvent *event);
    void drawObsticles(int count);
    void drawBackground(int count);
    //void keyReleaseEvent(QKeyEvent *event);

private:
    QGraphicsView *m_view;
    QGraphicsScene *m_scene;
    int new_obsticales_group_count = 0;
};

#endif // PLAYER_H
