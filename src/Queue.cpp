#include "Queue.h"

Queue::Queue(int maxSize)
{
    front = NULL;
    rear = NULL;
    space = maxSize;
}

Container *Queue::GetRear()
{
    return rear;
}

Container *Queue::GetFront()
{
    return front;
}

int Queue::EnQueue(Container *container)
{
    if (container == NULL || space <= 0)
    {
        return -1;
    }

    Container* newContainer = new Container(container->GetXPosition(), container->GetYPosition(), container->GetBlock());

    if (front == NULL)
    {
        front = newContainer;
        rear = newContainer;
    }
    else
    {
        rear->SetNext(newContainer);
        newContainer->SetPrevious(rear);
        rear = newContainer;
        
    }
    space--;
    return 0;
}

int Queue::DeQueue()
{
    if (front == NULL)
    {
        return -1;
    }
    Container* temp = front;
    front = front->GetNext();

    if (front == NULL)
    {
        rear = NULL;
    }
    delete (temp);
    space++;
    return 0;
}

int Queue::CountItems()
{
    int count = 0;
    Container *temp = front;
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

Container *Queue::Peek()
{
    return GetFront();
}

Queue::~Queue()
{
}