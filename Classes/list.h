#include<main.h>

class LinkedList
{
private:
    struct Node
    {
        int data;
        Node * next;
    };
    Node * head;
public:
    LinkedList(void);
    void insert(int x);
    void remove(int x);
    void Display(void);
    void clear(void);
};