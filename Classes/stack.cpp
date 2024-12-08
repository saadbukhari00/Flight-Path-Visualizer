#include "stack.h"

Stack::Stack() : top(nullptr), size(0) {}

void Stack::Push(const BookingState& state) 
{
    top = new Node(state, top);
    size++;
}

void Stack::Pop() 
{
    if(IsEmpty()) 
    {
        return;
    }

    Node* temp = top;
    top = top->next;
    delete temp;
    size--;
}

BookingState Stack::Top() const 
{
    if(IsEmpty()) 
    {
        throw out_of_range("Stack is empty");
    }

    return top->state;
}

bool Stack::IsEmpty() const { return size == 0; }
int Stack::Size() const { return size; }

Stack::~Stack() {
    while (!IsEmpty()) {
        Pop();
    }
}