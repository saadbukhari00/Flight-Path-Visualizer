#include "route.h"

// Assuming Route is defined and used elsewhere as in your provided code.

Route::Route(FlightGraph& graph) : flightGraph(graph) {}

void Route::initializeArrays(int* distances, int* previous, bool* visited, int size) 
{
    for (int i = 0; i < size; i++) 
    {
        distances[i] = INT_MAX;
        previous[i] = -1;
        visited[i] = false;
    }
}

int Route::findMinDistanceIndex(int* distances, bool* visited, int size)
{
    int minDistance = INT_MAX;
    int minIndex = -1;
    for (int i = 0; i < size; i++) 
    {
        if (!visited[i] && distances[i] < minDistance) 
        {
            minDistance = distances[i];
            minIndex = i;
        }
    }
    return minIndex;
}

void Route::findShortestRoute(const char* startCity, const char* endCity) 
{
    int startIndex = flightGraph.getCityIndex(startCity);
    int endIndex = flightGraph.getCityIndex(endCity);

    if (startIndex == -1 || endIndex == -1) 
    {
        cerr << "Invalid city name(s): " << startCity << " or " << endCity << "\n";
        return;
    }

    int numCities = flightGraph.getNumVertices();
    int* distances = new int[numCities];
    int* previous = new int[numCities];
    bool* visited = new bool[numCities];

    initializeArrays(distances, previous, visited, numCities);

    distances[startIndex] = 0;

    for (int i = 0; i < numCities - 1; i++) 
    {
        int currentIndex = findMinDistanceIndex(distances, visited, numCities);
        if (currentIndex == -1) break; // No reachable unvisited nodes left

        visited[currentIndex] = true;

        // Accessing Edge with FlightGraph::Edge
        FlightGraph::Edge* edge = flightGraph.getVertices()[currentIndex].head;
        while (edge) {
            int neighbor = edge->destination;
            if (!visited[neighbor] && distances[currentIndex] + edge->flightData->distance < distances[neighbor]) {
                distances[neighbor] = distances[currentIndex] + edge->flightData->distance;
                previous[neighbor] = currentIndex;
            }
            edge = edge->next;
        }
    }

    if (distances[endIndex] == INT_MAX) 
    {
        cout << "No route available from " << startCity << " to " << endCity << ".\n";
    } 
    else 
    {
        cout << "Shortest route from " << startCity << " to " << endCity << " is " << distances[endIndex] << " km.\n";
        cout << "Route: ";
        int current = endIndex;
        while (current != -1)
          {
            cout << flightGraph.getVertices()[current].city;
            current = previous[current];
            if (current != -1) cout << " <- ";
        }
        cout << "\n";
    }

    delete[] distances;
    delete[] previous;
    delete[] visited;
}

void Route::findCheapestRoute(const char* startCity, const char* endCity) 
{
    int startIndex = flightGraph.getCityIndex(startCity);
    int endIndex = flightGraph.getCityIndex(endCity);

    if (startIndex == -1 || endIndex == -1) 
    {
        cerr << "Invalid city name(s): " << startCity << " or " << endCity << "\n";
        return;
    }

    int numCities = flightGraph.getNumVertices();
    int* costs = new int[numCities];
    int* previous = new int[numCities];
    bool* visited = new bool[numCities];

    initializeArrays(costs, previous, visited, numCities);

    costs[startIndex] = 0;

    for (int i = 0; i < numCities - 1; i++) 
    {
        int currentIndex = findMinDistanceIndex(costs, visited, numCities);
        if (currentIndex == -1) break; // No reachable unvisited nodes left

        visited[currentIndex] = true;

        // Accessing Edge with FlightGraph::Edge
        FlightGraph::Edge* edge = flightGraph.getVertices()[currentIndex].head;
        while (edge) 
        {
            int neighbor = edge->destination;
            if (!visited[neighbor] && costs[currentIndex] + edge->flightData->price < costs[neighbor]) 
            {
                costs[neighbor] = costs[currentIndex] + edge->flightData->price;
                previous[neighbor] = currentIndex;
            }
            edge = edge->next;
        }
    }

    if (costs[endIndex] == INT_MAX) 
    {
        cout << "No route available from " << startCity << " to " << endCity << ".\n";
    } 
    else 
    {
        cout << "Cheapest route from " << startCity << " to " << endCity << " costs " << costs[endIndex] << ".\n";
        cout << "Route: ";
        int current = endIndex;
        while (current != -1) 
        {
            cout << flightGraph.getVertices()[current].city;
            current = previous[current];
            if (current != -1) cout << " <- ";
        }
        cout << "\n";
    }

    delete[] costs;
    delete[] previous;
    delete[] visited;
}

void Route::listShortestAndCheapest(const char* startCity, const char* endCity)
{
    cout << "Finding shortest and cheapest routes between " << startCity << " and " << endCity << "...\n";

    findShortestRoute(startCity, endCity);
    findCheapestRoute(startCity, endCity);
}

void Route::displayIndirectFlights(const char* startCity, const char* endCity) 
{
    int startIndex = flightGraph.getCityIndex(startCity);
    int endIndex = flightGraph.getCityIndex(endCity);

    if (startIndex == -1 || endIndex == -1) 
    {
        cerr << "Invalid city name(s): " << startCity << " or " << endCity << "\n";
        return;
    }

    cout << "Indirect flights from " << startCity << " to " << endCity << ":\n";

    bool found = false;
    // Explore all neighbors of the start city
    FlightGraph::Edge* edge = flightGraph.getVertices()[startIndex].head;
    while (edge) 
    {
        int intermediateIndex = edge->destination;
        // Check if there's a direct flight from this intermediate city to the destination
        FlightGraph::Edge* intermediateEdge = flightGraph.getVertices()[intermediateIndex].head;
        while (intermediateEdge) 
        {
            if (intermediateEdge->destination == endIndex) 
            {
                found = true;
                cout << "Route: " << startCity << " -> " 
                          << flightGraph.getVertices()[intermediateIndex].city << " -> "
                          << endCity << "\n";
                cout << "Total Distance: " << (edge->flightData->distance + intermediateEdge->flightData->distance) << " km\n";
                cout << "Total Cost: " << (edge->flightData->price + intermediateEdge->flightData->price) << "\n\n";
            }
            intermediateEdge = intermediateEdge->next;
        }
        edge = edge->next;
    }

    if (!found) 
    {
        cout << "No indirect flights available from " << startCity << " to " << endCity << ".\n";
    }
}
