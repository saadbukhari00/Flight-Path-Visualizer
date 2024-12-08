#ifndef LIST_H
#define LIST_H

#include"main.h"

class LinkedList
{
public:
    class FlightNode
    {
    public:
        FlightNode * next;
        Flight flight;
    public:
        FlightNode() { next = NULL; }
        FlightNode(Flight &x) { flight = x; next = NULL; }
    };
    LinkedList(void);
    FlightNode * getHead(void) { return head; }

    int size(void);
    FlightNode * getFlightByIndex(int index);
    FlightNode * getNodeAt(int index);
    Flight * getLastFlight(void);
    void removeLast(void);
    void merge(LinkedList&);

    void insert(Flight&);
    void remove(string);

    void Display(void);
    void DisplayIndirectFlights(void);
    
    void clear(void);
    Flight getHeadFlight(void) { return head->flight; }
    bool isEmpty(void) { return head == NULL; }
private:
    FlightNode * head;
};

#endif