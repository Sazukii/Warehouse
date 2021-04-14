#ifndef QUEUE_H
#define QUEUE_H

#include "Container.h"

class Queue
{
private:
    int space;
    Container* rear;
    Container* front;
public:
    Queue(int maxSize);
    Container* GetRear();
    Container* GetFront();
    int EnQueue(Container* container);
    int DeQueue();
    int CountItems();
    Container* Peek();
    ~Queue();
};

#endif