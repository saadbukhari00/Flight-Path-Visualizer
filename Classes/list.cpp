#include"list.h"

    LinkedList::LinkedList(void){head=NULL;}

    void LinkedList::insert(FlightData &x){
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

    void LinkedList::Display(void){
        if(head==NULL){
            cout<<"The list is empty\n";
            return;
        }
        FlightNode*curr=head;
        while(curr){
            cout<<"Origin: "<<curr->flight.origin<<endl;
            cout<<"Destination: "<<curr->flight.destination<<endl;
            cout<<"Date: "<<curr->flight.date<<endl;
            cout<<"Departure Time: "<<curr->flight.departureTime<<endl;
            cout<<"Arrival Time: "<<curr->flight.arrivalTime<<endl;
            cout<<"Airline: "<<curr->flight.airline<<endl;
            cout<<"Price: "<<curr->flight.price<<endl;
            cout<<"\n";
            curr=curr->next;
        }
        cout<<"\n";
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