#pragma once
#include <QDialog>
#include "ball.h"
#include "game.h"
#include "networkutil.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(Game* game, QWidget *parent = 0);
    ~Dialog();

protected:
    /**
     * @brief paintEvent - called whenever window repainting is requested
     */
    void paintEvent(QPaintEvent *);
public slots:
    /**
     * @brief nextAnim - move the objects and perform collision events
     */
    void nextAnim();
    /**
     * @brief tryRender - draw the objects to screen
     */
    void tryRender();

    // qt mouse click
    void mousePressEvent(QMouseEvent* event);
    // qt mouse release
    void mouseReleaseEvent(QMouseEvent* event);
    // qt mouse move
    void mouseMoveEvent(QMouseEvent* event);

    void keyPressEvent(QKeyEvent* event);

    void reviceMessage();

private:
    /**
     * @brief evalAllEventsOfTypeSpecified - for each of the functions in the event queue
     *  invoke them if the event type is equal
     * @param t - the event type
     * @param event - the event to forward on to the function
     */
    void evalAllEventsOfTypeSpecified(MouseEventable::EVENTS t, QMouseEvent* event, Context& message);

private:
    /**
     * @brief aTimer - timer for calling nextAnim in intervals
     */
    QTimer* aTimer = nullptr;
    /**
     * @brief dTimer - timer for calling tryRender in intervals
     */
    QTimer* dTimer = nullptr;
    /**
     * @brief ui our drawable ui
     */
    Ui::Dialog *ui;
    /**
     * @brief m_game - our game object to be played
     */
    Game* m_game = nullptr;

    Caretaker* caretaker;

    NetworkUtil* network;

    Context* message;
};

