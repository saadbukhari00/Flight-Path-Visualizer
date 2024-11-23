#include"stack.h"
    
    Stack::Stack(void){top = NULL; currentElements = 0; size = 0;}
    Stack::Stack(int s)
    {
        top = NULL;
        size = s;
        currentElements = 0;
    }
        
        void Stack::Push(const Flight& flight)
        {
            if(isFull())
            {
                return;
            }
            Node * newNode = new Node;
            newNode->next = NULL;
            newNode->flightdata = flight;

            if(top == NULL)
            {
                top = newNode;
            }
            else
            {
                newNode->next = top;
                top = newNode;
            }
            currentElements++;
        }
        Flight Stack::Pop(void)
        {
            Flight popFlight;
            if(!isEmpty())
            {
                popFlight = top->flightdata;
                Node * del = new Node;
                del = top;
                top = top->next;
            
                delete del;
                currentElements--;
                return popFlight;
            }
            return popFlight;
        }

        bool Stack::isEmpty()
        {
            return (top == NULL || currentElements == 0);
        }
        bool Stack::isFull()
        {
            if(currentElements == size)
            {
                return true;
            }
            return false;
        }

        void Stack::Clear(void)
        {
            Node* curr = top;
            Node* nxt = NULL;
            while(curr)
            {
                nxt = curr->next;
                delete curr;
                curr = nxt;
            }
        }
        Flight Stack::Top()
        {
            if(!isEmpty())
                return top->flightdata;
            else
            {
                Flight emptyFlight;
                return emptyFlight;
            }

        }

        void Stack::Display()
        {
            if(isEmpty())
            {
                cout<<"\nThe stack is empty\n";
                return;
            }
            cout<<"\nThe stack is: \n";
            Node* curr = top;
            while(curr)
            {
                curr->flightdata.display();
                curr = curr->next;
            }        
        }