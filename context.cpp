#include "context.h"

Context::Context():m_state(new canHitWithNOSave(nullptr)){}

void Context::request(State* nextState){
    this->m_state->handle(this, nextState);
}

void Context::changeState(State *state){
    this->m_state = state;
}

void canHitWithNOSave::handle(Context *context, State* nextState){
    context->changeState(nextState);
}

void canHitWithNeedSave::handle(Context *context, State *nextState){
    context->changeState(nextState);
}
void canHitWithSave::handle(Context *context, State* nextState){
    context->changeState(nextState);
}

void noHitWithNoSave::handle(Context *context, State *nextState){
    context->changeState(nextState);
}

void noHitWithNeedSave::handle(Context *context, State *nextState){
    context->changeState(nextState);
}

void noHitWithSave::handle(Context *context, State *nextState){
    context->changeState(nextState);
}
