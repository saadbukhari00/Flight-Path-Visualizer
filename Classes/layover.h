#ifndef LAYOVER_H
#define LAYOVER_H

#include "main.h"
#include "queue.h"
#include "list.h"

class Layover
{

Flight flight;
Queue flightsQueue;

public:
void enqueue(Flight&);
Flight dequeue();

int timeStringToMinutes(const char* time) {
    int hours = (time[0] - '0') * 10 + (time[1] - '0'); // First two characters for hours
    int minutes = (time[3] - '0') * 10 + (time[4] - '0'); // Last two characters for minutes
    return hours * 60 + minutes;
}

int calculateLayoverTime(const char* arrival, const char* departure) {
    int arrivalMinutes = timeStringToMinutes(arrival);
    int departureMinutes = timeStringToMinutes(departure);
    return departureMinutes - arrivalMinutes;
}


};

#endif