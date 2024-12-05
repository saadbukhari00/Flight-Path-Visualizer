#include"main.h"

class LinkedList
{
public:
    class FlightNode{
    public:
        FlightNode * next;
        Flight flight;
    public:
        FlightNode() { next = NULL; }
        FlightNode(Flight &x) { flight = x; next = NULL; }
    };
    LinkedList(void);
    FlightNode * getHead(void) { return head; }
    void insert(Flight&);
    void remove(string);
    void Display(void);
    void DisplayIndirect(void);
    void clear(void);
    Flight getHeadFlight(void) { return head->flight; }
    bool isEmpty(void) { return head == NULL; }
private:
    FlightNode * head;
};
