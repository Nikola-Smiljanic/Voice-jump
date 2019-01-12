#ifndef GAME_OVER_H
#define GAME_OVER_H

#include <QGraphicsTextItem>

class Game_over:public QGraphicsTextItem{
public:
    Game_over(int score,int preso);
};

#endif // GAME_OVER_H
