#include "player.h"

//variables
std::vector<QPoint> test; // linija kretanja
std::vector<QPoint> testTmp; // linija kretanja posle prve
int sizeOfTest; //velicina kretanja
int currentPosition = 0; //treutna pozicija kretanja
int skip = 20; // preskacemo prvih 20 zbog loseg ucitavanja zvuka
bool startDots = true; // za ucitavanje kretanja
bool isRecording = false;

Player::Player(QGraphicsView *view, QGraphicsScene *scene = nullptr)
    : m_view(view), m_scene(scene)
{
    qDebug() << "igrac";

    this->setRect(0, 0, 50, 50);
    this->setBrush(QBrush(Qt::red));

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
                if (test[currentPosition+skip].y()>0){
                    setPos(test[currentPosition+skip].x()-20.5,test[currentPosition+skip].y()-15.5);
                    currentPosition--;
                    m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() - 1);
                }
                else{
                    setPos(test[currentPosition+skip].x()-20.5,-15.5);
                    currentPosition--;
                    m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() - 1);
                }
            }
            else{
                qDebug() << "krece se u levo ravno";
                setPos(x()-1, y());
                m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() - 1);
            }
        }
    }

    else if(event->key() == Qt::Key_Right){
        if(!recorder->get_is_recording()){
            if (currentPosition<sizeOfTest-skip){
                if (test[currentPosition+skip].y()>0){
                    setPos(test[currentPosition+skip].x()-20.5,test[currentPosition+skip].y()-15.5);
                    currentPosition++;
                    m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() + 1);
                 }
                else{
                    setPos(test[currentPosition+skip].x()-20.5,-15.5);
                    currentPosition++;
                    m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() + 1);

                }
            }
            else{
                qDebug() << "krece se u desno ravno";
                setPos(x()+1, y());
                m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value() + 1);
            }
        }
        if( ((int)x()) / VIEW_WIDTH ==  new_obsticales_group_count){
            drawObsticles(((int)x())/VIEW_WIDTH + 1);
            new_obsticales_group_count++;
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
                test = recorder->stopRecording();
                sizeOfTest =  test.size();
                startDots = false;
            }
            else{
                testTmp = recorder->stopRecording();
                currentPosition = test.size();
                test.insert(test.end(),testTmp.begin(),testTmp.end());
                sizeOfTest =  test.size();
            }
            isRecording=false;
            qDebug() << "recording finished";

        }
    }

}

void Player::drawObsticles(int count){

    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap(":/images/background.png"));
    background->setPos((count+1)*1000, 470);
    background->setScale(-1);
    background->setZValue(-1);
    m_scene->addItem(background);

    int num_of_obsticles = 3;
    for (int i=1; i<=num_of_obsticles; i++){
        Obstacle *o = new Obstacle(count*VIEW_WIDTH + i*100 + i*400/(num_of_obsticles-1), 0);
        m_scene->addItem(o);
    }

    m_scene->addLine(count*VIEW_WIDTH-100, 0, (count+1)*VIEW_WIDTH, 0, QPen(Qt::black, 1));

}




