#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include "main.h"

struct Hotel 
{
    char city[50];
    int chargePerDay;
};

class FileHandling {
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
    ~FileHandling();
};

#endif