#ifndef CARETAKER_H
#define CARETAKER_H
#include <memento.h>
#include <qlist.h>

class Caretaker
{
public:
    Caretaker():mementolist(QList<Memento*>()){}
    void add(Memento* memento){mementolist.append(memento);}
    Memento* pop(){
        if(!mementolist.isEmpty()){
            return mementolist.takeLast();
        }
        else{
            return nullptr;
        }
    }
private:
    QList<Memento*> mementolist;
};

#endif // CARETAKER_H
