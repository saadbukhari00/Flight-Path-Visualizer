#include"main.h"
#include"FileHandling.h"

class Queue{
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
    void Dequeue();
    void PriorityDequeue();
    Flight Front();
    Flight Rear();
    bool isEmpty();
    void clear();
    void display();
};