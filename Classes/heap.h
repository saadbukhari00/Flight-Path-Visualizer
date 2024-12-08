#include"main.h"

class Heap
{
private:
    int *arr;
    int capacity;
    int currentSize;
public:
    Heap();
    Heap(int cap);
    Heap(int *a, int size);
    void insert(int x);
    void remove();
    void heapify(int index);
    void print();
    bool isEmpty();
    void clear();
    ~Heap();
};