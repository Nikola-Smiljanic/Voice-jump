#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>

class Score: public QGraphicsTextItem{
public:
    Score(QGraphicsItem * parent=0);
    void increaseScore();
    void increaseScoreFlowers();
    void decreaseScore();
    void decreaseCactusScore();
    void gameOverScore();
    void resetScore();
    int getScore();
    int getLives();
private:
    int score;
    int scoreMult;
    int lives = 3;
};

#endif // SCORE_H
