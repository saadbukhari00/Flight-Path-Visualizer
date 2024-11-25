#ifndef ROUTE_H
#define ROUTE_H

#include "FlightGraph.h"
#include <climits>
#include <cstring>
#include <iostream>
#include <ctime>

class Route 
{
private:
    FlightGraph& flightGraph;

    void initializeArrays(int* distances, int* previous, bool* visited, int size);

    int findMinDistanceIndex(int* distances, bool* visited, int size);

public:
    Route(FlightGraph& graph);

    void findShortestRoute(const char* startCity, const char* endCity);

    void findCheapestRoute(const char* startCity, const char* endCity);

    void listShortestAndCheapest(const char* startCity, const char* endCity);

    void displayFlights(const char* city);

    void findFlightsWithinRange(const char* startCity, int minRange, int maxRange);

     void displayIndirectFlights(const char* startCity, const char* endCity);  // List indirect flights
     void listFlightsWithinDateRange(const char* startDate, const char* endDate); // Flights in date range
};

#endif // ROUTE_H
