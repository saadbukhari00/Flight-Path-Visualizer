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
        cout << "\033[1;33m";
        cout << "\n\t\tShortest Route from " << startCity << " to " << endCity << "\n";
        cout << "  \t _____________________________________________________________________\n";
        cout << "\t| Date     | Airline | Departure Time| Arrival Time| Price | Distance |\n";
        cout << "\t|__________|_________|_______________|_____________|_______|__________|\n";
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
        cout << "\033[1;33m";
        cout << "\n\t\tCheapest Route from " << startCity << " to " << endCity << "\n";
        cout << "  \t _____________________________________________________________________\n";
        cout << "\t| Date     | Airline | Departure Time| Arrival Time| Price | Distance |\n";
        cout << "\t|__________|_________|_______________|_____________|_______|__________|\n";
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

    if(originIndex == -1 || destinationIndex == -1) 
    {
        cout << "\033[1;31mInvalid city name(s): " << originCity << " or " << destinationCity << "\033[0m\n";
        return;
    }

    bool foundFlights = false;
    // Check direct flights
    Edge* edge = flightGraph.getVertices()[originIndex].head;

    cout << "\033[1;36m";
    cout << "\n\t\tDirect Flights from " << originCity << " to " << destinationCity << "\n";
    cout << "  \t _____________________________________________________________________\n";
    cout << "\t| Date     | Airline | Departure Time| Arrival Time| Price | Distance |\n";
    cout << "\t|__________|_________|_______________|_____________|_______|__________|\n";

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

    // Check indirect flights
    edge = flightGraph.getVertices()[originIndex].head;

    cout << "\033[1;36m";
    cout << "\n\t\t\tIndirect Flights from " << originCity << " to " << destinationCity << "\n";
    cout << "  \t ______________________________________________________________________________________________\n";
    cout << "\t| First Leg Date | Second Leg Date | Airline | Departure Time | Arrival Time | Price | Distance |\n";
    cout << "\t|________________|_________________|_________|________________|______________|_______|__________|\n";
    while (edge) 
    {
        int intermediateIndex = edge->destination;
        Edge* intermediateEdge = flightGraph.getVertices()[intermediateIndex].head;
        
        while (intermediateEdge) 
        {
            if (intermediateEdge->destination == destinationIndex && isWithinDateRange(edge->flightData->date, startDate, endDate) && isWithinDateRange(intermediateEdge->flightData->date, startDate, endDate)) 
            {
                foundFlights = true;
                cout << "\t";
                cout << "|" << setw(15) << edge->flightData->date << " |" 
                     << setw(16) << intermediateEdge->flightData->date << " |" 
                     << setw(8) << edge->flightData->airline << " |" 
                     << setw(15) << edge->flightData->departureTime << " |" 
                     << setw(13) << intermediateEdge->flightData->arrivalTime << " |" 
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

void Route::findFlightsWithTransitCities(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, string* transitCities, int numberOfTransitCities) 
{
    int originIndex = flightGraph.getCityIndex(originCity);
    int destinationIndex = flightGraph.getCityIndex(destinationCity);

    if (originIndex == -1 || destinationIndex == -1) {
        cout << "\033[1;31mInvalid city name(s): " << originCity << " or " << destinationCity << "\033[0m\n";
        return;
    }

    bool foundFlights = false;

    // Linked List to store direct flights
    LinkedList directFlightsList;

    // Linked List to store indirect flights
    LinkedList indirectFlightsList;

    // Then, handle flights through each transit city
    for (int i = 0; i < numberOfTransitCities; ++i) {
        string transitCity = transitCities[i];
        int transitIndex = flightGraph.getCityIndex(transitCity.c_str());

        if (transitIndex == -1) {
            cout << "\033[1;31mInvalid transit city: " << transitCity << "\033[0m\n";
            continue;
        }

        // Treat each leg as a new route (origin -> transitCity -> destination)
        cout << "\033[1;36mSearching flights via " << transitCity << "...\033[0m\n";
        
        // Origin -> Transit
        foundFlights |= findDirectAndIndirectFlights(originCity, transitCity.c_str(), startDate, endDate, directFlightsList, indirectFlightsList);

        // Transit -> Destination
        foundFlights |= findDirectAndIndirectFlights(transitCity.c_str(), destinationCity, startDate, endDate, directFlightsList, indirectFlightsList);
    }

    // Display all flights (direct and indirect for each segment)
    if (foundFlights) 
    {
    	cout << "\033[1;36m\n\t\tAll Direct Flights from " << originCity << " to " << destinationCity << " with Transit Cities\n";
    	cout << "  \t_____________________________________________________________________________________\n";
    	cout << "\t| Origin        | Destination   | Date     | Airline       | Departure Time| Arrival Time  | Price    | Distance |\n";
    	cout << "\t|_______________|_______________|__________|_______________|_______________|_______________|__________|__________|\n";
    	directFlightsList.Display();

        cout << "\033[1;36m\n\t\tAll Indirect Flights from " << originCity << " to " << destinationCity << " with Transit Cities\n";
    	cout << "  \t_____________________________________________________________________________________\n";
    	cout << "\t| Origin        | Destination   | Date     | Airline       | Departure Time| Arrival Time  | Price    | Distance |\n";
    	cout << "\t|_______________|_______________|__________|_______________|_______________|_______________|__________|__________|\n";
    	indirectFlightsList.Display();
    } 
    else 
    {
        cout << "\033[1;31mNo flights found for the specified cities and dates.\033[0m\n";
    }
}

bool Route::findDirectAndIndirectFlights(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, LinkedList& directFlightsList, LinkedList& indirectFlightsList) {
    int originIndex = flightGraph.getCityIndex(originCity);
    int destinationIndex = flightGraph.getCityIndex(destinationCity);
    
    if (originIndex == -1 || destinationIndex == -1) {
        cout << "\033[1;31mInvalid city name(s): " << originCity << " or " << destinationCity << "\033[0m\n";
        return false;
    }

    bool foundFlights = false;

    // Check for direct flights
    Edge* edge = flightGraph.getVertices()[originIndex].head;
    while (edge) {
        if (edge->destination == destinationIndex && isWithinDateRange(edge->flightData->date, startDate, endDate)) 
        {
            Flight directFlight(
                originCity, destinationCity, edge->flightData->airline, edge->flightData->date,
                edge->flightData->departureTime, edge->flightData->arrivalTime,
                edge->flightData->price, edge->flightData->distance
            );
            
            directFlightsList.insert(directFlight);
            foundFlights = true;
        }
        edge = edge->next;
    }

    // Check for indirect flights (through transit cities)
    edge = flightGraph.getVertices()[originIndex].head;
    while (edge) {
        int intermediateIndex = edge->destination;
        Edge* intermediateEdge = flightGraph.getVertices()[intermediateIndex].head;

        while (intermediateEdge) {
            if (intermediateEdge->destination == destinationIndex && 
                isWithinDateRange(edge->flightData->date, startDate, endDate) && 
                isWithinDateRange(intermediateEdge->flightData->date, startDate, endDate)) {

                Flight firstLegFlight(
                    originCity, flightGraph.getCityName(intermediateIndex), edge->flightData->airline, edge->flightData->date,
                    edge->flightData->departureTime, edge->flightData->arrivalTime,
                    edge->flightData->price, edge->flightData->distance
                );

                Flight secondLegFlight(
                    flightGraph.getCityName(intermediateIndex), destinationCity, intermediateEdge->flightData->airline, intermediateEdge->flightData->date,
                    intermediateEdge->flightData->departureTime, intermediateEdge->flightData->arrivalTime,
                    intermediateEdge->flightData->price, intermediateEdge->flightData->distance
                );

                indirectFlightsList.insert(firstLegFlight);
                indirectFlightsList.insert(secondLegFlight);
                foundFlights = true;
            }
            intermediateEdge = intermediateEdge->next;
        }
        edge = edge->next;
    }

    return foundFlights;
}




void Route::listAllFlightsWithinDataRangeandPreferredAirline(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, string airline)
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
    // Check direct flights
    Edge* edge = flightGraph.getVertices()[originIndex].head;

    cout << "\033[1;36m\033[1m\033[1;36m";
    cout << "\n\t\tDirect Flights from " << originCity << " to " << destinationCity << " with Preferred Airline \"" << airline <<"\"\n";
    cout << "  \t _____________________________________________________________________\n";
    cout << "\t| Date     | Airline | Departure Time| Arrival Time| Price | Distance |\n";
    cout << "\t|__________|_________|_______________|_____________|_______|__________|\n";
    while (edge) 
    { 
        if(edge->destination == destinationIndex && isWithinDateRange(edge->flightData->date, startDate, endDate) && edge->flightData->airline == airline) 
        {
            foundFlights = true;
            cout << "\t";
            cout << "|" << setw(6) << edge->flightData->date << " |" 
                 << setw(8) << edge->flightData->airline << " |" 
                 << setw(14) << edge->flightData->departureTime << " |" 
                 << setw(12) << edge->flightData->arrivalTime << " |" 
                 << setw(6) << edge->flightData->price << " |" 
                 << setw(9) << edge->flightData->distance << " |\n";
        }
        edge = edge->next;
    }
    cout << "\t|__________|_________|_______________|_____________|_______|__________|\033[0m\n";

    // Check indirect flights
    edge = flightGraph.getVertices()[originIndex].head;


    cout << "\033[1;36m\033[1m\033[1;36m";
    cout << "\t\t\tIndirect Flights from " << originCity << " to " << destinationCity << " with Preferred Airline \"" << airline <<"\"\n";
    cout << "  \t ______________________________________________________________________________________________\n";
    cout << "\t| First Leg Date | Second Leg Date | Airline | Departure Time | Arrival Time | Price | Distance |\n";
    cout << "\t|________________|_________________|_________|________________|______________|_______|__________|\n";

    while (edge) 
    {
        int intermediateIndex = edge->destination;
        Edge* intermediateEdge = flightGraph.getVertices()[intermediateIndex].head;

        while (intermediateEdge) 
        {
            if (intermediateEdge->destination == destinationIndex && isWithinDateRange(edge->flightData->date, startDate, endDate) && isWithinDateRange(intermediateEdge->flightData->date, startDate, endDate) && edge->flightData->airline == airline)
            {
                foundFlights = true;
                cout << "\t";
                cout << "|" << setw(15) << edge->flightData->date << " |" 
                     << setw(16) << intermediateEdge->flightData->date << " |" 
                     << setw(8) << edge->flightData->airline << " |" 
                     << setw(15) << edge->flightData->departureTime << " |" 
                     << setw(13) << intermediateEdge->flightData->arrivalTime << " |" 
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
