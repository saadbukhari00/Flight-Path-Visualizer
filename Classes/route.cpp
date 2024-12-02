#include "route.h"
#include "menu.h"
// Assuming Route is defined and used elsewhere as in your provided code.

Route::Route(FlightGraph& graph)
    : flightGraph(graph) {}

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
    Menu menu;
    menu.displaySCheader(startCity, endCity);

    int startIndex = flightGraph.getCityIndex(startCity);
    int endIndex = flightGraph.getCityIndex(endCity);

    if (startIndex == -1 || endIndex == -1) 
    {
        cout << "\033[1;31mInvalid city name(s): " << startCity << " or " << endCity << "\033[0m\n";
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
        cout << "\033[1;31mNo route available from " << startCity << " to " << endCity << ".\033[0m\n";
    } 
    else 
    {
        int current = endIndex;
        while (current != -1) 
        {
            Edge* edge = flightGraph.getVertices()[current].head;
            while (edge) 
            {
                if (previous[current] == edge->destination) 
                {
                    cout << "\t";
                    cout << "|" << std::setw(6) << edge->flightData->date << " |" 
                    << setw(8) << edge->flightData->airline << " |" 
                    << setw(14) << edge->flightData->departureTime << " |" 
                    << setw(12) << edge->flightData->arrivalTime << " |" 
                    << setw(6) << edge->flightData->price << " |" 
                    << setw(9) << edge->flightData->distance << " |\n";
                        break;
                }
                edge = edge->next;
            }
            current = previous[current];
        }

        cout << "\t|__________|_________|_______________|_____________|_______|__________|\033[0m\n";
    }

    delete[] distances;
    delete[] previous;
    delete[] visited;
}
void Route::findCheapestRoute(const char* startCity, const char* endCity) 
{
    Menu menu;
    menu.displaySCheader(startCity, endCity);

    int startIndex = flightGraph.getCityIndex(startCity);
    int endIndex = flightGraph.getCityIndex(endCity);

    if (startIndex == -1 || endIndex == -1) 
    {
        cout << "\033[1;31mInvalid city name(s): " << startCity << " or " << endCity << "\033[0m\n";
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
        cout << "\033[1;31mNo route available from " << startCity << " to " << endCity << ".\033[0m\n";
    } 
    else 
    {

        int current = endIndex;
        while (current != -1) 
        {
            Edge* edge = flightGraph.getVertices()[current].head;
            while (edge) 
            {
                if (previous[current] == edge->destination) 
                {
                    cout << "\t";
                    cout << "|" << std::setw(6) << edge->flightData->date << " |" 
                    << setw(8) << edge->flightData->airline << " |" 
                    << setw(14) << edge->flightData->departureTime << " |" 
                    << setw(12) << edge->flightData->arrivalTime << " |" 
                    << setw(6) << edge->flightData->price << " |" 
                    << setw(9) << edge->flightData->distance << " |\n";
                    break;
                }
                edge = edge->next;
            }
            current = previous[current];
        }

        cout << "\t|__________|_________|_______________|_____________|_______|__________|\033[0m\n";
    }

    delete[] costs;
    delete[] previous;
    delete[] visited;
}
void Route::listShortestAndCheapest(const char* startCity, const char* endCity)
{
    Menu menu;
    
    findShortestRoute(startCity, endCity);

    findCheapestRoute(startCity, endCity);
}

void Route::listAllFlightsWithinDateRange(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate)
{
    int originIndex = flightGraph.getCityIndex(originCity);
    int destinationIndex = flightGraph.getCityIndex(destinationCity);

    Menu menu;

    if(originIndex == -1 || destinationIndex == -1) 
    {
        cout << "\033[1;31mInvalid city name(s): " << originCity << " or " << destinationCity << "\033[0m\n";
        return;
    }

    bool foundFlights = false;

    menu.displayFlightHeader(originCity, destinationCity);
    // Check direct flights
    Edge* edge = flightGraph.getVertices()[originIndex].head;

    while (edge) 
    { 
        if(edge->destination == destinationIndex && isWithinDateRange(edge->flightData->date, startDate, endDate)) 
        {
            foundFlights = true;
            cout << "\t";
            cout << "|" << std::setw(6) << edge->flightData->date << " |" 
                 << setw(8) << edge->flightData->airline << " |" 
                 << setw(14) << edge->flightData->departureTime << " |" 
                 << setw(12) << edge->flightData->arrivalTime << " |" 
                 << setw(6) << edge->flightData->price << " |" 
                 << setw(9) << edge->flightData->distance << " |\n";
        }
        edge = edge->next;
    }
    cout << "\t|__________|_________|_______________|_____________|_______|__________|\033[0m\n";

    menu.displayIndFlightHeader(originCity, destinationCity);
    // Check indirect flights
    edge = flightGraph.getVertices()[originIndex].head;

    while (edge) 
    {
        int intermediateIndex = edge->destination;
        Edge* intermediateEdge = flightGraph.getVertices()[intermediateIndex].head;

        while (intermediateEdge) 
        {
            if (intermediateEdge->destination == destinationIndex && isWithinDateRange(edge->flightData->date, startDate, endDate) && isWithinDateRange(intermediateEdge->flightData->date, startDate, endDate)) 
            {
                foundFlights = true;
                cout << setw(6) << edge->flightData->date << " |" 
                     << setw(6) << intermediateEdge->flightData->date << " |" 
                     << setw(8) << edge->flightData->airline << " |" 
                     << setw(14) << edge->flightData->departureTime << " |" 
                     << setw(12) << intermediateEdge->flightData->arrivalTime << " |" 
                     << setw(6) << edge->flightData->price + intermediateEdge->flightData->price << " |" 
                     << setw(9) << edge->flightData->distance + intermediateEdge->flightData->distance << " |\n";
            }
            intermediateEdge = intermediateEdge->next;
        }
        edge = edge->next;
    }

    cout << "\t|________________|_________________|_________|________________|______________|_______|__________|\033[0m\n";

    if (!foundFlights) 
    {
        cout << "\t\033[1;31mNo flights available from " << originCity << " to " << destinationCity << " within the specified date range.\033[0m\n";
    }
}

/*Need to be implemented
void Route::displaywithTransitCities(string * transit_cities, int numberOfTransitCities)
{
    int i = 0;
    if(choice == 'Y' || choice == 'y'){
        while (edge) 
        { 
            if(edge->destination == destinationIndex && isWithinDateRange(edge->flightData->date, startDate, endDate) && edge->flightData->airline == transit_cities[i] && i<numberOfTransitCities) 
            {
                foundFlights = true;
                cout << "\t";
                cout << "|" << std::setw(6) << edge->flightData->date << " |" 
                    << setw(8) << edge->flightData->airline << " |" 
                    << setw(14) << edge->flightData->departureTime << " |" 
                    << setw(12) << edge->flightData->arrivalTime << " |" 
                    << setw(6) << edge->flightData->price << " |" 
                    << setw(9) << edge->flightData->distance << " |\n";
            }
            edge = edge->next;
            i++;
        }
    }
    
}
*/

bool Route::isWithinDateRange(const char* flightDate, const char* startDate, const char* endDate)
{
    int flight = convertDateToComparableFormat(flightDate);
    int start = convertDateToComparableFormat(startDate);
    int end = convertDateToComparableFormat(endDate);

    return flight >= start && flight <= end;
    
}

int Route::convertDateToComparableFormat(const char* date)
{
    int day, month, year;
    char delimiter1, delimiter2;

    std::stringstream dateStream(date);
    dateStream >> day >> delimiter1 >> month >> delimiter2 >> year;

    // Convert date into an integer like YYYYMMDD
    return year * 10000 + month * 100 + day;
}
