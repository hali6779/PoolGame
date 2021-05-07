#include "dialog.h"
#include "ui_dialog.h"
#include <QPainter>
#include <QTimer>
#include <iostream>
#include <QMouseEvent>
#include "utils.h"
#include "QHostAddress"
#include "iostream"
#include "QVector2D"
Dialog::Dialog(Game *game, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_game(game),
    caretaker(new Caretaker()),
    message(new Context()),
    network(new NetworkUtil(QHostAddress::LocalHost,QHostAddress::LocalHost))
{
    ui->setupUi(this);

    // for animating (i.e. movement, collision) every animFrameMS
    aTimer = new QTimer(this);
    connect(aTimer, SIGNAL(timeout()), this, SLOT(nextAnim()));
    connect(aTimer, SIGNAL(timeout()), this, SLOT(reviceMessage()));
    aTimer->start(animFrameMS);

    // for drawing every drawFrameMS milliseconds
    dTimer = new QTimer(this);
    connect(dTimer, SIGNAL(timeout()), this, SLOT(tryRender()));
    dTimer->start(drawFrameMS);


    // set the window size to be at least the table size
    this->resize(game->getMinimumWidth(), game->getMinimumHeight());
}

Dialog::~Dialog()
{
    delete aTimer;
    delete dTimer;
    delete m_game;
    delete ui;
}

void Dialog::tryRender() {
    this->update();
}

void Dialog::nextAnim() {
    m_game->animate(1.0/(double)animFrameMS);
}

void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    m_game->render(painter);
}

void Dialog::mousePressEvent(QMouseEvent* event) {
    evalAllEventsOfTypeSpecified(MouseEventable::EVENTS::MouseClickFn, event, *message);
}

void Dialog::reviceMessage(){
    QVector2D delta;
    bool recover = false;
    if (network->receiveMessage(delta, recover)){
            std::cout << "recived: " <<std::to_string(delta.x()) + " " + std::to_string(delta.y()) << std::endl;
            if(recover){
                Memento* temp = caretaker->pop();
                if(temp){
                    m_game->loadMemento(temp);
                }
            } else if(dynamic_cast<noHitWithNeedSave*>(message->getState()) != nullptr || dynamic_cast<canHitWithNOSave*>(message->getState()) != nullptr){
                message->request(new noHitWithSave(&delta));
                caretaker->add(m_game->saveMemento());
                evalAllEventsOfTypeSpecified(MouseEventable::EVENTS::MouseRelFn, nullptr, *message);
            }
    }

}
void Dialog::mouseReleaseEvent(QMouseEvent* event) {
    evalAllEventsOfTypeSpecified(MouseEventable::EVENTS::MouseRelFn, event, *message);
    if (dynamic_cast<noHitWithNoSave*>(message->getState()) != nullptr){
        network->sendCommand(&(message->getState()->getDelta()), false);
        message->request(new noHitWithNeedSave(nullptr));
    }
}
void Dialog::mouseMoveEvent(QMouseEvent* event) {
    evalAllEventsOfTypeSpecified(MouseEventable::EVENTS::MouseMoveFn, event, *message);
    if (dynamic_cast<canHitWithNeedSave*>(message->getState()) != nullptr){
        caretaker->add(m_game->saveMemento());
        message->request(new canHitWithSave(nullptr));
    }
}

void Dialog::keyPressEvent(QKeyEvent* event){
    if (event->key() == Qt::Key_R){
        Memento* temp = caretaker->pop();
        if(temp){
            m_game->loadMemento(temp);
            network->sendCommand(nullptr, true);
        }
    }
}
void Dialog::evalAllEventsOfTypeSpecified(MouseEventable::EVENTS t, QMouseEvent *event, Context& message) {
    // handle all the clicky events, and remove them if they've xPIRED
    MouseEventable::EventQueue& Qu = m_game->getEventFns();
    for (ssize_t i = Qu.size()-1; i >= 0; i--) {
        if (auto spt = (Qu.at(i)).lock()) {
            if (spt->second == t) {
                spt->first(event, message);
            }
        } else {
            // remove this element from our vector
            Qu.erase(Qu.begin() + i);
        }
    }
}
