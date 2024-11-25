#include "route.h"

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

        Edge* edge = flightGraph.getVertices()[currentIndex].head;
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

        Edge* edge = flightGraph.getVertices()[currentIndex].head;
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

void Route::displayFlights(const char* city) 
{
    int cityIndex = flightGraph.getCityIndex(city);

    if (cityIndex == -1) 
    {
        cerr << "Invalid city name: " << city << "\n";
        return;
    }

    cout << "Flights from " << city << ":\n";

    bool hasFlights = false;
    Edge* edge = flightGraph.getVertices()[cityIndex].head;

    while (edge) 
    {
        hasFlights = true;
        cout << "  -> Destination: " << flightGraph.getVertices()[edge->destination].city
                  << ", Price: " << edge->flightData->price << "\n";
        edge = edge->next;
    }

    if (!hasFlights) 
    {
        cout << "  No direct flights available from " << city << ".\n";
    }
}

void Route::findFlightsWithinRange(const char* startCity, int minRange, int maxRange) 
{
    int cityIndex = flightGraph.getCityIndex(startCity);

    if (cityIndex == -1) 
    {
        cerr << "Invalid city name: " << startCity << "\n";
        return;
    }

    cout << "Finding flights from " << startCity << " within the price range " << minRange << " - " << maxRange << "...\n";

    bool foundFlights = false;
    int closestAbove = INT_MAX, closestBelow = INT_MIN;

    Edge* edge = flightGraph.getVertices()[cityIndex].head;

    while (edge) 
    {
        int price = edge->flightData->price;

        if (price >= minRange && price <= maxRange) 
        {
            foundFlights = true;
            cout << "  -> Destination: " << flightGraph.getVertices()[edge->destination].city
                      << ", Price: " << price << "\n";
        }

        if (price > maxRange && price < closestAbove) 
        {
            closestAbove = price;
        }

        if (price < minRange && price > closestBelow) 
        {
            closestBelow = price;
        }

        edge = edge->next;
    }

    if (!foundFlights) 
    {
        cout << "  No flights found within the specified range.\n";

        if (closestBelow != INT_MIN)
        {
            cout << "  Closest cheaper flight: Price = " << closestBelow << "\n";
        }
        if (closestAbove != INT_MAX) 
        {
            cout << "  Closest more expensive flight: Price = " << closestAbove << "\n";
        }
    }
}
