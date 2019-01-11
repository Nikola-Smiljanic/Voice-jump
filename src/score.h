#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>

class Score: public QGraphicsTextItem{
public:
    Score(QGraphicsItem * parent=0);
    void increaseScore();
    void decreaseScore();
    void gameOverScore();
    int getScore();
private:
    int score;
    int scoreMult;
};

#endif // SCORE_H
