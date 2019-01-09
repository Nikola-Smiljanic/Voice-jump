#include "score.h"
#include <QFont>

Score::Score(QGraphicsItem *parent): QGraphicsTextItem(parent){
    // pocetna score vrednost
    score = 0;
    // ispis score-a
    setPlainText(QString("Score: ") + QString::number(score));
    setFont(QFont("times",16));
    setDefaultTextColor(Qt::red);

}

void Score::increaseScore(){
    score++;
    setPlainText(QString("Score: ") + QString::number(score));
}

int Score::getScore(){
    return score;
}
