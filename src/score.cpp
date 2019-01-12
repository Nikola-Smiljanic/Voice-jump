#include "score.h"
#include <QFont>

Score::Score(QGraphicsItem *parent): QGraphicsTextItem(parent){
    // pocetna score vrednost
    score = 0;
    scoreMult = 1;
    // ispis score-a
    setPlainText(QString("Score: ") + QString::number(score));
    setFont(QFont("times",16));
    setDefaultTextColor(Qt::red);
    setTransform(QTransform::fromScale(1, -1));
}

void Score::increaseScore(){
    score+= 100*scoreMult;
    scoreMult++;
    setPlainText(QString("Score: ") + QString::number(score));
}

void Score::decreaseScore(){
    scoreMult--;
    score-= 100*scoreMult;
    if(score<0)
        score=0;
    setPlainText(QString("Score: ") + QString::number(score));
}

void Score::gameOverScore(){
    score= 0;
    scoreMult= 1;
    setPlainText(QString(""));

}


int Score::getScore(){
    return score;
}

void Score::resetScore(){
    score= 0;
    scoreMult= 1;
    setPlainText(QString("Score: ") + QString::number(score));
}
