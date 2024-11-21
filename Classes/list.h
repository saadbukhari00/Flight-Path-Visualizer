#include"main.h"
    struct FlightData
    {
        char origin[50], destination[50], date[20], departureTime[10], arrivalTime[10], airline[50];
        int price;
    };
class LinkedList
{
private:
    class FlightNode{
    public:
        FlightNode * next;
        FlightData flight;
    public:
        FlightNode() { next = NULL; }
        FlightNode(FlightData &x) { flight = x; next = NULL; }
    };
    FlightNode * head;
public:
    LinkedList(void);
    void insert(FlightData&);
    void remove(string);
    void Display(void);
    void clear(void);
};