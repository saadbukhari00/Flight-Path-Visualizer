#ifndef HOTELSLIST_H
#define HOTELSLIST_H

#include "main.h"
#include "FileHandling.h"

class HotelsList
{
public:
    class HotelNode
    {
    public:
        HotelNode * next;
        Hotel hotel;

    HotelNode() { next = nullptr; }
    };

    HotelsList(FileHandling& fileHandler) : head(nullptr), fileHandler(fileHandler) {}

    void addHotel(Hotel hotel);
    void displayHotels();

    void populateHotelsList();

    void deleteHotel(const char* cityName);
    void deleteAllHotels();
    bool isEmpty() { return head == nullptr; }
    HotelNode * getHead() { return head; }
private:
    HotelNode * head;
    FileHandling& fileHandler;
};

#endif