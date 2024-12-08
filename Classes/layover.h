#ifndef LAYOVER_H
#define LAYOVER_H

#include "main.h"
#include "queue.h"
#include "list.h"
#include <iostream>
#include <cstring>

class Layover
{
private:
    Queue flightsQueue; // Queue of Flight objects

    int timeStringToMinutes(const char* time) {
        int hours = (time[0] - '0') * 10 + (time[1] - '0'); 
        int minutes = (time[3] - '0') * 10 + (time[4] - '0'); 
        return hours * 60 + minutes;
    }

    // Convert date string to a comparable integer format
    int convertDateToComparableFormat(const char* date)
    {
        int day, month, year;
        char delimiter1, delimiter2;

        std::stringstream dateStream(date);
        dateStream >> day >> delimiter1 >> month >> delimiter2 >> year;

        // Convert date into an integer like YYYYMMDD
        return year * 10000 + month * 100 + day;
    }

public:

    // Calculate layover time between two times
    int calculateLayoverTime(const char* arrival, const char* departure) 
    {
        int arrivalMinutes = timeStringToMinutes(arrival);
        int departureMinutes = timeStringToMinutes(departure);
        return departureMinutes - arrivalMinutes;
    }

    bool isEmpty()
    {
        return flightsQueue.isEmpty();
    }    
    
    void enqueue(Flight &f);
    Flight dequeue();
    int calculateTotalLayoverTime();
    void printLayoverDetails();

};

#endif