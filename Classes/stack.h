#include"main.h"

class Stack{
private:
        struct Node{
            char character;
            Node * next;
        };
        Node * top;
        int size;
        int currentElements;
public:

    Stack(void);
    Stack(int s);

    void Push(char character);
    char Pop(void);
    char Top(void);
    void Clear(void);
    bool isEmpty(void);
    bool isFull(void);
    void Display(void);
    int Size(void);
    ~Stack(void);

};
