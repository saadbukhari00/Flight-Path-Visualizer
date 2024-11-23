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
    
    void displayFlights() const;
    void displayHotels() const;

    Flight* getFlightByIndex(int index) const;
    Hotel* getHotelByIndex(int index) const;

    void searchByOrigin(const char* origin) const;
    void searchByDestination(const char* destination) const;
    void searchByDate(const char* date) const;
    void searchByAirline(const char* airline) const;
    void searchByPriceRange(int minPrice, int maxPrice) const;

    void searchHotelByCity(const char* city) const;
    void searchHotelByPriceRange(int minPrice, int maxPrice) const;

    ~FileHandling();
};

#endif