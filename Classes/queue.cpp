#include"queue.h"
    
    Queue::Queue() 
    {
        front = NULL;
        rear = NULL;
    }

    Queue::~Queue()
    {
        Node * curr = front;
        Node * next = NULL;

        while(curr)
        {
            next = curr->next;
            delete curr;
            curr = next;
        }
    }

    void Queue::enqueue(const Flight& flight)
    {
        Node * newNode = new Node;
        newNode->flightData = flight;
        newNode->priority = 0;
        newNode->next = NULL;

        if(front == NULL && rear == NULL)
        {
            //jab queue empty ho
            front = newNode;
            rear = newNode;
        }
        else
        {
            //add at the end
            rear->next = newNode;
            rear = newNode;
        }   
    }

    void Queue::PriorityEnqueue(const Flight& flight, int pri)
    {
        Node * newNode = new Node;
        newNode->flightData = flight;
        newNode->priority = pri;
        newNode->next = NULL;

        if(front == NULL && rear == NULL)
        {
            //jab queue empty ho
            front = newNode;
            rear = newNode;
        }
        else
        {
            //add at the end
            rear->next = newNode;
            rear = newNode;
        }

    }

    void Queue::Dequeue()
    {
        if(isEmpty())
        {
            cout<<"\n\tThe Queue is Empty\n\tCannot Dequeue further\n";
        }
        else
        {
            Node * deletingNode = front;
            front = front->next;
            delete deletingNode;

            if(front == NULL)
            {
                rear = NULL;
            }
        }
    }

    void Queue::PriorityDequeue()
    {
        if(isEmpty())
        {
            cout<<"\n\tThe Queue is Empty\n\tCannot Dequeue further\n";
        }
        else
        {
            Node * next = front->next;

            Node * maxPriority = front;

            while(next)
            {
                if(next->priority < maxPriority->priority)
                {
                    maxPriority = next;
                }
                next = next->next;
            }

            Node * curr = front;
            Node * prev = NULL;

            while(curr->next && curr != maxPriority)
            {
                prev = curr;
                curr = curr->next;
            }

            if(rear == maxPriority)
            {
                rear = prev;
                if(rear == NULL)
                {
                    front = NULL;
                    return;
                }
                delete curr;
                rear->next = NULL;
                
            }
            else if(front == maxPriority)
            {
                front = front->next;
                if(front == NULL)
                {
                    rear = NULL;
                }
                delete curr;
            }
            else
            {
                prev->next = curr->next;
                delete curr;
            }
        }
    }

    Flight Queue::Front()
    {
        return front->flightData;
    }

    Flight Queue::Rear()
    {
        return rear->flightData;
    }

    bool Queue::isEmpty()
    {
        if(front == NULL && rear == NULL)
        {
            return true;
        }
        return false;
    }

    void Queue::clear()
    {
        Node * curr = front;
        Node * next = NULL;

        while(curr)
        {
            next = curr->next;
            delete curr;
            curr = next;
        }
    }

    void Queue::display()
    {
        if(!isEmpty())
        {
            Node* curr = front;
            while(curr)
            {
                curr->flightData.display();
                curr = curr->next;
            }
        }
        else
        {
            cout<<"\n\tThe Queue is Empty\n";
        }
    }
        