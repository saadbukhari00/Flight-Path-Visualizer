#include "RouteList.h"
#include "list.h"

void RouteList::insertRoute(FlightRoute&newRoute)
{
    RouteNode *newNode = new RouteNode;
    newNode->route = newRoute;
    newNode->next = NULL;
    if (head == NULL)
    {
        head = newNode;
    }
    else
    {
        RouteNode *curr = head;
        while (curr->next)
        {
            curr = curr->next;
        }
        curr->next = newNode;
    }
}

int RouteList::countRoutes() {
    int count = 0;
    RouteNode* curr = head;
    while (curr) { curr = curr->next; count++; }
    return count;
}

RouteList::RouteNode* RouteList::getHead()
{
    return head;
}


void RouteList::displayRoutes() {
    // Assume you have a function countRoutes() and a way to get RouteNode by index
    RouteNode* curr = head;
    int index = 0;
    while (curr) {
        // Summarize route: print origin, intermediate stops, final destination
        // We'll extract it from curr->route.legs
        LinkedList::FlightNode* leg = curr->route.legs.getHead();
        if (leg) {
            cout << "Route #" << index << ": ";
            // Print a chain like: Origin -> ... -> Final Destination
            // Gather city names
            cout << leg->flight.origin;
            LinkedList::FlightNode* temp = leg;
            while (temp) {
                cout << " -> " << temp->flight.destination;
                temp = temp->next;
            }
            cout << " (" << curr->route.legs.size() << " legs)" << "\n";
        } else {
            cout << "Route #" << index << ": No flights.\n";
        }
        curr = curr->next;
        index++;
    }
}

RouteList::RouteNode* RouteList::getRouteByIndex(int idx) {
    int count = 0;
    RouteNode* curr = head;
    while (curr && count < idx) {
        curr = curr->next;
        count++;
    }
    return curr;
}

void RouteList::clear()
{
    RouteNode *curr = head, *next = NULL;
    while (curr)
    {
        next = curr->next;
        delete curr;
        curr = next;
    }
    head = NULL;
}
