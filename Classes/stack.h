#ifndef STACK_H
#define STACK_H

#include "main.h"
#include "list.h"
#include "RouteList.h"
struct BookingState 
{
    string origin;
    string destination;
    string fromDate;
    string toDate;
    LinkedList directFlights;
    RouteList indirectRoutes;
    Flight selectedFlight;       

    BookingState() {}
    BookingState(const string& ori, const string& dest, const string& from, const string& to)
        : origin(ori), destination(dest), fromDate(from), toDate(to) {}
};
class Stack 
{
private:
private:
    struct Node {
        BookingState state;
        Node* next;

        Node(const BookingState& st, Node* nxt) : state(st), next(nxt) {}
    };

    Node* top;                       
    int size;                                             

public:
    Stack();


    void Push(const BookingState& state);
    void Pop();
    BookingState Top() const;

    bool IsEmpty() const;
    int Size() const;
    ~Stack();
};

#endif