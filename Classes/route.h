#ifndef ROUTE_H
#define ROUTE_H

#include "FlightGraph.h"
#include <climits>
#include <cstring>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "list.h"

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

    void listAllFlightsWithinDateRange(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate);
    void listAllFlightsWithinDataRangeandTransitCities(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, string* transit_cities, int numberOfTransitCities);
    void listAllFlightsWithinDataRangeandPreferredAirline(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, string airline);

    bool isWithinDateRange(const char* flightDate, const char* startDate, const char* endDate);
    int convertDateToComparableFormat(const char* date);
    void findFlightsWithTransitCities(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, string* transitCities, int numberOfTransitCities);
    bool findDirectAndIndirectFlights(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, LinkedList& directFlightsList, LinkedList& indirectFlightsList);

};

#endif // ROUTE_H
