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

    Flight Queue::PriorityDequeue()
{
    if (isEmpty())
    {
        cout << "\n\tThe Queue is Empty\n\tCannot Dequeue further\n";
    }

    Node* next = front->next;
    Node* maxPriority = front;

    // Find the node with the highest priority (smallest priority value)
    while (next)
    {
        if (next->priority < maxPriority->priority)
        {
            maxPriority = next;
        }
        next = next->next;
    }

    Node* curr = front;
    Node* prev = nullptr;

    // Find the position of the maxPriority node
    while (curr->next && curr != maxPriority)
    {
        prev = curr;
        curr = curr->next;
    }

    // Store the flight data to be returned
    Flight dequeueFlight = curr->flightData;

    // Remove the maxPriority node from the queue
    if (front == maxPriority)  // If the maxPriority node is the front
    {
        front = front->next;
        if (front == nullptr) // If queue becomes empty
        {
            rear = nullptr;
        }
    }
    else if (rear == maxPriority)  // If the maxPriority node is the rear
    {
        rear = prev;
        rear->next = nullptr;
    }
    else  // If the maxPriority node is in the middle
    {
        prev->next = curr->next;
    }

    // Delete the dequeued node
    delete curr;

    return dequeueFlight;
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
        