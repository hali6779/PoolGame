#ifndef MEMENTO_H
#define MEMENTO_H
#include "ball.h"
#include "balldecorator.h"

class Memento
{
public:
    Memento(std::vector<Ball*>* balls):m_balls(new std::vector<Ball*>()), cueBall(nullptr){
        for(Ball* ball: *balls){
            Ball* temp = deepcopy(ball);
            if(this->cueBall == nullptr && dynamic_cast<CueBall*>(ball) != nullptr){
                this->cueBall = new CueBall(temp);
            }
            else{
                m_balls->push_back(temp);
            }
        }
    }
    std::vector<Ball*>* getBalls(){
        return m_balls;
    }

    Ball* deepcopy(Ball* ball){
        CompositeBall* oldball = dynamic_cast<CompositeBall*>(ball);
        if(oldball){
            CompositeBall* newball = new CompositeBall(oldball->getColor(),oldball->getPosition(),oldball->getVelocity(),oldball->getMass(),oldball->getRadius(),oldball->getStrength());
            for(Ball* temp: oldball->getChildren()){
                newball->addChild(deepcopy(temp));
            }
            return newball;
        }
        else{
            StageOneBall* temp = new StageOneBall(ball->getColor(), ball->getPosition(), ball->getVelocity(), ball->getMass(), ball->getRadius());
            return temp;
        }
    }
    CueBall* getCueball(){
        return cueBall;
    }
private:
    std::vector<Ball*>* m_balls;
    CueBall* cueBall;

};

#endif // MEMENTO_H
