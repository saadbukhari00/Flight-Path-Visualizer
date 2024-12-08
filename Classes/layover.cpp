#include "layover.h"

void Layover::enqueue(Flight &f) 
{
    
    flightsQueue.enqueue(f);
}

Flight Layover::dequeue() 
{
    if (!flightsQueue.isEmpty()) 
    {
        return flightsQueue.dequeue();
    }
    // If empty, return a default Flight (handle as needed)
    Flight emptyFlight;
    strcpy(emptyFlight.origin, "");
    strcpy(emptyFlight.destination, "");
    strcpy(emptyFlight.date, "");
    strcpy(emptyFlight.departureTime, "");
    strcpy(emptyFlight.arrivalTime, "");
    emptyFlight.airline[0] = '\0';
    emptyFlight.price = 0;
    emptyFlight.distance = 0;
    return emptyFlight;
}

int Layover::calculateTotalLayoverTime() 
{
    // To calculate total layover time, we need at least 2 flights
    if (flightsQueue.isEmpty()) return 0;
    
    Queue tempQueue = flightsQueue; 
    if (tempQueue.isEmpty()) return 0;

    Flight prevFlight = tempQueue.dequeue(); 
    int totalLayover = 0;

    while (!tempQueue.isEmpty()) {
        Flight nextFlight = tempQueue.dequeue();
        // Check if nextFlight is on the same date or a later date:
        // If same date, calculate layover based on times
        // If later date, layover includes the full difference from arrival of prevFlight day to departure of next day.
        
        //darte coversion
        int prevDate = convertDateToComparableFormat(prevFlight.date);
        int nextDate = convertDateToComparableFormat(nextFlight.date);

        if (nextDate == prevDate) {
            // Same date, just calculate difference in times
            totalLayover += calculateLayoverTime(prevFlight.arrivalTime, nextFlight.departureTime);
        } 
        else if (nextDate > prevDate) 
        {
            // Different date, calculate full day difference
            int dayDifference = (nextDate - prevDate); 
            
            // Minimum, assume 1 day difference = 1440 minutes (24*60)
            //jist for demonstration purpose
            int arrivalMin = timeStringToMinutes(prevFlight.arrivalTime);
            int departureMin = timeStringToMinutes(nextFlight.departureTime);
            totalLayover += dayDifference*1440 + (departureMin - arrivalMin);
        }
        prevFlight = nextFlight;
    }

    return totalLayover;
}


// Print layover details    
void Layover::printLayoverDetails() 
{
    // Similar to totalLayoverTime, but print each layover
    if (flightsQueue.isEmpty()) 
    {
        cout << "No flights in the layover.\n";
        return;
    }

    Queue tempQueue = flightsQueue; 
    Flight prevFlight = tempQueue.dequeue(); 
    int legIndex = 0;

    cout << "\n\033[1;36mLayover Details:\033[0m\n";

    while (!tempQueue.isEmpty()) 
    {
        Flight nextFlight = tempQueue.dequeue();
        int prevDate = convertDateToComparableFormat(prevFlight.date);
        int nextDate = convertDateToComparableFormat(nextFlight.date);

        int layoverMinutes = 0;
        if (nextDate == prevDate) 
        {
            layoverMinutes = calculateLayoverTime(prevFlight.arrivalTime, nextFlight.departureTime);
        } 
        else if (nextDate > prevDate) 
        {
            // Approximation for multi-day layovers:
            int arrivalMin = timeStringToMinutes(prevFlight.arrivalTime);
            int departureMin = timeStringToMinutes(nextFlight.departureTime);
            int dayDifference = (nextDate - prevDate);
            layoverMinutes = dayDifference * 1440 + (departureMin - arrivalMin);
        }

       cout << "Leg " << legIndex << " layover before next flight: " << layoverMinutes << " minutes\n";
        prevFlight = nextFlight;
        legIndex++;
    }

    int total = calculateTotalLayoverTime();
    cout << "Total Layover Time: " << total << " minutes\n";
}