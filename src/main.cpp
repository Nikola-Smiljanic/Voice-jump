#include <QtGui>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QImage>
#include <QBrush>
#include <QTileRules>
#include <QPixmap>
#include <QGraphicsPixmapItem>

#include "player.h"
#include "recorder.h"
#include "score.h"

#define VIEW_WIDTH 1000

Recorder *recorder;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //  inicijalizuje se scena
    QGraphicsScene scene;
    scene.setSceneRect(-2100, -180, 5000, 700);
    //scene.setBackgroundBrush(QBrush(QImage(":/images/background.png").scaled(5000, 700).mirrored()));

    // seed for random;
    srand(time(NULL));
    QGraphicsPixmapItem *background0 = new QGraphicsPixmapItem(QPixmap(":/images/background.png"));
    background0->setPos(0, 470);
    background0->setScale(-1);
    background0->setZValue(-1);
    scene.addItem(background0);

    //  kreira se pogled za scenu
    QGraphicsView view(&scene);
    Player *player = new Player(&view, &scene);
    QGraphicsItem *score = new Score();
    recorder = new Recorder(&scene, &scene);


    scene.addLine(0, 0, VIEW_WIDTH, 0, QPen(Qt::black, 1));
    QGraphicsRotation rot;

    scene.addItem(player);

    player->drawObsticles(0);
    view.setWindowTitle("voice jump");

    view.scale(1, -1);
    score->setPos(-50,380);
    scene.addItem(score);
    view.setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    view.setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view.show();

    return app.exec();
}
