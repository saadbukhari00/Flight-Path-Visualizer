#ifndef ROUTE_H
#define ROUTE_H

#include "FlightGraph.h"
#include <climits>
#include <cstring>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

class Route 
{
private:
    FlightGraph& flightGraph;
    sf::RenderWindow& window;
    const sf::Texture& mapTexture;

    void initializeArrays(int* distances, int* previous, bool* visited, int size);

    int findMinDistanceIndex(int* distances, bool* visited, int size);

public:
    Route(FlightGraph& graph, sf::RenderWindow& window, const sf::Texture& mapTexture);

    void findShortestRoute(const char* startCity, const char* endCity);
    void findCheapestRoute(const char* startCity, const char* endCity);

    void listShortestAndCheapest(const char* startCity, const char* endCity);

    void displayFlight(const char* originCity, const char* destinationCity);

    void findFlightsWithinRange(const char* startCity, int minRange, int maxRange);

    void displayIndirectFlights(const char* startCity, const char* endCity);  // List indirect flights
    void listAllFlightsWithinDateRange(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate);
     
    bool isWithinDateRange(const char* flightDate, const char* startDate, const char* endDate);
    int convertDateToComparableFormat(const char* date);

    void highlightShortestOrCheapest(const char* origin, const char* destination, bool shortest);
};

#endif // ROUTE_H