#ifndef HOTELBOOKING_H
#define HOTELBOOKING_H

#include "HotelsList.h"
#include <iostream>
#include <cstring>
#include <iomanip>

class HotelBooking
{
private:
    HotelsList &hotelsList; // Reference to a hotels list populated externally

public:
    HotelBooking(HotelsList &hList);
    void searchHotelsInCity(const char* cityName);
    void bookHotelInCity(const char* cityName);
};

#endif