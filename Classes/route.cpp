#include "route.h"
#include "menu.h"

// Assuming Route is defined and used elsewhere as in your provided code.

Route::Route(FlightGraph& graph)
    : flightGraph(graph) {}


struct QueueElement {
    int totalTime;
    int cityIndex;
    int lastArrivalTime;
    int* path;     // Dynamic array to store the path
    int pathLength;
    

    QueueElement(int maxCities) 
    {
        path = new int[maxCities];
        pathLength = 0;
    }

    ~QueueElement() {
        delete[] path;
    }

    // Copy constructor for managing dynamic memory
    QueueElement(const QueueElement& other, int maxCities) 
    {
        totalTime = other.totalTime;
        cityIndex = other.cityIndex;
        lastArrivalTime = other.lastArrivalTime;
        pathLength = other.pathLength;

        path = new int[maxCities];
        for (int i = 0; i < pathLength; ++i) 
        {
            path[i] = other.path[i];
        }
    }

    // Assignment operator for managing dynamic memory
    QueueElement& operator=(const QueueElement& other) 
    {
        if (this != &other) 
        {
            delete[] path;

            totalTime = other.totalTime;
            cityIndex = other.cityIndex;
            lastArrivalTime = other.lastArrivalTime;
            pathLength = other.pathLength;

            path = new int[pathLength];
            for (int i = 0; i < pathLength; ++i) {
                path[i] = other.path[i];
            }
        }
        return *this;
    }
};


//For cheapestr flight
struct QueueElement2 
{
    int totalCost;       // Total cost for this path
    int cityIndex;       // Current city index
    int* path;           // Dynamic array for the path
    int pathLength;

    QueueElement2(int maxCities) 
    {
        path = new int[maxCities];
        pathLength = 0;
        totalCost = INT_MAX; // Initialize cost to infinity
    }

    ~QueueElement2() 
    {
        delete[] path;
    }

    QueueElement2(const QueueElement2& other, int maxCities) 
    {
        totalCost = other.totalCost;
        cityIndex = other.cityIndex;
        pathLength = other.pathLength;

        path = new int[maxCities];
        for (int i = 0; i < pathLength; ++i) 
        {
            path[i] = other.path[i];
        }
    }

    QueueElement2& operator=(const QueueElement2& other) 
    {
        if (this != &other) 
        {
            delete[] path;

            totalCost = other.totalCost;
            cityIndex = other.cityIndex;
            pathLength = other.pathLength;

            path = new int[pathLength];
            for (int i = 0; i < pathLength; ++i) 
            {
                path[i] = other.path[i];
            }
        }
        return *this;
    }
};


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

void Route::shortestPath(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate) 
{
    const int MAX_CITIES = flightGraph.getNumVertices();
    int originIndex = flightGraph.getCityIndex(originCity);
    int destinationIndex = flightGraph.getCityIndex(destinationCity);

    if (originIndex == -1 || destinationIndex == -1) 
    {
        std::cout << "\033[1;31mInvalid city name(s): " << originCity << " or " << destinationCity << "\033[0m\n";
        return;
    }

    // Array-based priority queue
    QueueElement* priorityQueue[MAX_CITIES];
    int queueSize = 0;

    // Distance array to track minimum time to each city
    int minTime[MAX_CITIES];
    for (int i = 0; i < MAX_CITIES; ++i) 
    {
        minTime[i] = INT_MAX;
    }

    // Initialize with the origin city
    minTime[originIndex] = 0;
    priorityQueue[queueSize++] = new QueueElement(MAX_CITIES);
    priorityQueue[0]->totalTime = 0;
    priorityQueue[0]->cityIndex = originIndex;
    priorityQueue[0]->lastArrivalTime = 0;
    priorityQueue[0]->path[0] = originIndex;
    priorityQueue[0]->pathLength = 1;

    int* bestPath = new int[MAX_CITIES];
    int bestPathLength = 0;

    while (queueSize > 0) 
    {
        // Find the element with the smallest totalTime
        int minIndex = 0;
        for (int i = 1; i < queueSize; ++i) 
        {
            if (priorityQueue[i]->totalTime < priorityQueue[minIndex]->totalTime) 
            {
                minIndex = i;
            }
        }

        // Extract the minimum element
        QueueElement* current = priorityQueue[minIndex];
        queueSize--;
        for (int i = minIndex; i < queueSize; ++i) 
        {
            priorityQueue[i] = priorityQueue[i + 1];
        }

        // Stop if we reach the destination
        if (current->cityIndex == destinationIndex) 
        {
            bestPathLength = current->pathLength;
            for (int i = 0; i < bestPathLength; ++i) 
            {
                bestPath[i] = current->path[i];
            }
            delete current;
            break;
        }

        // Traverse all outgoing edges (flights) from the current city
        Edge* edge = flightGraph.getVertices()[current->cityIndex].head;
        while (edge) 
        {
            Flight* flight = edge->flightData;

            // Date and time constraints
            if (!isWithinDateRange(flight->date, startDate, endDate)) 
            {
                edge = edge->next;
                continue;
            }

            // Format for departure and arrival times (e.g., "2019-12-02 08:00")
            std::string depTime = std::string(flight->date) + " " + std::string(flight->departureTime);
	    std::string arrTime = std::string(flight->date) + " " + std::string(flight->arrivalTime);


            int travelTime = calculateTravelTime(depTime, arrTime);

            // Ensure the flight departure is after the last arrival time
            if (travelTime < current->lastArrivalTime) 
            {
                edge = edge->next;
                continue;
            }

            // Calculate the new total time
            int newTotalTime = current->totalTime + travelTime;

            // If this path is better, update and push it into the queue
            if (newTotalTime < minTime[edge->destination]) 
            {
                minTime[edge->destination] = newTotalTime;

                QueueElement* newElement = new QueueElement(MAX_CITIES);
                newElement->totalTime = newTotalTime;
                newElement->cityIndex = edge->destination;
                newElement->lastArrivalTime = travelTime; // Set the last arrival time
                newElement->pathLength = current->pathLength;
                for (int i = 0; i < current->pathLength; ++i) 
                {
                    newElement->path[i] = current->path[i];
                }
                newElement->path[newElement->pathLength++] = edge->destination;

                priorityQueue[queueSize++] = newElement;
            }

            edge = edge->next;
        }

        delete current;
    }

	// Display the result
if (bestPathLength > 0) {
    std::cout << "\033[1;36mShortest Path Found:\033[0m\n";
    for (int i = 0; i < bestPathLength - 1; ++i) {
        int currentCityIndex = bestPath[i];
        int nextCityIndex = bestPath[i + 1];

        // Get flight information between the current and next city
        Edge* edge = flightGraph.getVertices()[currentCityIndex].head;
        while (edge) {
            Flight* flight = edge->flightData;

            // If the next city is the destination of this edge, print the details
            if (strcmp(flight->destination, flightGraph.getCityName(nextCityIndex)) == 0)
            {
                std::cout << "Flight from " 
                          << flightGraph.getCityName(currentCityIndex) << " to "
                          << flightGraph.getCityName(nextCityIndex) << ":\n"
                          << "Departure Time: " << flight->departureTime << "\n"
                          << "Arrival Time: " << flight->arrivalTime << "\n"
                          << "Date: " << flight->date << "\n"
                          << "Airline: " << flight->airline << "\n"
                          << "Price: " << flight->price << " USD\n\n";
                break;
            }
            edge = edge->next;
        }
    }
} else {
    std::cout << "\033[1;31mNo path found between " << originCity << " and " << destinationCity
              << " within the given date range.\033[0m\n";
}



    delete[] bestPath;
}


void Route::cheapestFlight(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate) 
{
    const int MAX_CITIES = flightGraph.getNumVertices();

    int originIndex = flightGraph.getCityIndex(originCity);
    int destinationIndex = flightGraph.getCityIndex(destinationCity);

    if (originIndex == -1 || destinationIndex == -1) 
    {
        std::cout << "\033[1;31mInvalid city name(s): " << originCity << " or " << destinationCity << "\033[0m\n";
        return;
    }

    // Array-based priority queue
    QueueElement2* priorityQueue[MAX_CITIES];
    int queueSize = 0;

    // Distance array to track minimum cost to each city
    int minCost[MAX_CITIES];
    for (int i = 0; i < MAX_CITIES; ++i) 
    {
        minCost[i] = INT_MAX;
    }

    // Initialize with the origin city
    minCost[originIndex] = 0;
    priorityQueue[queueSize++] = new QueueElement2(MAX_CITIES);
    priorityQueue[0]->totalCost = 0;
    priorityQueue[0]->cityIndex = originIndex;
    priorityQueue[0]->path[0] = originIndex;
    priorityQueue[0]->pathLength = 1;

    int* bestPath = new int[MAX_CITIES];
    int bestPathLength = 0;

    while (queueSize > 0) 
    {
        // Find the element with the smallest totalCost
        int minIndex = 0;
        for (int i = 1; i < queueSize; ++i) 
        {
            if (priorityQueue[i]->totalCost < priorityQueue[minIndex]->totalCost) 
            {
                minIndex = i;
            }
        }

        // Extract the minimum element
        QueueElement2* current = priorityQueue[minIndex];
        queueSize--;
        for (int i = minIndex; i < queueSize; ++i) 
        {
            priorityQueue[i] = priorityQueue[i + 1];
        }

        // Stop if we reach the destination
        if (current->cityIndex == destinationIndex) 
        {
            bestPathLength = current->pathLength;
            for (int i = 0; i < bestPathLength; ++i) 
            {
                bestPath[i] = current->path[i];
            }
            delete current;
            break;
        }

        // Traverse all outgoing edges (flights) from the current city
        Edge* edge = flightGraph.getVertices()[current->cityIndex].head;
        while (edge) {
            Flight* flight = edge->flightData;

            // Date constraint
            if (!isWithinDateRange(flight->date, startDate, endDate)) {
                edge = edge->next;
                continue;
            }

            // Calculate new total cost
            int newTotalCost = current->totalCost + flight->price;

            // If this path is cheaper, update and push it into the queue
            if (newTotalCost < minCost[edge->destination]) 
            {
                minCost[edge->destination] = newTotalCost;

                QueueElement2* newElement = new QueueElement2(MAX_CITIES);
                newElement->totalCost = newTotalCost;
                newElement->cityIndex = edge->destination;
                newElement->pathLength = current->pathLength;
                for (int i = 0; i < current->pathLength; ++i) 
                {
                    newElement->path[i] = current->path[i];
                }
                newElement->path[newElement->pathLength++] = edge->destination;

                priorityQueue[queueSize++] = newElement;
            }

            edge = edge->next;
        }

        delete current;
    }

    // Display the result
if (bestPathLength > 0) {
    std::cout << "\033[1;36mCheapest Flight Found:\033[0m\n";

    // Loop through the best path and display flight details
    for (int i = 0; i < bestPathLength - 1; ++i) 
    {
        int currentCityIndex = bestPath[i];
        int nextCityIndex = bestPath[i + 1];

        // Find the flight between the current and next city
        Edge* edge = flightGraph.getVertices()[currentCityIndex].head;
        while (edge) {
            Flight* flight = edge->flightData;

            // Check if the destination of the flight matches the next city in the path
            if (strcmp(flight->destination, flightGraph.getCityName(nextCityIndex)) == 0) {
                // Display flight details
                std::cout << "Flight from " << flight->origin << " to " << flight->destination << ":\n";
                std::cout << "Airline: " << flight->airline << "\n";
                std::cout << "Date: " << flight->date << "\n";
                std::cout << "Departure Time: " << flight->departureTime << "\n";
                std::cout << "Arrival Time: " << flight->arrivalTime << "\n";
                std::cout << "Price: $" << flight->price << "\n";
                std::cout << "Distance: " << flight->distance << " km\n";
                std::cout << endl << endl;
                break;  // We found the flight, so we can exit the loop
            }
            edge = edge->next;
        }
    }

    // Display total cost
    std::cout << "Total Cost: $" << minCost[destinationIndex] << "\n";
} 
else 
{
    std::cout << "\033[1;31mNo flight found between " << originCity << " and " << destinationCity
              << " within the given date range.\033[0m\n";
}


    delete[] bestPath;
}



void Route::listShortestAndCheapest(const char* startCity, const char* endCity)
{
}

LinkedList Route::listDirectFlightsWithinDateRange(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate)
{
    int originIndex = flightGraph.getCityIndex(originCity);
    int destinationIndex = flightGraph.getCityIndex(destinationCity);

    if (originIndex == -1 || destinationIndex == -1) 
    {
        cout << "\033[1;31mInvalid city name(s): " << originCity << " or " << destinationCity << "\033[0m\n";
        LinkedList empty;
        return empty;
    }

    bool foundFlights = false;
    LinkedList directFlights;

    Edge* edge = flightGraph.getVertices()[originIndex].head;


    while (edge) 
    {
        if (edge->destination == destinationIndex && isWithinDateRange(edge->flightData->date, startDate, endDate)) 
        {
            foundFlights = true;
            Flight flight(
                originCity, destinationCity, edge->flightData->airline, edge->flightData->date, 
                edge->flightData->departureTime, edge->flightData->arrivalTime, 
                edge->flightData->price, edge->flightData->distance
            );
            directFlights.insert(flight);
        }
        edge = edge->next;
    }


    if (!foundFlights) 
    {
        cout << "\033[1;31mNo direct flights available from " << originCity << " to " << destinationCity << ".\033[0m\n";
    }

    return directFlights;
}

LinkedList Route::listIndirectFlightsWithinDateRange(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate) 
{
    int originIndex = flightGraph.getCityIndex(originCity);
    int destinationIndex = flightGraph.getCityIndex(destinationCity);

    if (originIndex == -1 || destinationIndex == -1) 
    {
        cout << "\033[1;31mInvalid city name(s): " << originCity << " or " << destinationCity << "\033[0m\n";
        return LinkedList(); // Return an empty LinkedList
    }

    LinkedList indirectFlights;
    FlightRoute route;

    bool foundFlights = false;
    Edge* edge = flightGraph.getVertices()[originIndex].head;

    while (edge) 
    {
        int intermediateIndex = edge->destination;
        Edge* intermediateEdge = flightGraph.getVertices()[intermediateIndex].head;

        while (intermediateEdge) 
        {
            if (intermediateEdge->destination == destinationIndex && 
                isWithinDateRange(edge->flightData->date, startDate, endDate) && 
                isWithinDateRange(intermediateEdge->flightData->date, startDate, endDate)) 
            {
                // Create flights for each leg
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

                indirectFlights.insert(firstLegFlight);
                indirectFlights.insert(secondLegFlight);

                foundFlights = true;
            }
            intermediateEdge = intermediateEdge->next;
        }
        edge = edge->next;
    }

    if (!foundFlights)
    {
        cout << "\033[1;31mNo indirect flights found for the specified cities and dates.\033[0m\n";
    }

    return indirectFlights;
}

LinkedList Route::findFlightsWithTransitCities(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, string* transitCities, int numberOfTransitCities) 
{
    int originIndex = flightGraph.getCityIndex(originCity);
    int destinationIndex = flightGraph.getCityIndex(destinationCity);

    if (originIndex == -1 || destinationIndex == -1) {
        cout << "\033[1;31mInvalid city name(s): " << originCity << " or " << destinationCity << "\033[0m\n";
        LinkedList empty;
        return empty;
    }

    bool foundFlights = false;

   // LinkedList directFlightsList;           //Removing this as we dont need direct flights in linkedlist

    // Linked List to store indirect flights
    LinkedList indirectFlightsList;

    // Then, handle flights through each transit city
    for (int i = 0; i < numberOfTransitCities; ++i) {
        string transitCity = transitCities[i];
        int transitIndex = flightGraph.getCityIndex(transitCity.c_str());

        if (transitIndex == -1) 
        {
            cout << "\033[1;31mInvalid transit city: " << transitCity << "\033[0m\n";
            continue;
        }

        // Treat each leg as a new route (origin -> transitCity -> destination)
        cout << "\033[1;36m\nSearching flights via " << transitCity << "...\033[0m\n";
        
        // Origin -> Transit
        foundFlights |= findDirectAndIndirectFlights(originCity, transitCity.c_str(), startDate, endDate, indirectFlightsList);

        // Transit -> Destination
        foundFlights |= findDirectAndIndirectFlights(transitCity.c_str(), destinationCity, startDate, endDate, indirectFlightsList);
    }

    // Display all flights (direct and indirect for each segment)
    if (foundFlights) 
    {
    	/*cout << "\033[1;36m\n\t\tAll Direct Flights from " << originCity << " to " << destinationCity << " with Transit Cities\n";
    	directFlightsList.Display();*/

        cout << "\033[1;36m\n\t\tAll Indirect Flights from " << originCity << " to " << destinationCity << " with Transit Cities\n";
    	indirectFlightsList.Display();
        return indirectFlightsList;
    } 
    else 
    {
        cout << "\033[1;31mNo flights found for the specified cities and dates.\033[0m\n";
        LinkedList empty; 
        return empty;
    }
}

LinkedList Route::filterByTransitCitiesAndAirline( const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, string* transitCities, int numberOfTransitCities, const string& preferredAirline) 
{
    LinkedList transitFilteredFlights;

    for (int i = 0; i < numberOfTransitCities; ++i) {
        string transitCity = transitCities[i];
        int transitIndex = flightGraph.getCityIndex(transitCity.c_str());

        if (transitIndex == -1) {
            cout << "\033[1;31mInvalid transit city: " << transitCity << "\033[0m\n";
            continue;
        }

        // Find flights via the transit city
        findDirectAndIndirectFlights(originCity, transitCity.c_str(), startDate, endDate, transitFilteredFlights);
        findDirectAndIndirectFlights(transitCity.c_str(), destinationCity, startDate, endDate, transitFilteredFlights);
    }

    if (transitFilteredFlights.isEmpty()) {
        cout << "\033[1;31mNo flights available matching transit cities.\033[0m\n";
        return LinkedList();
    }

    LinkedList airlineFilteredFlights;

    LinkedList::FlightNode* current = transitFilteredFlights.getHead();
    while (current) {
        Flight& flight = current->flight;

        if (flight.airline == preferredAirline) 
        {
            airlineFilteredFlights.insert(flight);
        }

        current = current->next;
    }

    if(airlineFilteredFlights.isEmpty()) 
    {
        cout << "\033[1;31mNo flights available matching both transit cities and airline.\033[0m\n";
    } else {
        cout << "\033[1;36m Filtered Flights from " << originCity << " to " << destinationCity << " with Transit Cities and Preferred Airline \"" << preferredAirline << "\"\n";
        airlineFilteredFlights.Display();
    }

    return airlineFilteredFlights;
}

LinkedList Route::listAllFlightsWithinDataRangeandPreferredAirline(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, string airline)
{
    int originIndex = flightGraph.getCityIndex(originCity);
    int destinationIndex = flightGraph.getCityIndex(destinationCity);

    if (originIndex == -1 || destinationIndex == -1) 
    {
        cout << "\033[1;31mInvalid city name(s): " << originCity << " or " << destinationCity << "\033[0m\n";
        return LinkedList(); // Return an empty LinkedList
    }

    bool foundFlights = false;
    LinkedList preferredFlights; // Store flights matching the criteria

    // Check direct flights
    Edge* edge = flightGraph.getVertices()[originIndex].head;

    cout << "\033[1;36mDirect Flights from " << originCity << " to " << destinationCity 
         << " with Preferred Airline \"" << airline << "\"\033[0m\n";
    cout << "\t ________________________________________________________________________________________________________________________\n";
    cout << "\t| Origin         | Destination    | Date      | Airline        | Departure Time | Arrival Time   | Price     | Distance   |\n";
    cout << "\t|________________|________________|___________|________________|________________|________________|___________|____________|\n";

    while (edge) 
    { 
        if (edge->destination == destinationIndex && 
            isWithinDateRange(edge->flightData->date, startDate, endDate) && 
            edge->flightData->airline == airline) 
        {
            foundFlights = true;
            Flight flight(
                originCity, destinationCity, edge->flightData->airline, edge->flightData->date, 
                edge->flightData->departureTime, edge->flightData->arrivalTime, 
                edge->flightData->price, edge->flightData->distance
            );
            preferredFlights.insert(flight);

            // Display flight information
            cout << "\t|" << setw(15) << originCity << " |" 
                 << setw(15) << destinationCity << " |" 
                 << setw(10) << edge->flightData->date << " |" 
                 << setw(15) << edge->flightData->airline << " |" 
                 << setw(15) << edge->flightData->departureTime << " |" 
                 << setw(15) << edge->flightData->arrivalTime << " |" 
                 << setw(10) << edge->flightData->price << " |" 
                 << setw(10) << edge->flightData->distance << " |\n";
        }
        edge = edge->next;
    }
    cout << "\t|________________|________________|___________|________________|________________|________________|___________|___________|\033[0m\n";

    // Check indirect flights
    edge = flightGraph.getVertices()[originIndex].head;

    cout << "\033[1;36mIndirect Flights from " << originCity << " to " << destinationCity 
         << " with Preferred Airline \"" << airline << "\"\033[0m\n";
    cout << "\t ________________________________________________________________________________________________________________________\n";
    cout << "\t| Origin         | Destination    | Date      | Airline        | Departure Time | Arrival Time   | Price     | Distance   |\n";
    cout << "\t|________________|________________|___________|________________|________________|________________|___________|____________|\n";

    while (edge) 
    {
        int intermediateIndex = edge->destination;
        Edge* intermediateEdge = flightGraph.getVertices()[intermediateIndex].head;

        while (intermediateEdge) 
        {
            if (intermediateEdge->destination == destinationIndex && 
                isWithinDateRange(edge->flightData->date, startDate, endDate) && 
                isWithinDateRange(intermediateEdge->flightData->date, startDate, endDate) && 
                edge->flightData->airline == airline) 
            {
                foundFlights = true;
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

                preferredFlights.insert(firstLegFlight);
                preferredFlights.insert(secondLegFlight);

                // Display flight information
                cout << "\t|" << setw(15) << originCity << " |" 
                     << setw(15) << destinationCity << " |" 
                     << setw(10) << edge->flightData->date << " |" 
                     << setw(15) << edge->flightData->airline << " |" 
                     << setw(15) << edge->flightData->departureTime << " |" 
                     << setw(15) << intermediateEdge->flightData->arrivalTime << " |" 
                     << setw(10) << edge->flightData->price + intermediateEdge->flightData->price << " |" 
                     << setw(10) << edge->flightData->distance + intermediateEdge->flightData->distance << " |\n";
            }
            intermediateEdge = intermediateEdge->next;
        }
        edge = edge->next;
    }

    cout << "\t|________________|________________|___________|________________|________________|________________|___________|___________|\033[0m\n";

    if (!foundFlights) 
    {
        cout << "\t\033[1;31mNo flights available from " << originCity << " to " << destinationCity << " with the preferred airline \"" << airline << "\".\033[0m\n";
    }

    return preferredFlights;
}

RouteList Route::listIndirectRoutesWithinDateRange(
    const char* originCity, 
    const char* destinationCity, 
    const char* startDate, 
    const char* endDate
) 
{
    RouteList allRoutes;

    int originIndex = flightGraph.getCityIndex(originCity);
    int destinationIndex = flightGraph.getCityIndex(destinationCity);

    if (originIndex == -1 || destinationIndex == -1) {
        cout << "Invalid city name(s): " << originCity << " or " << destinationCity << "\n";
        return allRoutes; // Empty
    }

    int numCities = flightGraph.getNumVertices();
    bool* visited = new bool[numCities];
    for (int i = 0; i < numCities; i++) {
        visited[i] = false;
    }

    LinkedList currentRoute; // Use LinkedList instead of vector for current route

    // DFS from origin to destination
    dfsBuildRoutes(originIndex, destinationIndex, startDate, endDate, visited, currentRoute, allRoutes);

    delete[] visited; // Clean up dynamic array

    if (!allRoutes.hasRoutes()) {
        cout << "No indirect routes found for the specified cities and dates.\n";
    }

    return allRoutes;
}

void Route::dfsBuildRoutes(
    int currentIndex,
    int destinationIndex,
    const char* startDate,
    const char* endDate,
    bool* visited,
    LinkedList& currentRoute,
    RouteList& allRoutes
) 
{
    // If we've reached the destination city, currentRoute holds a full journey
    if (currentIndex == destinationIndex) {
        // Convert currentRoute to a Route and add it to allRoutes
        FlightRoute completeRoute;
        LinkedList::FlightNode* node = currentRoute.getHead();
        while (node) {
            completeRoute.legs.insert(node->flight);
            node = node->next;
        }
        allRoutes.insertRoute(completeRoute);
        return;
    }

    visited[currentIndex] = true;
    Edge* edge = flightGraph.getVertices()[currentIndex].head;

    // Get the arrival time of the last flight in the current route (if any)
    Flight* lastFlight = currentRoute.getLastFlight();
    const char* lastArrivalTime = (lastFlight) ? lastFlight->arrivalTime : NULL;

    while (edge) {
        int nextCity = edge->destination;
        if (!visited[nextCity]) {
            // Check if flight falls within date range
            if (isWithinDateRange(edge->flightData->date, startDate, endDate)) {
                // Check time feasibility: either no previous flight or departure >= last arrival
                if (lastArrivalTime == NULL || compareTimes(edge->flightData->departureTime, lastArrivalTime) >= 0) {
                    // Add this flight to currentRoute
                    Flight nextFlight(
                        flightGraph.getCityName(currentIndex),
                        flightGraph.getCityName(nextCity),
                        edge->flightData->airline,
                        edge->flightData->date,
                        edge->flightData->departureTime,
                        edge->flightData->arrivalTime,
                        edge->flightData->price,
                        edge->flightData->distance
                    );

                    currentRoute.insert(nextFlight); // Insert at end of LinkedList

                    // Recurse
                    dfsBuildRoutes(nextCity, destinationIndex, startDate, endDate, visited, currentRoute, allRoutes);

                    // Backtrack: remove the last flight from currentRoute
                    currentRoute.removeLast();
                }
            }
        }
        edge = edge->next;
    }

    visited[currentIndex] = false;
}

int Route::compareTimes(const char* time1, const char* time2) {
    // Convert HH:MM to integer minutes and compare
    int h1, m1, h2, m2;
    sscanf(time1, "%d:%d", &h1, &m1);
    sscanf(time2, "%d:%d", &h2, &m2);
    int total1 = h1*60+m1;
    int total2 = h2*60+m2;
    if (total1 == total2) return 0;
    return (total1 > total2) ? 1 : -1;
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
bool Route::findDirectAndIndirectFlights(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, LinkedList& indirectFlightsList) {
    int originIndex = flightGraph.getCityIndex(originCity);
    int destinationIndex = flightGraph.getCityIndex(destinationCity);
    
    if (originIndex == -1 || destinationIndex == -1) {
        cout << "\033[1;31mInvalid city name(s): " << originCity << " or " << destinationCity << "\033[0m\n";
        return false;
    }

    bool foundFlights = false;

    // Check for indirect flights (through transit cities)
    Edge * edge = flightGraph.getVertices()[originIndex].head;
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
bool Route::isWithinDateRange(const char* flightDate, const char* startDate, const char* endDate)
{
    int flight = convertDateToComparableFormat(flightDate);
    int start = convertDateToComparableFormat(startDate);
    int end = convertDateToComparableFormat(endDate);

    return flight >= start && flight <= end;
    
}


// Function to calculate travel time between two time stamps with full date information
int Route::calculateTravelTime(const std::string& departure, const std::string& arrival) 
{
    std::tm depTime = {}, arrTime = {};
    
    // Parse date and time (format: YYYY-MM-DD HH:MM)
    strptime(departure.c_str(), "%Y-%m-%d %H:%M", &depTime);
    strptime(arrival.c_str(), "%Y-%m-%d %H:%M", &arrTime);
    
    // Convert to time_t for easier calculations (seconds since epoch)
    time_t depEpoch = mktime(&depTime);
    time_t arrEpoch = mktime(&arrTime);
    
    // If arrival time is earlier than departure, we assume it's the next day
    if (arrEpoch < depEpoch) {
        arrEpoch += 24 * 60 * 60;  // Add 24 hours to arrival time
    }

    // Return the difference in minutes
    return difftime(arrEpoch, depEpoch) / 60;
}
