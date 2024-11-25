#include"main.h"
class Stack{
private:
        struct Node{
            Flight flightdata;
            Node * next;
        };
        Node * top;
        int size;
        int currentElements;
public:

    Stack(void);
    Stack(int s);

    void Push(const Flight&);
    Flight Pop(void);
    Flight Top(void);
    void Clear(void);
    bool isEmpty(void);
    bool isFull(void);
    void Display(void);
    int Size(void);
    ~Stack(void);

};
