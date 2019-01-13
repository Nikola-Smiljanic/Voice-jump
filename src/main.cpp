#include <QtGui>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "player.h"
#include "recorder.h"
#include "score.h"

#define VIEW_WIDTH 1000

Recorder *recorder;
Score *score;



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //  inicijalizuje se scena
    QGraphicsScene scene;
    scene.setSceneRect(-2100, -180, 5000, 700);

    // seed for random;
    srand(time(NULL));

    //  kreira se pogled za scenu
    QGraphicsView view(&scene);
    view.scale(1, -1);

    Player *player = new Player(&view, &scene);

    recorder = new Recorder(&scene, &scene);

    scene.addItem(player);

    player->drawBackground(-1);

    player->drawBackground(0);
    player->drawObsticles(0);
    player->drawFlowers(0);

    player->drawBackground(1);
    player->drawObsticles(1);
    player->drawFlowers(1);

    view.setWindowTitle("voice jump");

    score = new Score();
    score->setPos(-50, 400);
    scene.addItem(score);

    view.setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    view.setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view.show();

    return app.exec();
}
