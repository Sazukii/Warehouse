#ifndef CONTAINER_H
#define CONTAINER_H

#include <stddef.h>
#include <stdlib.h>

typedef enum block
{
    red,
    white,
    blue,
    empty
} Block;

class Container
{
private:
    Container *previous;
    int xPosition;
    int yPosition;
    int priority;
    enum block block;
    Container *next;
public:
    Container(int xPosition, int yPosition, enum block block);
    Container* GetPrevious();
    void SetPrevious(Container* container);
    int GetXPosition();
    int GetYPosition();
    int GetPriority();
    int SetPriority(int priority);
    enum block GetBlock();
    void SetBlock(enum block block);
    Container* GetNext();
    void SetNext(Container* container);
    ~Container();
};

#endif