#include "LinkedList.h"
#include <stdexcept>
LinkedList::LinkedList()
{
    maxX = 0;
    maxY = 0;
    firstContainer = NULL;
}
int LinkedList::MakeList(int x, int y)
{
    if (x > 0 && y >> 0)
    {
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                AddContainer(new Container(j, i, empty), 0);
            }
        }
        maxX = x;
        maxY = y;
        return 0;
    }
    return -1;
}

int LinkedList::AddContainer(Container* container, int priority)
{
    if(container->GetXPosition() < 0 || container->GetYPosition() < 0)
    {
        return -1;
    }

    if(firstContainer == NULL)
    {
        firstContainer = container;
    }
    else
    {
        Container* lastContainer = FindLastContainer();
        lastContainer->SetNext(container);
        container->SetPrevious(lastContainer);
    }

    if (container->GetBlock() != empty)
    {
        container->SetPriority(priority);
    }
    return 0;
}

int LinkedList::EditContainer(Block block, int x, int y)
{
    if (x >= maxX || y >= maxY || firstContainer == NULL)
    {
        return -1;
    }

    Container *toChange = firstContainer;
    
    while(toChange->GetXPosition() != x || toChange->GetYPosition() != y)
    {
        toChange = toChange->GetNext();
    }

    if (toChange->GetBlock() == block)
    {
        return -1;
    }

    toChange->SetBlock(block);
    return 0;
}
Block LinkedList::SearchByIndex(int x, int y)
{
    if(firstContainer == NULL)
    {
        throw std::invalid_argument("This list is empty!");
    }

    Container* temp = firstContainer;

    while(temp->GetXPosition() != x || temp->GetYPosition() != y)
    {
        temp = temp->GetNext();
    }
    return temp->GetBlock();
}

Container* LinkedList::SearchContainer(Block block)
{
    if(firstContainer == NULL)
    {
        return NULL;
    }

    int priority = 65535;
    Container* temp = firstContainer;
    Container* toReturn = nullptr;

    while(temp != NULL)
    {
        if(temp->GetBlock() == block)
        {
            if(temp->GetPriority() < priority) 
            {
                priority = temp->GetPriority();
                toReturn = temp;
            }
        }
        temp = temp->GetNext();
    }
    return toReturn;
}
Container* LinkedList::GetFirstContainer()
{
    return firstContainer;
}

Container *LinkedList::FindLastContainer()
{
    if (firstContainer == NULL)
    {
        return NULL;
    }
    Container *temp = firstContainer;
    while (temp->GetNext() != NULL)
    {
        temp = temp->GetNext();
    }
    return temp;
}

int LinkedList::CountItems()
{
    int count = 0;
    Container *temp = firstContainer;
    if (temp == NULL)
    {
        return count;
    }
    else
    {
        count = 1;
        while (temp->GetNext() != NULL)
        {
            temp = temp->GetNext();
            count++;
        }
        return count;
    }
}

LinkedList::~LinkedList()
{
}