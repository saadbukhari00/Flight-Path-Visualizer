#ifndef QUEUE_H
#define QUEUE_H

#include"main.h"

class Queue
{
private:
    struct Node
    {
        Flight flightData;
        int priority;
        Node * next;
    };
    Node * front;
    Node * rear;
public:
    Queue();
    ~Queue();
    void PriorityEnqueue(const Flight&, int);
    void enqueue(const Flight&);
    Flight dequeue();
    Flight PriorityDequeue();
    Flight Front();
    Flight Rear();
    bool isEmpty();
    void clear();
    void display();
};

#endif