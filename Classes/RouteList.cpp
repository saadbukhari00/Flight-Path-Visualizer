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

int RouteList::countRoutes() 
{
    int count = 0;
    RouteNode* curr = head;
    while (curr) { curr = curr->next; count++; }
    return count;
}

void RouteList::DisplayWithIndexOffset(int offset)
{
    RouteNode* curr = head;
    int index = offset;
    
    while (curr) {
        LinkedList::FlightNode* leg = curr->route.legs.getHead();
        int legCount = curr->route.legs.size();

        if (leg && legCount > 0) {
            // Print route header
            cout << "\033[1;33mRoute #" << index << ":\033[0m\n";
            cout << "\033[1;34m"; // Set color for table lines
            cout << "       ________________________________________________________________________________________________________________________________\n";
            cout << "      |  Leg  | Origin         | Destination    | Date      | Airline        | Departure Time | Arrival Time   | Price     | Distance  |\n";
            cout << "      |_______|________________|________________|___________|________________|________________|________________|___________|___________|\n";
            cout << "\033[0m"; // Reset color

            int legIndex = 0;
            LinkedList::FlightNode* temp = leg;
            while (temp) {
                // Print each flight in a formatted row
                cout << "      |\033[1;33m" << setw(6) << legIndex << "\033[0m |" 
                     << setw(15) << temp->flight.origin << " |"
                     << setw(15) << temp->flight.destination << " |"
                     << setw(10) << temp->flight.date << " |"
                     << setw(15) << temp->flight.airline << " |"
                     << setw(15) << temp->flight.departureTime << " |"
                     << setw(15) << temp->flight.arrivalTime << " |"
                     << setw(10) << temp->flight.price << " |"
                     << setw(10) << temp->flight.distance << " |\n";

                temp = temp->next;
                legIndex++;
            }

            cout << "\033[1;34m";
            cout << "      |_______|________________|________________|___________|________________|________________|________________|___________|___________|\n";
            cout << "\033[0m\n"; // Reset
        } else {
            // No flights in this route
            cout << "\033[1;33mRoute #" << index << ":\033[0m \033[1" << "No flights.\033[0m\n\n";
        }
        curr = curr->next;
        index++;
    }
}

void RouteList::Display()
{
    RouteNode* curr = head;
    int index = 0;
    
    while (curr)
    {
        LinkedList::FlightNode* leg = curr->route.legs.getHead();
        int legCount = curr->route.legs.size();

        if (leg && legCount > 0) {
            // Print route header
            cout << "\033[1;33mRoute #" << index << ":\033[0m\n";
            cout << "\033[1;34m"; // Set color for table lines
            cout << "       ________________________________________________________________________________________________________________________________\n";
            cout << "      |  Leg  | Origin         | Destination    | Date      | Airline        | Departure Time | Arrival Time   | Price     | Distance  |\n";
            cout << "      |_______|________________|________________|___________|________________|________________|________________|___________|___________|\n";
            cout << "\033[0m"; // Reset color

            int legIndex = 0;
            LinkedList::FlightNode* temp = leg;
            while (temp) {
                // Print each flight in a formatted row
                cout << "      |\033[1;33m" << setw(6) << legIndex << "\033[0m |" 
                     << setw(15) << temp->flight.origin << " |"
                     << setw(15) << temp->flight.destination << " |"
                     << setw(10) << temp->flight.date << " |"
                     << setw(15) << temp->flight.airline << " |"
                     << setw(15) << temp->flight.departureTime << " |"
                     << setw(15) << temp->flight.arrivalTime << " |"
                     << setw(10) << temp->flight.price << " |"
                     << setw(10) << temp->flight.distance << " |\n";

                temp = temp->next;
                legIndex++;
            }

            cout << "\033[1;34m";
            cout << "      |_______|________________|________________|___________|________________|________________|________________|___________|___________|\n";
            cout << "\033[0m\n"; // Reset
        } else {
            // No flights in this route
            cout << "\033[1;33mRoute #" << index << ":\033[0m \033[1;31mNo flights.\033[0m\n\n";
        }

        curr = curr->next;
        index++;
    }
}

RouteList::RouteNode* RouteList::getRouteByIndex(int idx) 
{
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


bool RouteList::isEmpty()
{
    return head == NULL;
}