#include "Container.h"

Container::Container(int xPosition, int yPosition, enum block block)
{
    this->xPosition = xPosition;
    this->yPosition = yPosition;
    this->block = block;
    next = NULL;
    previous = NULL;
    priority = 0;
}

Container* Container::GetPrevious()
{
    return previous;
}

void Container::SetPrevious(Container* container)
{
    previous = container;
}

int Container::GetXPosition()
{
    return xPosition;
}

int Container::GetYPosition()
{
    return yPosition;
}

int Container::GetPriority()
{
    return priority;
}

int Container::SetPriority(int priority)
{
    if(priority < 0)
    {
        return -1;
    }
    this->priority = priority;
    return 0;
}

enum block Container::GetBlock()
{
    return block;
}

void Container::SetBlock(enum block block)
{
    this->block = block;
}

Container* Container::GetNext()
{
    return next;
}

void Container::SetNext(Container* container)
{
    next = container;
}

Container::~Container()
{

}