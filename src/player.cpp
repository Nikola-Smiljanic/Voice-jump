#include "player.h"

std::vector<QPoint> test;
int sizeOfTest;
int currentPosition;
int skip = 20;
Player::Player(QGraphicsView *view, QGraphicsScene *scene = nullptr)
    : m_view(view), m_scene(scene)
{
    qDebug() << "igrac";

    this->setRect(0, 0, 50, 50);
    this->setBrush(QBrush(Qt::red));

    //  igrac je ispred ostalih objekata na sceni
    this->setZValue(1);

    //  igrac se stavlja u fokus
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFocus();

}

void Player::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Left){
        if(!recorder->get_is_recording()){
//            setPos(x(), y());
            if (currentPosition>0){
                setPos(test[currentPosition+skip].x()-20.5,test[currentPosition+skip].y()-15.5);
                currentPosition--;
                m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() - 1);
            }
            else{
                qDebug() << "ne moze vise levo";

            }
        }
    }

    else if(event->key() == Qt::Key_Right){
        if(!recorder->get_is_recording()){
            if (currentPosition<sizeOfTest-skip){
//            setPos(x()+10, y());
                setPos(test[currentPosition+skip].x()-20.5,test[currentPosition+skip].y()-15.5);
                currentPosition++;
                m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() + 1);
            }
            else{
                qDebug() << "ne moze vise desno";
            }
        }
        if( ((int)x()) / VIEW_WIDTH ==  new_obsticales_group_count){
            drawObsticles(((int)x())/VIEW_WIDTH + 1);
            new_obsticales_group_count++;
        }

    }

    else if(event->key() == Qt::Key_Space){
        qDebug() << "space";
        recorder->startRecording(x(), y());
    }

    else if(event->key() == Qt::Key_S){
        qDebug() << "stop";
        test = recorder->stopRecording();
        sizeOfTest =  test.size();
        currentPosition = 0;
    }

    // popraviti da view prati igraca
    //m_view->centerOn(this);
}

void Player::drawObsticles(int count){

    int num_of_obsticles = 3;
    for (int i=1; i<=num_of_obsticles; i++){
        Obstacle *o = new Obstacle(count*VIEW_WIDTH + i*100 + i*400/(num_of_obsticles-1), 0);
        m_scene->addItem(o);
    }

    m_scene->addLine(count*VIEW_WIDTH-100, 0, (count+1)*VIEW_WIDTH, 0, QPen(Qt::black, 1));

}




