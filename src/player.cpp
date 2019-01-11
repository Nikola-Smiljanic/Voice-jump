#include "player.h"
#include "score.h"
//variables
std::vector<QPoint> movementLine; // linija kretanja
std::vector<QPoint> testTmp; // linija kretanja posle prve
int sizeOfTest; //velicina kretanja
int currentPosition = 0; //treutna pozicija kretanja
int skip = 20; // preskacemo prvih 20 zbog loseg ucitavanja zvuka
bool startDots = true; // za ucitavanje kretanja
int moveRScore = 0;
bool isRecording = false;
extern Score* score;

Player::Player(QGraphicsView *view, QGraphicsScene *scene = nullptr)
    : m_view(view), m_scene(scene)
{
    qDebug() << "igrac";

    this->setRect(0, 0, 50, 50);
    QBrush q;
    q.setTextureImage(QImage(":/images/balloon.png").scaled(50, 50).mirrored());
    this->setBrush(q);

    this->setPen(QPen(Qt::transparent));

    //  igrac je ispred ostalih objekata na sceni
    this->setZValue(1);

    //  igrac se stavlja u fokus za setPos()
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFocus();


}

void Player::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Left){
        if(!recorder->get_is_recording()){
            if (currentPosition>0){
                if (movementLine[currentPosition+skip].y()>0){
                    setPos(movementLine[currentPosition+skip].x()-20.5,movementLine[currentPosition+skip].y()-15.5);
                    currentPosition--;
                    if(currentPosition%300 ==0){
                        score->decreaseScore();
                    }
                    m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() - 1);
                }
                else{
                    setPos(movementLine[currentPosition+skip].x()-20.5,-15.5);
                    currentPosition--;
                    score->decreaseScore();
                    m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() - 1);
                }
            }
            else{
                qDebug() << "krece se u levo ravno";
                setPos(x()-1, y());              
                m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() - 1);
            }
            score->setPos(-50 + x(), 380);
        }
    }

    else if(event->key() == Qt::Key_Right){
        if(!recorder->get_is_recording()){
            if (currentPosition<sizeOfTest-skip){
                if (movementLine[currentPosition+skip].y()>0){
                    setPos(movementLine[currentPosition+skip].x()-20.5,movementLine[currentPosition+skip].y()-15.5);
                    currentPosition++;
                    m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() + 1);
                    if(currentPosition%300 ==0){
                        score->increaseScore();
                    }
                 }
                else{
                    setPos(movementLine[currentPosition+skip].x()-20.5,-15.5);
                    currentPosition++;
                    m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() + 1);
                }
            }
            else{
                qDebug() << "krece se u desno ravno";
                setPos(x()+1, y());
                score->increaseScore();
                m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() + 1);
            }

            score->setPos(-50 + x(), 400);
        }
        if( ((int)x()) / VIEW_WIDTH ==  new_obsticales_group_count){
            drawBackground(((int)x())/VIEW_WIDTH + 2);
            drawObsticles(((int)x())/VIEW_WIDTH + 2);
            new_obsticales_group_count++;
        }

        //  provera da li se igrac sudario s kaktusom
        const auto collision_obsticales = scene()->items(QPolygonF({ mapToScene(0, 0),
                                                                     mapToScene(50, 0),
                                                                     mapToScene(50, 50)
                                                                    }));

        for (auto item: collision_obsticales) {
            if (item == this)
                continue;

            else if (item->type() == Obstacle::Type){
                qDebug() << "Kaktus!";
                score->decreaseScore();

                if(movementLine.size() != 0){
                    //  vraca se na pocetak linije i brise se prikaz linije
                    currentPosition = 0;
                    double diff = x() - (movementLine[currentPosition+skip].x()-20.5);
                    setPos(movementLine[currentPosition+skip].x()-20.5,movementLine[currentPosition+skip].y()-15.5);
                    m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() - diff);

                    //  brise se linija za kretanje, TODO popraviti
                    movementLine.resize(0);
                    testTmp.resize(0);

                    // brisu se sve linije sa scene
                    QList<QGraphicsItem *> all = m_scene->items();
                    for(QGraphicsItem *i : all){
                        if (i->type() == QGraphicsLineItem::Type){
                            m_scene->removeItem(i);
                            delete(i);
                        }
                    }
                    //  vraca se linija za x osu
                    m_scene->addLine(-200, 0, 5000, 0, QPen(Qt::black, 1));

                }
                else{
                    setPos(x()-100, 0);
                    m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() - 100);
                }

            }
        }


    }

    else if(event->key() == Qt::Key_Space){
        qDebug() << "space";
        if (!isRecording){
            recorder->startRecording(x(), y());
            qDebug() << "recording started";
            isRecording = true;
        }
    }

    else if(event->key() == Qt::Key_S){
        if(isRecording){
            qDebug() << "stop";
            if(startDots){
                movementLine = recorder->stopRecording();
                sizeOfTest =  movementLine.size();
                startDots = false;
            }
            else{
                testTmp = recorder->stopRecording();
                currentPosition = movementLine.size();
                movementLine.insert(movementLine.end(),testTmp.begin(),testTmp.end());
                sizeOfTest =  movementLine.size();
            }
            isRecording=false;
            qDebug() << "recording finished";

        }
    }

}

void Player::drawObsticles(int count){

    int num_of_obsticles = 3;
    for (int i=1; i<=num_of_obsticles; i++){
        Obstacle *o = new Obstacle(count*VIEW_WIDTH + i*100 + i*400/(num_of_obsticles-1), 0);
        qDebug() <<" POZ:" << count*VIEW_WIDTH + i*100 + i*400/(num_of_obsticles-1);
        m_scene->addItem(o);
    }

    m_scene->addLine(count*VIEW_WIDTH-100, 0, (count+1)*VIEW_WIDTH, 0, QPen(Qt::black, 1));

}

void Player::drawBackground(int count){
    QGraphicsPixmapItem *background0 = new QGraphicsPixmapItem(QPixmap(":/images/background.png"));
    background0->setPos((count+1)*1000, 700);
    background0->setScale(-1.6);
    background0->setZValue(-1.1);
    m_scene->addItem(background0);

    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap(":/images/background.png"));
    background->setPos((count+1)*1000, 465);
    background->setScale(-1);
    background->setZValue(-1);
    m_scene->addItem(background);
}




