#include"list.h"
#include <iomanip>

    LinkedList::LinkedList(void){head=NULL;}

    void LinkedList::insert(Flight &x){
        FlightNode *newNode=new FlightNode(x);
        newNode->next=NULL;
        if(head==NULL){
            head=newNode;
        }else{
            FlightNode *curr=head;
            while(curr->next){
                curr=curr->next;
            }
            curr->next=newNode;
        }
    }

    void LinkedList::remove(string airL){
        FlightNode *curr=head,*prev=NULL;
        while(curr&&curr->flight.airline != airL){
            prev=curr;
            curr=curr->next;
        }
        if(curr){
            if(prev){
                prev->next=curr->next;
                delete curr;
            }else{
                head=curr->next;
                delete curr;
            }
        }
    }

int LinkedList::size() 
{
    int count = 0;
    FlightNode* curr = head;
    while (curr) {
        count++;
        curr = curr->next;
    }
    return count;
}

LinkedList::FlightNode* LinkedList::getFlightByIndex(int index) 
{
    FlightNode* curr = head;
    int count = 0;
    while (curr) {
        if (count == index) {
            return curr;
        }
        count++;
        curr = curr->next;
    }
    return NULL;
}

LinkedList::FlightNode* LinkedList::getNodeAt(int index) 
{
    return getFlightByIndex(index);
}

Flight* LinkedList::getLastFlight() {
    if (!head) return NULL;
    FlightNode* curr = head;
    while (curr->next) {
        curr = curr->next;
    }
    return &(curr->flight);
}

void LinkedList::removeLast() {
    if (!head) return;
    if (!head->next) {
        delete head;
        head = NULL;
        return;
    }
    FlightNode* curr = head;
    while (curr->next && curr->next->next) {
        curr = curr->next;
    }
    delete curr->next;
    curr->next = NULL;
}

void LinkedList::merge(LinkedList& list) 
{
    FlightNode* curr = list.getHead();
    while (curr) {
        insert(curr->flight);
        curr = curr->next;
    }
}

void LinkedList::Display() 
{
    // Table header with colors
    cout << "\033[1;34m";
    cout << "       ________________________________________________________________________________________________________________________________\n";
    cout << "      |  IDX  | Origin         | Destination    | Date      | Airline        | Departure Time | Arrival Time   | Price     | Distance  |\n";
    cout << "      |_______|________________|________________|___________|________________|________________|________________|___________|___________|\n";
    cout << "\033[0m";

    FlightNode* curr = head;
    int index = 0;
    while (curr) 
    {
        // Color each row alternatingly (optional)
        // For simplicity, all rows same color here
        cout << "      |\033[1;33m" << setw(6) << index << "\033[0m |" 
             << setw(15) << curr->flight.origin << " |" 
             << setw(15) << curr->flight.destination << " |" 
             << setw(10) << curr->flight.date << " |" 
             << setw(15) << curr->flight.airline << " |" 
             << setw(15) << curr->flight.departureTime << " |" 
             << setw(15) << curr->flight.arrivalTime << " |" 
             << setw(10) << curr->flight.price << " |" 
             << setw(10) << curr->flight.distance << " |\n";
        curr = curr->next;
        index++;
    }

    cout << "\033[1;34m";
    cout << "      |_______|________________|________________|___________|________________|________________|________________|___________|___________|\n";
    cout << "\033[0m";
}

void LinkedList::DisplayIndirectFlights()
{
    if (head == NULL)
    {
        cout << "No indirect flights available.\n";
        return;
    }

    FlightNode* curr = head;
    int index = 0;
    cout << "\nIndirect Flights:\n";
    while (curr) 
    {
        cout << "Flight #" << index << ":\n";
        cout << curr->flight.origin << " -> " << curr->flight.destination << "\n";
        cout << "Date: " << curr->flight.date << ", Airline: " << curr->flight.airline
             << ", Departure: " << curr->flight.departureTime 
             << ", Arrival: " << curr->flight.arrivalTime 
             << ", Price: " << curr->flight.price 
             << ", Distance: " << curr->flight.distance << "\n\n";
        curr = curr->next;
        index++;
    }
}

    void LinkedList::clear(){
        FlightNode *curr=head,*next=NULL;
        while(curr){
            next=curr->next;
            delete curr;
            curr=next;
        }
        head=NULL;
    }
