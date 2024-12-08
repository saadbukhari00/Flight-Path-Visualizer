#ifndef ROUTELIST_H
#define ROUTELIST_H

#include "main.h"
#include "list.h"


struct FlightRoute 
{
    LinkedList legs;
};

class RouteList 
{
public:
    struct RouteNode 
    {
        FlightRoute route;
        RouteNode* next;
        bool cheapest;
        bool shortest;
        RouteNode()
        {
            cheapest = false;
            shortest = false;
            next = NULL;
        }
    };
    
    RouteList() : head(NULL) {}
    bool isEmpty();
    void insertRoute(FlightRoute& newRoute);

    void DisplayWithIndexOffset(int offset);
    void Display();
    void clear();
    int countRoutes();
    RouteNode* getRouteByIndex(int idx);
    RouteNode* getHead() { return head; }
    bool hasRoutes() { return head != NULL; }
private:
    RouteNode* head;
};

#endif