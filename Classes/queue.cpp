#include<queue.h>
    
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

    void Queue::Enqueue(int x, int pri)
    {
        Node * newNode = new Node;
        newNode->S = x;
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

    int Queue::Front()
    {
        return front->S;
    }

    int Queue::Rear()
    {
        return rear->S;
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
                cout<<curr->S<<" ";
                curr = curr->next;
            }
        }
        else
        {
            cout<<"\n\tThe Queue is Empty\n";
        }
    }
        