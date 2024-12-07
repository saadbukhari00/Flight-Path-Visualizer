#ifndef ROUTE_H
#define ROUTE_H

#include "FlightGraph.h"
#include "main.h"
#include "list.h"
#include "RouteList.h"
#include <iomanip>
#include <climits>

class Route 
{
private:
    FlightGraph& flightGraph;

    void initializeArrays(int* distances, int* previous, bool* visited, int size);

    int findMinDistanceIndex(int* distances, bool* visited, int size);

public:
    Route(FlightGraph& graph);

    void shortestPath(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate);
    void cheapestFlight(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate);
    void listShortestAndCheapest(const char* startCity, const char* endCity);

    LinkedList listDirectFlightsWithinDateRange(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate);
    LinkedList listIndirectFlightsWithinDateRange(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate);
    LinkedList listAllFlightsWithinDataRangeandPreferredAirline(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, string airline);
    LinkedList findFlightsWithTransitCities(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, string* transitCities, int numberOfTransitCities);
    LinkedList filterByTransitCitiesAndAirline( const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, string* transitCities, int numberOfTransitCities, const string& preferredAirline);

    bool isWithinDateRange(const char* flightDate, const char* startDate, const char* endDate);
    int convertDateToComparableFormat(const char* date); 
    bool findDirectAndIndirectFlights(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, LinkedList& indirectFlightsList);

    RouteList listIndirectRoutesWithinDateRange(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate);
    void dfsBuildRoutes(int currentIndex,int destinationIndex,const char* startDate,const char* endDate,bool* visited,LinkedList& currentRoute,RouteList& allRoutes); 
    int compareTimes(const char* time1, const char* time2);
    int calculateTravelTime(const string& departure, const string& arrival);
};

#endif // ROUTE_H
