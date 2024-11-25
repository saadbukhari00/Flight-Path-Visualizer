#ifndef ROUTE_H
#define ROUTE_H

#include "FlightGraph.h"
#include <climits>
#include <cstring>
#include <iostream>

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
};

#endif // ROUTE_H
