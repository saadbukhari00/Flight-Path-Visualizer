#include "layover.h"



void Layover::enqueue(Flight& flight)
{
    int layovertime = calculateLayoverTime(flight.arrivalTime,flight.departureTime);

     flightsQueue.PriorityEnqueue(flight,layovertime);
}

Flight Layover::dequeue()
{
    return flightsQueue.PriorityDequeue();
}