#ifndef LINKEDLIST_H   
#define LINKEDLIST_H

#include <stddef.h>
#include <stdlib.h>
#include "Container.h"

class LinkedList
{
private:
    int maxX;
    int maxY;
    Container* firstContainer;
public:
    LinkedList();
    int MakeList(int x, int y);
    int AddContainer(Container* container, int priority);
    int EditContainer(Block block, int x, int y);
    Block SearchByIndex(int x, int y);
    Container* SearchContainer(Block block);
    Container* GetFirstContainer();
    Container* FindLastContainer();
    int CountItems();
    ~LinkedList();
};

#endif