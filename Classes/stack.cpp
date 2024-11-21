#include"stack.h"
    
    Stack::Stack(void){top = NULL; currentElements = 0; size = 0;}
    Stack::Stack(int s)
    {
        top = NULL;
        size = s;
        currentElements = 0;
    }
        
        void Stack::Push(char character)
        {
            if(isFull())
            {
                return;
            }
            Node * newNode = new Node;
            newNode->next = NULL;
            newNode->character = character;

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
        char Stack::Pop(void)
        {
            if(!isEmpty())
            {
                char tp = top->character;
                Node * del = new Node;
                del = top;
                top = top->next;
            
                delete del;
                currentElements--;
                return tp;
            }
            return '\0';
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
        char Stack::Top()
        {
            if(!isEmpty())
                return top->character;
            else
                return '\0';
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
                cout<<curr->character<<" ";
                curr = curr->next;
            }        
        }