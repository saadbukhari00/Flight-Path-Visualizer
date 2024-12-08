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

    void shortestPath(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, LinkedList& directFlights, RouteList& indirectRoutes);
    void cheapestFlight(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, LinkedList& directFlights, RouteList& indirectRoutes);
    

    LinkedList listDirectFlightsWithinDateRange(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate);
    LinkedList listIndirectFlightsWithinDateRange(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate);

    LinkedList listDirectFlightsWithinDataRangeandPreferredAirline(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, string airline);
    RouteList filterMultiLegRoutesByAirline(RouteList& indirectRoutes, const string& preferredAirline);

    RouteList filterByTransitCities(RouteList& allRoutes, string* transitCities, int numberOfTransitCities);
    
    RouteList filterByTransitCitiesAndAirline(RouteList& allRoutes, const string& airline, string* transitCities, int numberOfTransitCities);

    RouteList listIndirectRoutesWithinDateRange(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate);
    void dfsBuildRoutes(int currentIndex,int destinationIndex,const char* startDate,const char* endDate,bool* visited,LinkedList& currentRoute,RouteList& allRoutes); 
    
    void removeDuplicates(RouteList &allRoutes);
    bool areRoutesIdentical(LinkedList &routeA, LinkedList &routeB);
    bool isDuplicateRoute(RouteList &allRoutes, LinkedList &candidate);
    bool isWithinDateRange(const char* flightDate, const char* startDate, const char* endDate);

    int convertDateToComparableFormat(const char* date); 
    int compareTimes(const char* time1, const char* time2);
    int calculateTravelTime(const string& departure, const string& arrival);

    int convertDateToInt(const string &dateStr);
    bool isDateInRange(const string &flightDate, const string &fromDate, const string &toDate);
    string convertDDMMYYYYToYYYYMMDD(const char* dateStr);

};

#endif // ROUTE_H
