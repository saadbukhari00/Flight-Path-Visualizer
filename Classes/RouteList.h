#ifndef ROUTELIST_H
#define ROUTELIST_H

#include "main.h"
#include "list.h"


struct FlightRoute {
    LinkedList legs;
};

class RouteList {
public:
    struct RouteNode {
        FlightRoute route;
        RouteNode* next;
    };
    
    RouteList() : head(NULL) {}

    void insertRoute(FlightRoute& newRoute);
    void displayRoutes();
    void clear();
    int countRoutes();
    RouteNode* getRouteByIndex(int idx);
    bool hasRoutes() { return head != NULL; }
private:
    RouteNode* head;
};

#endif