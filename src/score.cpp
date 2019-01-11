#include "score.h"
#include <QFont>

Score::Score(QGraphicsItem *parent): QGraphicsTextItem(parent){
    // pocetna score vrednost
    score = 0;
    // ispis score-a
    setPlainText(QString("Score: ") + QString::number(score));
    setFont(QFont("times",16));
    setDefaultTextColor(Qt::red);
    setTransform(QTransform::fromScale(1, -1));
}

void Score::increaseScore(){
    score+= 200;
    setPlainText(QString("Score: ") + QString::number(score));
}

void Score::decreaseScore(){
    score-= 200;
    if(score<0)
        score=0;
    setPlainText(QString("Score: ") + QString::number(score));
}


int Score::getScore(){
    return score;
}
