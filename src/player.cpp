#include "player.h"
#include "score.h"

extern Score* score;

Player::Player(QGraphicsView *view, QGraphicsScene *scene = nullptr)
    : m_view(view), m_scene(scene)
{
    qDebug() << "igrac";

    this->setRect(0, 0, 50, 50);
    QBrush q;
    q.setTextureImage(QImage(":/images/balloon1.png").scaled(50, 50).mirrored());
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

        if(!recorder->get_is_recording() && x() > 0){

            //  ako se igrac (balon) nalazi na liniji krece se po njoj
            if (currentPosition>0 && (position - recordingStartPosition+skip) < (int)movementLine.size()){

                currentPosition--;
                setPos(movementLine[currentPosition+skip].x()-20.5,movementLine[currentPosition+skip].y()-15.5);
                position--;
                if(position%300 ==0){
                    score->decreaseScore();
                }

            }
            //  ako se igrac (balon) nalazi van linije krece se ravno po podu
            else{

                qDebug() << "krece se u levo ravno" << position;
                setPos(x()-1, 0);
            }
            position--;
            m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() - 1);
            score->setPos(-50 + x(), 400);
        }
    }

    else if(event->key() == Qt::Key_Right){

        if(!recorder->get_is_recording()){

            //  ako se igrac (balon) nalazi na liniji krece se po njoj
            if (movementLine.size() > 0 && currentPosition<sizeOfTest-skip && position >= recordingStartPosition){

                setPos(movementLine[currentPosition+skip].x()-20.5,movementLine[currentPosition+skip].y()-15.5);
                currentPosition++;
                position++;
                if(position%300 ==0){
                    score->increaseScore();
                }

            }
            //  ako se igrac (balon) nalazi van linije krece se ravno po podu
            else{
                qDebug() << "krece se u desno ravno" << position;
                setPos(x()+1, 0);
            }

            position++;
            m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() + 1);
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
                score->gameOverScore();

                //  ako je balon bio na liniji zvuka kad je naisao na kaktus
                if(movementLine.size() > 0){
                    //  brise se prikaz linije
                    currentPosition = 0;
                    movementLine.clear();
                    sizeOfTest = 0;

                    recorder->delete_lines();
                }
                //  igrac se vraca unazad
                setPos(x()-200, 0);
                m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() - 200);
            }
        }


    }

    else if(event->key() == Qt::Key_Space){
        qDebug() << "space";
        if (!recorder->get_is_recording()){
            //  igrac (balon) se spusta na pod ako je bio iznad
            setPos(x(), 0);
            recorder->delete_lines();
            recordingStartPosition = position;
            currentPosition = 0;

            recorder->startRecording(x(), y());
            qDebug() << "recording started";
        }
    }

    else if(event->key() == Qt::Key_S){
        if(recorder->get_is_recording()){

            movementLine = recorder->stopRecording();
            sizeOfTest =  movementLine.size();

            qDebug() << "recording stopped";
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

    m_scene->addLine(count*VIEW_WIDTH-100, 0, (count+1)*VIEW_WIDTH, 0, QPen(Qt::green, 1));

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
