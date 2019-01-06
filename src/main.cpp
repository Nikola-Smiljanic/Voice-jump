#include <QtGui>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "player.h"
#include "recorder.h"

#define VIEW_WIDTH 1000

Recorder *recorder;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //  inicijalizuje se scena
    QGraphicsScene scene;
    scene.setSceneRect(-1600, -300, 4000, 800);

    //  kreira se pogled za scenu
    QGraphicsView view(&scene);

    Player *player = new Player(&view, &scene);
    recorder = new Recorder(&scene, &scene);

    scene.addLine(0, 0, VIEW_WIDTH, 0, QPen(Qt::black, 1));

    scene.addItem(player);

    player->drawObsticles(0);
    view.setWindowTitle("voice jump");

    view.scale(1, -1);
    view.setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    view.setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view.show();

    return app.exec();
}
