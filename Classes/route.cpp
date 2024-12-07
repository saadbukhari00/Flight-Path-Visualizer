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

// A helper function to display a list of flights in a table
void displayFlightList(const LinkedList &flights, const char* headerColor, const char* headerTitle) {
    if (flights.isEmpty()) {
        std::cout << "\033[1;31mNo " << headerTitle << " found.\033[0m\n";
        return;
    }

    std::cout << headerColor << headerTitle << "\033[0m\n";
    std::cout << "       _______________________________________________________________________________________________________________\n";
    std::cout << "      |  IDX  | Origin         | Destination    | Date      | Airline        | Departure Time | Arrival Time   | Price     |\n";
    std::cout << "      |_______|________________|________________|___________|________________|________________|________________|___________|\n";

    LinkedList::FlightNode* curr = flights.getHead();
    int index = 0;
    while (curr) {
        const Flight &f = curr->flight;
        std::cout << "      |" << std::setw(6) << index << " |"
                  << std::setw(15) << f.origin << " |"
                  << std::setw(15) << f.destination << " |"
                  << std::setw(10) << f.date << " |"
                  << std::setw(15) << f.airline << " |"
                  << std::setw(15) << f.departureTime << " |"
                  << std::setw(15) << f.arrivalTime << " |"
                  << std::setw(10) << f.price << " |\n";
        curr = curr->next;
        index++;
    }
    std::cout << "      |_______|________________|________________|___________|________________|________________|________________|___________|\n\n";
}

void Route::displayShortestPathResult(const char* originCity, const char* destinationCity,
                                      LinkedList &shortestPathFlights,
                                      LinkedList &directFlights, 
                                      RouteList &indirectRoutes) 
{
    if (shortestPathFlights.isEmpty()) {
        std::cout << "\033[1;31mNo shortest path found from " << originCity << " to " << destinationCity 
                  << " within the given date range.\033[0m\n";
        return;
    }

    // Display the shortest path flights
    displayFlightList(shortestPathFlights, "\033[1;36m", "Shortest Path Found");

    // Now attempt to find matches for each flight in shortestPathFlights
    LinkedList::FlightNode* node = shortestPathFlights.getHead();
    int legIndex = 0;
    while (node) {
        const Flight &f = node->flight;
        bool foundDirect = false, foundIndirect = false;

        // Check direct flights
        LinkedList::FlightNode* dCurr = directFlights.getHead();
        int dIndex = 0;
        while (dCurr) {
            const Flight &df = dCurr->flight;
            if (strcmp(df.origin, f.origin) == 0 &&
                strcmp(df.destination, f.destination) == 0 &&
                strcmp(df.date, f.date) == 0 &&
                strcmp(df.departureTime, f.departureTime) == 0 &&
                strcmp(df.arrivalTime, f.arrivalTime) == 0) {
                std::cout << "\033[1;33mLeg " << legIndex << ": Matches Direct Flight Index: " << dIndex << "\033[0m\n";
                foundDirect = true;
                break;
            }
            dCurr = dCurr->next;
            dIndex++;
        }

        // If not direct, check indirect routes
        if (!foundDirect) {
            RouteList::RouteNode* rCurr = indirectRoutes.getHead();
            int rIndex = 0;
            while (rCurr) {
                LinkedList::FlightNode* iLeg = rCurr->route.legs.getHead();
                while (iLeg) {
                    const Flight &iflight = iLeg->flight;
                    if (strcmp(iflight.origin, f.origin) == 0 &&
                        strcmp(iflight.destination, f.destination) == 0 &&
                        strcmp(iflight.date, f.date) == 0 &&
                        strcmp(iflight.departureTime, f.departureTime) == 0 &&
                        strcmp(iflight.arrivalTime, f.arrivalTime) == 0) {
                        std::cout << "\033[1;33mLeg " << legIndex << ": Matches Indirect Route Index: " << rIndex << "\033[0m\n";
                        foundIndirect = true;
                        break;
                    }
                    iLeg = iLeg->next;
                }
                if (foundIndirect) break;
                rCurr = rCurr->next;
                rIndex++;
            }
        }

        if (!foundDirect && !foundIndirect) {
            std::cout << "\033[1;33mLeg " << legIndex << ": No direct or indirect match found.\033[0m\n";
        }

        node = node->next;
        legIndex++;
    }
    std::cout << "\n";
}

void Route::displayCheapestFlightResult(const char* originCity, const char* destinationCity,
                                        LinkedList &cheapestPathFlights,
                                        LinkedList &directFlights, 
                                        RouteList &indirectRoutes)
{
    if (cheapestPathFlights.isEmpty()) {
        std::cout << "\033[1;31mNo cheapest path found from " << originCity << " to " << destinationCity 
                  << " within the given date range.\033[0m\n";
        return;
    }

    // Display the cheapest path flights
    displayFlightList(cheapestPathFlights, "\033[1;36m", "Cheapest Flight Found");

    // Attempt to find matches for each leg
    LinkedList::FlightNode* node = cheapestPathFlights.getHead();
    int legIndex = 0;
    while (node) {
        const Flight &f = node->flight;
        bool foundDirect = false, foundIndirect = false;

        // Check direct flights
        LinkedList::FlightNode* dCurr = directFlights.getHead();
        int dIndex = 0;
        while (dCurr) {
            const Flight &df = dCurr->flight;
            if (strcmp(df.origin, f.origin) == 0 &&
                strcmp(df.destination, f.destination) == 0 &&
                strcmp(df.date, f.date) == 0 &&
                strcmp(df.departureTime, f.departureTime) == 0 &&
                strcmp(df.arrivalTime, f.arrivalTime) == 0) {
                std::cout << "\033[1;33mLeg " << legIndex << ": Matches Direct Flight Index: " << dIndex << "\033[0m\n";
                foundDirect = true;
                break;
            }
            dCurr = dCurr->next;
            dIndex++;
        }

        // If not direct, check indirect routes
        if (!foundDirect) {
            RouteList::RouteNode* rCurr = indirectRoutes.getHead();
            int rIndex = 0;
            while (rCurr) {
                LinkedList::FlightNode* iLeg = rCurr->route.legs.getHead();
                while (iLeg) {
                    const Flight &iflight = iLeg->flight;
                    if (strcmp(iflight.origin, f.origin) == 0 &&
                        strcmp(iflight.destination, f.destination) == 0 &&
                        strcmp(iflight.date, f.date) == 0 &&
                        strcmp(iflight.departureTime, f.departureTime) == 0 &&
                        strcmp(iflight.arrivalTime, f.arrivalTime) == 0) {
                        std::cout << "\033[1;33mLeg " << legIndex << ": Matches Indirect Route Index: " << rIndex << "\033[0m\n";
                        foundIndirect = true;
                        break;
                    }
                    iLeg = iLeg->next;
                }
                if (foundIndirect) break;
                rCurr = rCurr->next;
                rIndex++;
            }
        }

        if (!foundDirect && !foundIndirect) {
            std::cout << "\033[1;33mLeg " << legIndex << ": No direct or indirect match found.\033[0m\n";
        }

        node = node->next;
        legIndex++;
    }
    std::cout << "\n";
}

void Route::shortestPath(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, LinkedList& directFlights, RouteList& indirectRoutes)
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

    // Display the result only once for the best path
    if (bestPathLength > 0) 
    {
        std::cout << "\033[1;36mShortest Path Found:\033[0m\n";
        for (int i = 0; i < bestPathLength - 1; ++i) 
        {
            int currentCityIndex = bestPath[i];
            int nextCityIndex = bestPath[i + 1];

            // Get flight information between the current and next city
            Edge* edge = flightGraph.getVertices()[currentCityIndex].head;
            while (edge) 
            {
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
                              << "Price: " << flight->price << " USD\n";

                    // Compare with direct flights
                    bool foundDirect = false;
                    LinkedList::FlightNode* directCurr = directFlights.getHead();
                    int index = 0;
                    while (directCurr) 
                    {
                        if (strcmp(directCurr->flight.origin, flightGraph.getCityName(currentCityIndex)) == 0 &&
    strcmp(directCurr->flight.destination, flightGraph.getCityName(nextCityIndex)) == 0 &&
    strcmp(directCurr->flight.date, flight->date) == 0 &&
    strcmp(directCurr->flight.departureTime, flight->departureTime) == 0 &&
    strcmp(directCurr->flight.arrivalTime, flight->arrivalTime) == 0)
                        {
                            std::cout << "\tDirect Flight Index: " << index << "\n";
                            foundDirect = true;
                            break;
                        }
                        directCurr = directCurr->next;
                        index++;
                    }
                    
                    if (!foundDirect)
                    {
                        // Compare with indirect routes
                        bool foundIndirect = false;
                        RouteList::RouteNode* indirectCurr = indirectRoutes.getHead();
                        int indirectIndex = 0;
                        while (indirectCurr) 
                        {
                            LinkedList::FlightNode* indirectLeg = indirectCurr->route.legs.getHead();
                            while (indirectLeg) 
                            {
                                if (strcmp(indirectLeg->flight.origin, flightGraph.getCityName(currentCityIndex)) == 0 &&
    strcmp(indirectLeg->flight.destination, flightGraph.getCityName(nextCityIndex)) == 0 &&
    strcmp(indirectLeg->flight.date, flight->date) == 0 &&
    strcmp(indirectLeg->flight.departureTime, flight->departureTime) == 0 &&
    strcmp(indirectLeg->flight.arrivalTime, flight->arrivalTime) == 0)
                                {
                                    std::cout << "\tIndirect Route Index: " << indirectIndex << "\n";
                                    foundIndirect = true;
                                    break;
                                }
                                indirectLeg = indirectLeg->next;
                            }
                            indirectCurr = indirectCurr->next;
                            indirectIndex++;
                        }

                        if (!foundDirect && !foundIndirect)
                        {
                            std::cout << "\tNo direct or indirect match found.\n";
                        }
                    }
                    std::cout << "\n";
                    break;
                }
                edge = edge->next;
            }
        }
    } 
    else 
    {
        std::cout << "\033[1;31mNo path found between " << originCity << " and " << destinationCity
                  << " within the given date range.\033[0m\n";
    }

    delete[] bestPath;
}

void Route::cheapestFlight(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, LinkedList& directFlights, RouteList& indirectRoutes)
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
             
             // Compare with direct flights
                    bool foundDirect = false;
                    LinkedList::FlightNode* directCurr = directFlights.getHead();
                    int index = 0;
                    while (directCurr) 
                    {
                        if (strcmp(directCurr->flight.origin, flightGraph.getCityName(currentCityIndex)) == 0 &&
                            strcmp(directCurr->flight.destination, flightGraph.getCityName(nextCityIndex)) == 0 &&
                            strcmp(directCurr->flight.date, flight->date) == 0 &&
                            strcmp(directCurr->flight.departureTime, flight->departureTime) == 0 &&
                            strcmp(directCurr->flight.arrivalTime, flight->arrivalTime) == 0)
                        {
                            std::cout << "\tDirect Flight Index: " << index << "\n";
                            foundDirect = true;
                            break;
                        }
                        directCurr = directCurr->next;
                        index++;
                    }
                    
                    if (!foundDirect)
                    {
                        // Compare with indirect routes
                        bool foundIndirect = false;
                        RouteList::RouteNode* indirectCurr = indirectRoutes.getHead();
                        int indirectIndex = 0;
                        while (indirectCurr) 
                        {
                            LinkedList::FlightNode* indirectLeg = indirectCurr->route.legs.getHead();
                            while (indirectLeg) 
                            {
                                if (strcmp(indirectLeg->flight.origin, flightGraph.getCityName(currentCityIndex)) == 0 &&
                                    strcmp(indirectLeg->flight.destination, flightGraph.getCityName(nextCityIndex)) == 0 &&
                                    strcmp(indirectLeg->flight.date, flight->date) == 0 &&
                                    strcmp(indirectLeg->flight.departureTime, flight->departureTime) == 0 &&
                                    strcmp(indirectLeg->flight.arrivalTime, flight->arrivalTime) == 0)
                                {
                                    std::cout << "\tIndirect Route Index: " << indirectIndex << "\n";
                                    foundIndirect = true;
                                    break;
                                }
                                indirectLeg = indirectLeg->next;
                            }
                            indirectCurr = indirectCurr->next;
                            indirectIndex++;
                        }

                        if (!foundDirect && !foundIndirect)
                        {
                            std::cout << "\tNo direct or indirect match found.\n";
                        }
                    }
                    std::cout << "\n";
                    break;
                }
                edge = edge->next;
            }
        }
    } 
    else 
    {
        std::cout << "\033[1;31mNo path found between " << originCity << " and " << destinationCity
                  << " within the given date range.\033[0m\n";
    }

    delete[] bestPath;
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

LinkedList Route::listDirectFlightsWithinDataRangeandPreferredAirline(
    const char* originCity, 
    const char* destinationCity, 
    const char* startDate, 
    const char* endDate, 
    string airline)
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

    // Check direct flights only
    Edge* edge = flightGraph.getVertices()[originIndex].head;

    while (edge) 
    { 
        Flight* flightData = edge->flightData;

        // Check if the edge leads directly to the destination city
        // and matches date range and airline
        if (edge->destination == destinationIndex && 
            isWithinDateRange(flightData->date, startDate, endDate) && 
            flightData->airline == airline) 
        {
            foundFlights = true;
            Flight flight(
                originCity, destinationCity, flightData->airline, flightData->date, 
                flightData->departureTime, flightData->arrivalTime, 
                flightData->price, flightData->distance
            );
            preferredFlights.insert(flight);
        }

        edge = edge->next;
    }

    if (!foundFlights) 
    {
        cout << "\t\033[1;31mNo direct flights available from " << originCity << " to " << destinationCity 
             << " with the preferred airline \"" << airline << "\".\033[0m\n";
    }

    return preferredFlights;
}

RouteList Route::filterMultiLegRoutesByAirline(RouteList& allRoutes, const string& airline) 
{
    RouteList filteredRoutes;

    RouteList::RouteNode* current = allRoutes.getHead();
    while (current) {
        LinkedList::FlightNode* leg = current->route.legs.getHead();
        bool validRoute = true;

        while (leg) {
            if (leg->flight.airline != airline) {
                validRoute = false;
                break;
            }
            leg = leg->next;
        }

        if (validRoute) {
            filteredRoutes.insertRoute(current->route);
        }

        current = current->next;
    }

    if (filteredRoutes.hasRoutes()) {
        cout << "\033[1;36m\nFiltered Routes with Airline \"" << airline << "\"\033[0m\n";
        filteredRoutes.Display();
    } else {
        cout << "\033[1;31mNo routes found with the airline \"" << airline << "\".\033[0m\n";
    }

    return filteredRoutes;
}

RouteList Route::filterByTransitCities(RouteList& allRoutes, string* transitCities, int numberOfTransitCities) 
{
    RouteList filteredRoutes;

    RouteList::RouteNode* current = allRoutes.getHead();
    while (current) {
        LinkedList::FlightNode* leg = current->route.legs.getHead();
        int transitIndex = 0;
        bool validRoute = true;

        while (leg && transitIndex < numberOfTransitCities) {
            if (strcmp(leg->flight.destination, transitCities[transitIndex].c_str()) == 0) {
                transitIndex++;
            }
            leg = leg->next;
        }

        if (transitIndex < numberOfTransitCities) {
            validRoute = false;
        }

        if (validRoute) {
            filteredRoutes.insertRoute(current->route);
        }

        current = current->next;
    }

    if (filteredRoutes.hasRoutes()) {
        cout << "\033[1;36m\nFiltered Routes with Transit Cities\033[0m\n";
        filteredRoutes.Display();
    } else {
        cout << "\033[1;31mNo routes found with the specified transit cities.\033[0m\n";
    }

    return filteredRoutes;
}

RouteList Route::filterByTransitCitiesAndAirline(RouteList& allRoutes, const string& airline, string* transitCities, int numberOfTransitCities) 
{
    RouteList filteredRoutes;

    RouteList::RouteNode* current = allRoutes.getHead();
    while (current) {
        LinkedList::FlightNode* leg = current->route.legs.getHead();
        bool validRoute = true;

        // Check if the route has the desired airline
        while (leg) {
            if (leg->flight.airline != airline) {
                validRoute = false;
                break;
            }
            leg = leg->next;
        }

        // Check if the route has the desired transit cities
        if (validRoute) {
            leg = current->route.legs.getHead();
            int transitIndex = 0;
            while (leg && transitIndex < numberOfTransitCities) {
                if (strcmp(leg->flight.destination, transitCities[transitIndex].c_str()) == 0) {
                    transitIndex++;
                }
                leg = leg->next;
            }

            if (transitIndex < numberOfTransitCities) {
                validRoute = false;
            }
        }

        if (validRoute) {
            filteredRoutes.insertRoute(current->route);
        }

        current = current->next;
    }

    if (filteredRoutes.hasRoutes()) {
        cout << "\033[1;36m\nFiltered Routes with Airline \"" << airline << "\" and Transit Cities\033[0m\n";
        filteredRoutes.Display();
    } else {
        cout << "\033[1;31mNo routes found with the airline \"" << airline << "\" and transit cities.\033[0m\n";
    }

    return filteredRoutes;
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
    if (currentIndex == destinationIndex) {
        int legCount = currentRoute.size();
        
        // Skip single-leg routes if desired:
        if (legCount <= 1) {
            return; 
        }

        // Check for duplicates before inserting:
        if (!isDuplicateRoute(allRoutes, currentRoute)) {
            // Convert currentRoute to a FlightRoute and add it
            FlightRoute newRoute;
            LinkedList::FlightNode* node = currentRoute.getHead();
            while (node) {
                newRoute.legs.insert(node->flight);
                node = node->next;
            }
            allRoutes.insertRoute(newRoute);
        }
        return;
    }

    visited[currentIndex] = true;
    Edge* edge = flightGraph.getVertices()[currentIndex].head;
    Flight* lastFlight = currentRoute.getLastFlight();
    const char* lastArrivalTime = lastFlight ? lastFlight->arrivalTime : NULL;

    while (edge) {
        int nextCity = edge->destination;
        if (!visited[nextCity]) {
            if (isWithinDateRange(edge->flightData->date, startDate, endDate)) {
                // Check time feasibility
                if (lastArrivalTime == NULL || compareTimes(edge->flightData->departureTime, lastArrivalTime) >= 0) {
                    // Insert this flight into currentRoute
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

                    currentRoute.insert(nextFlight);

                    // Recurse
                    dfsBuildRoutes(nextCity, destinationIndex, startDate, endDate, visited, currentRoute, allRoutes);

                    // Backtrack
                    currentRoute.removeLast();
                }
            }
        }
        edge = edge->next;
    }

    visited[currentIndex] = false;
}

void Route::removeDuplicates(RouteList &allRoutes) {
    RouteList uniqueRoutes;
    RouteList::RouteNode* curr = allRoutes.getHead();
    while (curr) {
        if (!isDuplicateRoute(uniqueRoutes, curr->route.legs)) {
            uniqueRoutes.insertRoute(curr->route);
        }
        curr = curr->next;
    }
    allRoutes.clear();
    allRoutes = uniqueRoutes;
}

bool Route::isDuplicateRoute(RouteList &allRoutes, LinkedList &candidate) {
    int candidateSize = candidate.size();

    RouteList::RouteNode* curr = allRoutes.getHead();
    while (curr) {
        // Check if route sizes match
        int existingSize = curr->route.legs.size();
        if (existingSize == candidateSize) {
            if (areRoutesIdentical(curr->route.legs, candidate)) {
                return true; // Found a duplicate
            }
        }
        curr = curr->next;
    }
    return false;
}

bool Route::areRoutesIdentical(LinkedList &routeA, LinkedList &routeB) {
    LinkedList::FlightNode* aNode = routeA.getHead();
    LinkedList::FlightNode* bNode = routeB.getHead();

    while (aNode && bNode) {
        Flight &fA = aNode->flight;
        Flight &fB = bNode->flight;

        // Compare only certain fields (like origin, destination, airline):
        if (strcmp(fA.origin, fB.origin) != 0 ||
            strcmp(fA.destination, fB.destination) != 0 ||
            strcmp(fA.airline, fB.airline) != 0) {
            return false;
        }

        // If you want exact match on date and time, keep these checks,
        // otherwise remove or relax them.
        // if (strcmp(fA.date, fB.date) != 0) return false;
        // if (strcmp(fA.departureTime, fB.departureTime) != 0) return false;
        // if (strcmp(fA.arrivalTime, fB.arrivalTime) != 0) return false;

        // Similarly for price and distance:
        // if (fA.price != fB.price) return false;
        // if (fA.distance != fB.distance) return false;

        aNode = aNode->next;
        bNode = bNode->next;
    }

    // Both routes must be of the same length
    return (aNode == NULL && bNode == NULL);
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
