#pragma once

#include <QMouseEvent>
#include <context.h>

/**
 * @brief The MouseEventable class
 *  go away
 *
 * Inherit this class & override the mouse___Event(QMouseEvent) if you want those functions to fire
 * Make sure you pass the vector from getEvents to m_game's EventQueue
 */
class MouseEventable {
private:
    // close your eyes, there is blood lining the floors of this devil's supermarket
    // the voice speaker keeps asking more employees to clean up aisle 5
    // but the ****void&&& just consumes more victims

    /**
     * this is part of the magic for detection of overridden methods.
     * this will throw an exception if the type HAS been overridden
     *      (which gets caught and special things ȟ͗͆á͈̖̞̐̋̀̋̈́̊̍͝p̊̃̉͂p̓̇̋̀͗ę̺̬͕̰͖͓́̃͗̅̔̓̌ň̆̀̚͡)
    */
    struct OverrideTest {
        operator QMouseEvent*() const{throw true;}
    };
    Context test;
    virtual void mouseClickEvent(OverrideTest, Context&) {}
    virtual void mouseMoveEvent(OverrideTest, Context&) {}
    virtual void mouseReleaseEvent(OverrideTest, Context&) {}

    virtual void mouseClickEvent(QMouseEvent*, Context& message) {}
    virtual void mouseMoveEvent(QMouseEvent*, Context& message) {}
    virtual void mouseReleaseEvent(QMouseEvent*, Context& message) {}

public:
    typedef std::function<void(QMouseEvent*, Context&)> MouseFn;
    // store all the functions that can be called on event happenings (i.e. onMouseClick, etc etc)
    enum EVENTS { MouseClickFn, MouseRelFn, MouseMoveFn };
    typedef std::pair<MouseEventable::MouseFn, MouseEventable::EVENTS> EventHook;
    typedef std::vector<std::weak_ptr<EventHook>> EventQueue;

    /**
     * @brief getEvents
     * @return all event functions that can be invoked
     */
    virtual EventQueue getEvents() {
        MouseEventable::EventQueue retV;
        for(size_t i = 0; i < m_ownedFns.size();++i) {
            retV.push_back(m_ownedFns[i]);
        }
        return retV;
    }
protected:
    std::vector<std::shared_ptr<EventHook>> m_ownedFns;

protected:
    /**
     * This ctor takes a MouseEventable subclass, so that we can register the functions that may or may not have been overridden
     *  We perform some magic, just to make it add the functions that have been overridden
     *  as the default functions are empty, and shouldn't be seen
     * @param child - a pointer to a type that MUST inherit MouseEventable
     */
    template <typename T>
    MouseEventable(T child):test(Context()){
        // enforce that we can only construct with MouseEventable types - this will probably throw an error including this line somewhere
        Q_UNUSED(static_cast<MouseEventable*>(child));

        try{child->mouseClickEvent(OverrideTest(), this->test);}
        catch(bool){
            m_ownedFns.push_back(std::shared_ptr<EventHook>(new EventHook(
                std::function<void(QMouseEvent*, Context&)>([&](QMouseEvent*e, Context& message) {this->mouseClickEvent(e, message);}), MouseClickFn)));
        }

        try{child->mouseMoveEvent(OverrideTest(), this->test);}
        catch(bool){
            m_ownedFns.push_back(std::shared_ptr<EventHook>(new EventHook(
                std::function<void(QMouseEvent*, Context&)>([&](QMouseEvent*e, Context& message) {this->mouseMoveEvent(e, message);}), MouseMoveFn)));
        }

        try{child->mouseReleaseEvent(OverrideTest(), this->test);}
        catch(bool){
            m_ownedFns.push_back(std::shared_ptr<EventHook>(new EventHook(
                std::function<void(QMouseEvent*, Context&)>([&](QMouseEvent*e, Context& message) {this->mouseReleaseEvent(e, message);}), MouseRelFn)));
        }
    }

};
