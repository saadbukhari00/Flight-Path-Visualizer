#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include <climits>
#include "filehandling.h"
#include "heap.h"
#include "queue.h"
#include "stack.h"

class Route {
private:
    struct Node {
        int cityIndex; // Index of the city
        int cost;      // Cost to reach this city
        int time;      // Time to reach this city

        Node(int cityIndex, int cost, int time)
            : cityIndex(cityIndex), cost(cost), time(time) {}
    };

    int findCityIndex(const  string& cityName,  string cities[], int cityCount);
    void initializeGraph(int graph[100][100], int size);

public:
    void findShortestRoute(const  string& origin, const  string& destination, int cityCount,  string cities[], int graph[100][100]);
    void findCheapestRoute(const  string& origin, const  string& destination, int cityCount,  string cities[], int graph[100][100]);
    void listFlightsInRange(int minCost, int maxCost, Flight flights[], int flightCount);
    void displayDirectFlights(const  string& origin, const  string& destination, Flight flights[], int flightCount);
    void displayIndirectFlights(const  string& origin, const  string& destination, Flight flights[], int flightCount);
};

#endif
