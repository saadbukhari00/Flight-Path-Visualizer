#include<main.h>

class Queue{
private:
    struct Node{
        int S;
        int priority;
        Node * next;
    };
    Node * front;
    Node * rear;
public:
    Queue();
    ~Queue();
    void Enqueue(int, int);
    void Dequeue();
    void PriorityDequeue();
    int Front();
    int Rear();
    bool isEmpty();
    void clear();
    void display();
};