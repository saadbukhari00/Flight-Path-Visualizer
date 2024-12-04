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

void LinkedList::Display() 
{
    if (head == NULL) 
    {
        cout << "The list is empty\n";
        return;
    }

    FlightNode* curr = head;
    while (curr) {
        cout << "\t| " << left << setw(15) << curr->flight.origin
             << " | " << left << setw(15) << curr->flight.destination
             << " | " << left << setw(10) << curr->flight.date
             << " | " << left << setw(15) << curr->flight.airline
             << " | " << left << setw(15) << curr->flight.departureTime
             << " | " << left << setw(15) << curr->flight.arrivalTime
             << " | " << left << setw(10) << curr->flight.price
             << " | " << left << setw(10) << curr->flight.distance << " |\n";
        curr = curr->next;
    }
}

void LinkedList::DisplayIndirect() 
{
    if (head == NULL) 
    {
        cout << "The list is empty\n";
        return;
    }

    FlightNode* curr = head;
    while (curr) {
        cout << "\t| " << left << setw(15) << curr->flight.origin
             << " | " << left << setw(15) << curr->flight.destination
             << " | " << left << setw(10) << curr->flight.date
             << " | " << left << setw(15) << curr->flight.airline
             << " | " << left << setw(15) << curr->flight.departureTime
             << " | " << left << setw(15) << curr->flight.arrivalTime
             << " | " << left << setw(10) << curr->flight.price
             << " | " << left << setw(10) << curr->flight.distance << " |\n";
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
