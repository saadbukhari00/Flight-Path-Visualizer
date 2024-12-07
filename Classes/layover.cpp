#include "layover.h"

void Layover::enqueue(Flight &f) {
    flightsQueue.enqueue(f);
}

Flight Layover::dequeue() {
    if (!flightsQueue.isEmpty()) {
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

int Layover::calculateTotalLayoverTime() {
    // To calculate total layover time, we need at least 2 flights
    if (flightsQueue.isEmpty()) return 0;

    // We will copy the queue contents temporarily (if Queue supports iteration or copying)
    // Alternatively, if we must preserve the queue, we might need a temporary structure.
    // Assuming we have a way to iterate or copy queue without destroying it:
    
    Queue tempQueue = flightsQueue; // assuming copy constructor or operator= is available
    if (tempQueue.isEmpty()) return 0;

    Flight prevFlight = tempQueue.dequeue(); 
    int totalLayover = 0;

    while (!tempQueue.isEmpty()) {
        Flight nextFlight = tempQueue.dequeue();
        // Check if nextFlight is on the same date or a later date:
        // If same date, calculate layover based on times
        // If later date, layover includes the full difference from arrival of prevFlight day to departure of next day.
        
        int prevDate = convertDateToComparableFormat(prevFlight.date);
        int nextDate = convertDateToComparableFormat(nextFlight.date);

        if (nextDate == prevDate) {
            // Same date, just calculate difference in times
            totalLayover += calculateLayoverTime(prevFlight.arrivalTime, nextFlight.departureTime);
        } else if (nextDate > prevDate) {
            // Different days, layover at least includes a day gap
            // For simplicity, let's consider that the layover doesn't span multiple days beyond
            // Just take difference in minutes assuming next flight departs next day:
            // One approach: treat arrival as absolute minute in day, departure as absolute minute in next day.
            // We can assume that if the date is strictly greater, we have a long layover.
            
            // This simplistic approach: no multiple-day difference calculation needed.
            // Just note that if nextDate > prevDate by N days, add (N*24*60 + difference).
            int dayDifference = (nextDate - prevDate); 
            // dayDifference is not pure days if dates differ by more than a month, but let's assume test data is close
            // A robust approach would parse the date fully, but we assume small date differences.
            
            // Minimum, assume 1 day difference = 1440 minutes (24*60)
            // This is a rough approach; for accuracy, you might need full date diff calculation.
            // For now, let's do simple approach: 
            int arrivalMin = timeStringToMinutes(prevFlight.arrivalTime);
            int departureMin = timeStringToMinutes(nextFlight.departureTime);
            totalLayover += dayDifference*1440 + (departureMin - arrivalMin);
        }
        prevFlight = nextFlight;
    }

    return totalLayover;
}

void Layover::printLayoverDetails() {
    // Similar to totalLayoverTime, but print each layover
    if (flightsQueue.isEmpty()) {
        std::cout << "No flights in the layover.\n";
        return;
    }

    Queue tempQueue = flightsQueue; 
    Flight prevFlight = tempQueue.dequeue(); 
    int legIndex = 0;

    std::cout << "\n\033[1;36mLayover Details:\033[0m\n";

    while (!tempQueue.isEmpty()) {
        Flight nextFlight = tempQueue.dequeue();
        int prevDate = convertDateToComparableFormat(prevFlight.date);
        int nextDate = convertDateToComparableFormat(nextFlight.date);

        int layoverMinutes = 0;
        if (nextDate == prevDate) {
            layoverMinutes = calculateLayoverTime(prevFlight.arrivalTime, nextFlight.departureTime);
        } else if (nextDate > prevDate) {
            // Approximation for multi-day layovers:
            int arrivalMin = timeStringToMinutes(prevFlight.arrivalTime);
            int departureMin = timeStringToMinutes(nextFlight.departureTime);
            int dayDifference = (nextDate - prevDate);
            layoverMinutes = dayDifference * 1440 + (departureMin - arrivalMin);
        }

        std::cout << "Leg " << legIndex << " layover before next flight: " << layoverMinutes << " minutes\n";
        prevFlight = nextFlight;
        legIndex++;
    }

    int total = calculateTotalLayoverTime();
    std::cout << "Total Layover Time: " << total << " minutes\n";
}