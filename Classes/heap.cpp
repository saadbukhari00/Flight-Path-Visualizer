#include"heap.h"
    
    Heap::Heap() 
    {
        capacity = 10;
        currentSize = 0;
        arr = new int[capacity];
    }



    Heap::Heap(int cap) 
    {
        capacity = cap;
        currentSize = 0;
        arr = new int[capacity];
    }

   
    Heap::Heap(int *a, int size) 
    {
        capacity = size;
        currentSize = size;
        arr = new int[capacity];
        for(int i = 0; i < size; i++) 
        {
            arr[i] = a[i];
        }
        for(int i = size/2-1; i >= 0; i--) //greater than size/2-1 are leaf nodes
        {
            heapify(i);
        }
    }


    //insert element in heap
    void Heap::insert(int x) 
    {
        if(currentSize >= capacity) 
        {
            cout << "Heap is full, cannot insert more" << endl;
            return;
        }
        arr[currentSize] = x;
        currentSize++;
        int i = currentSize - 1;

        //swap insrted elemnt with parent untl its greater
        while(i > 0 && arr[i]>arr[(i-1)/2]) 
        {
            swap(arr[i], arr[(i-1)/2]);
            i = (i-1)/2;
        }
    }
    
    //heapify the heap
    void Heap::heapify(int index) 
    {
        int largest = index;
        int L = 2 * index + 1;
        int R = 2 * index + 2;

        if(L < currentSize && arr[L] > arr[largest]) 
            largest = L;
        if(R < currentSize && arr[R] > arr[largest]) 
            largest = R;

        if(largest != index) 
        {
            swap(arr[index], arr[largest]);
            heapify(largest);
        }
    }

    //remove element from heap
    void Heap::remove() 
    {
        if(currentSize == 0) 
        {
            cout << "Heap is empty, cannot remove more" << endl;
            return;
        }
        arr[0] = arr[currentSize - 1];
        currentSize--;
        heapify(0);
    }
    
    //clear hjeap
    void Heap::clear() 
    {
        currentSize = 0;
    }

    //chck if heap is empy
    bool Heap::isEmpty() 
    {
        return currentSize == 0;
    }

    //pritns the heap
    void Heap::print() 
    {
        for(int i = 0; i < currentSize; i++) 
        {
            cout << arr[i] << " ";
        }
        cout << endl;
    }