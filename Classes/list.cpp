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
    if (head == NULL) 
    {
        cout << "The list is empty\n";
        return;
    }

    FlightNode* curr = head;
    while (curr) {
        cout << "\t";
        cout << "|" << setw(15) << curr->flight.origin << " |" 
             << setw(15) << curr->flight.destination << " |" 
             << setw(10) << curr->flight.date << " |" 
             << setw(15) << curr->flight.airline << " |" 
             << setw(15) << curr->flight.departureTime << " |" 
             << setw(15) << curr->flight.arrivalTime << " |" 
             << setw(10) << curr->flight.price << " |" 
             << setw(10) << curr->flight.distance << " |\n";
        curr = curr->next;
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
