#include "game_over.h"
#include <QFont>

Game_over::Game_over(int score,int preso)
{
    setPlainText(QString("Better luck next time :(\nScore: ") + QString::number(score) +QString("\nMoved: ")+ QString::number(preso));
    setFont(QFont("times",23));
    setDefaultTextColor(Qt::red);
    setTransform(QTransform::fromScale(1, -1));
}
