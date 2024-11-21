#include<main.h>
#include<linkedlist.h>

	LinkedList::LinkedList(void) { head = NULL; }

	void LinkedList::insert(int x)
	{
        Node * newNode = new Node();
        newNode->data = x;
        newNode->next = NULL;

		if(head == NULL)
        {
            head = newNode;
        }
        else
        {
            Node * curr = head;
            while(curr->next)
            {
                curr = curr->next;
            }
            curr->next = newNode;
        }
	}

    void LinkedList::remove(int x)
    {
        Node * curr = head;
        Node * prev = NULL;

        while(curr && curr->data != x)
        {
            prev = curr;
            curr = curr->next;
        }

        if(curr)
        {
            if(prev)
            {
                prev->next = curr->next;
                delete curr;
            }
            else
            {
                head = curr->next;
                delete curr;
            }
        }
    }

    void LinkedList::Display(void)
	{
        if(head == NULL)
        {
            cout<<"The list is empty\n";
            return;
        }
		int currIndex = 1;
		Node* currNode = head;

		while(currNode)
		{
            if(currNode->next == NULL)
            {
                cout<<currNode->data<<" ";
            }
            else
            {
                cout<<currNode->data<<", ";
            }
			currNode = currNode->next;
		}			
	}

    void LinkedList::clear()
    {
        Node * curr = head;
        Node * next = NULL;
        while(curr)
        {
            next = curr->next;
            delete curr;
            curr = next;
        }
        head = NULL;
    }
