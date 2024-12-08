#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include "main.h"

class FileHandling 
{
private:
    Flight* flights;
    Hotel* hotels;
    int flightCount, hotelCount;
    int maxFlights, maxHotels;

public:
    FileHandling(int = 50, int = 50);

    int getFlightCount() const;
    int getHotelCount() const;

    Flight* readFlightsFile();
    Hotel* readHotelsFile();

    Flight* getFlightByIndex(int);
    Hotel* getHotelByIndex(int);
    ~FileHandling();
};

#endif