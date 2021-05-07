#pragma once
#include "QVector2D"
class State;
class canHitWithNOSave;
class canHitWithSave;
class noHitWithNoSave;
class noHitWithSave;
class Context{
public:
    Context();
    virtual ~Context(){}

    void request(State* nextState);
    void changeState(State* state);
    State* getState(){return m_state;}
protected:
    State* m_state;
};

class State{
public:
    QVector2D& getDelta(){return *m_delta;}
    virtual void handle(Context* context, State* nextState) = 0;
protected:
    State(QVector2D* delta):m_delta(delta){}
    QVector2D* m_delta;
};

class canHitWithNOSave : public State{
public:
    canHitWithNOSave(QVector2D* delta):State(delta){}
    ~canHitWithNOSave(){}
    virtual void handle(Context* context, State* nextState);
};

class canHitWithNeedSave : public State{
public:
    canHitWithNeedSave(QVector2D* delta):State(delta){}
    ~canHitWithNeedSave(){}
    virtual void handle(Context* context, State* nextState);
};
class canHitWithSave: public State{
public:
    canHitWithSave(QVector2D* delta):State(delta){}
    ~canHitWithSave(){}
    virtual void handle(Context* context, State* nextState);
};
class noHitWithNoSave : public State{
public:
    noHitWithNoSave(QVector2D* delta):State(delta){}
    ~noHitWithNoSave(){}
    virtual void handle(Context* context, State* nextState);
};
class noHitWithNeedSave : public State{
public:
    noHitWithNeedSave(QVector2D* delta):State(delta){}
    ~noHitWithNeedSave(){}
    virtual void handle(Context* context, State* nextState);
};
class noHitWithSave : public State{
public:
    noHitWithSave(QVector2D* delta):State(delta){}
    ~noHitWithSave(){}
    virtual void handle(Context* context, State* nextState);
};
