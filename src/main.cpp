#include <QtGui>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "player.h"
#include "recorder.h"

#define VIEW_WIDTH 1000

Recorder *recorder;
Player *player;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //  inicijalizuje se scena
    QGraphicsScene scene;
    scene.setSceneRect(0, -200, 900, 400);

    //  kreira se pogled za scenu
    QGraphicsView view(&scene);

    player = new Player(&view, &scene);
    recorder = new Recorder(&scene, &scene);

    scene.addItem(player);
    player->drawObsticles(0);


    scene.addLine(0, 0, 1000, 0, QPen(Qt::black, 1));

    // ne radi to sto treba
    view.centerOn(player);

    view.setWindowTitle("voice jump");
    view.resize(VIEW_WIDTH, 500);
    view.scale(1, -1);

    view.show();

    return app.exec();
}
