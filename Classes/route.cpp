#include "route.h"
#include "menu.h"

// Assuming Route is defined and used elsewhere as in your provided code.

Route::Route(FlightGraph& graph)
    : flightGraph(graph) {}


//struct for shortest path
struct QueueElement 
{
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


//helper function to initialize arrays
void Route::initializeArrays(int* distances, int* previous, bool* visited, int size) 
{
    for (int i = 0; i < size; i++) 
    {
        distances[i] = INT_MAX;
        previous[i] = -1;
        visited[i] = false;
    }
}

//helper function to find min distance index
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


// Shortest path function
void Route::shortestPath(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, LinkedList& directFlights, RouteList& indirectRoutes)
{
    const int MAX_CITIES = flightGraph.getNumVertices();
    int originIndex = flightGraph.getCityIndex(originCity);
    int destinationIndex = flightGraph.getCityIndex(destinationCity);

    if (originIndex == -1 || destinationIndex == -1) 
    {
        cout << "\033[1;31mInvalid city name(s): " << originCity << " or " << destinationCity << "\033[0m\n";
        return;
    }

    struct QueueElement 
    {
        int totalTime;
        int cityIndex;
        time_t lastArrivalEpoch;
        int* path;
        int pathLength;

        QueueElement(int maxCities) 
        {
            path = new int[maxCities];
            pathLength = 0;
            lastArrivalEpoch = 0; // initialize at origin
            totalTime = 0;
        }

        ~QueueElement() { delete[] path; }
    };

    QueueElement* priorityQueue[MAX_CITIES];
    int queueSize = 0;
    int minTime[MAX_CITIES];
    for (int i = 0; i < MAX_CITIES; ++i) 
        minTime[i] = INT_MAX;

    // Initialize with origin
    priorityQueue[queueSize++] = new QueueElement(MAX_CITIES);
    priorityQueue[0]->totalTime = 0;
    priorityQueue[0]->cityIndex = originIndex;
    priorityQueue[0]->path[0] = originIndex;
    priorityQueue[0]->pathLength = 1;
    priorityQueue[0]->lastArrivalEpoch = 0; // Starting point with no arrival time (0 means start)

    int* bestPath = new int[MAX_CITIES];
    int bestPathLength = 0;

    while (queueSize > 0) 
    {
        // Find element with smallest totalTime
        int minIndex = 0;
        for (int i = 1; i < queueSize; ++i) {
            if (priorityQueue[i]->totalTime < priorityQueue[minIndex]->totalTime)
            {
                minIndex = i;
            }
        }

        QueueElement* current = priorityQueue[minIndex];
        queueSize--;
        for (int i = minIndex; i < queueSize; ++i) 
        {
            priorityQueue[i] = priorityQueue[i + 1];
        }

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

        Edge* edge = flightGraph.getVertices()[current->cityIndex].head;
        while (edge) {
            Flight* flight = edge->flightData;
            // Date range check
            if (!isWithinDateRange(flight->date, startDate, endDate))
            {
                edge = edge->next;
                continue;
            }

            std::tm depTime = {}, arrTime = {};
            {
                // Format: DD/MM/YYYY and HH:MM
                // Convert to "YYYY-MM-DD HH:MM" first
                string formattedDate = convertDDMMYYYYToYYYYMMDD(flight->date);
                string dep = formattedDate + " " + flight->departureTime;
                string arr = formattedDate + " " + flight->arrivalTime;

                strptime(dep.c_str(), "%Y-%m-%d %H:%M", &depTime);
                strptime(arr.c_str(), "%Y-%m-%d %H:%M", &arrTime);
            }

            time_t depEpoch = mktime(&depTime);
            time_t arrEpoch = mktime(&arrTime);
            if (current->lastArrivalEpoch > 0 && depEpoch < current->lastArrivalEpoch) 
            {
                // Flight departs before last arrival, skip
                edge = edge->next;
                continue;
            }

            int flightDurationMins = (int)difftime(arrEpoch, depEpoch) / 60;
            int newTotalTime = current->totalTime + flightDurationMins;

            if (newTotalTime < minTime[edge->destination]) 
            {
                minTime[edge->destination] = newTotalTime;
                QueueElement* newElement = new QueueElement(MAX_CITIES);
                newElement->totalTime = newTotalTime;
                newElement->cityIndex = edge->destination;
                newElement->lastArrivalEpoch = arrEpoch; // Store absolute arrival time
                newElement->pathLength = current->pathLength;
                for (int i = 0; i < current->pathLength; ++i) {
                    newElement->path[i] = current->path[i];
                }
                newElement->path[newElement->pathLength++] = edge->destination;
                priorityQueue[queueSize++] = newElement;
            }

            edge = edge->next;
        }

        delete current;
    }

    if (bestPathLength > 0) 
    {
        cout << "\033[1;36;1mShortest Path Found:\033[0m\n"; // Bold & Cyan

        // Variable to store the indirect index found (if any)
        int foundIndirectIndex = -1;

        for (int i = 0; i < bestPathLength - 1; ++i) 
        {
            int currentCityIndex = bestPath[i];
            int nextCityIndex = bestPath[i + 1];

            Edge* edge = flightGraph.getVertices()[currentCityIndex].head;
            while (edge) 
            {
                Flight* flight = edge->flightData;
                if (strcmp(flight->destination, flightGraph.getCityName(nextCityIndex)) == 0)
                {
                    cout << "\n\033[1;34mFlight Segment:\033[0m\n";
                    cout << "From: " << flightGraph.getCityName(currentCityIndex) << " → " 
                            << flightGraph.getCityName(nextCityIndex) << "\n"
                            << "Departure: " << flight->departureTime << " | Arrival: " << flight->arrivalTime << "\n"
                            << "Date: " << flight->date << ", Airline: " << flight->airline << ", Price: " << flight->price << " USD\n";

                    // Check direct flights
                    bool foundDirect = false;
                    LinkedList::FlightNode* directCurr = directFlights.getHead();
                    LinkedList::FlightNode* lastIndirectLeg = nullptr;
                    int index = 0;
                    while (directCurr) 
                    {
                        if (strcmp(directCurr->flight.origin, flightGraph.getCityName(currentCityIndex)) == 0 &&
                            strcmp(directCurr->flight.destination, flightGraph.getCityName(nextCityIndex)) == 0 &&
                            strcmp(directCurr->flight.date, flight->date) == 0 &&
                            strcmp(directCurr->flight.departureTime, flight->departureTime) == 0 &&
                            strcmp(directCurr->flight.arrivalTime, flight->arrivalTime) == 0) 
                        {
                            directCurr->flight.shortest = true;
                            
                            cout << "\033[1;33m\t→ Direct Route Index:\033[0m " << index << "\n";
                            
                            foundDirect = true;
                            break;
                        }
                        directCurr = directCurr->next;
                        index++;
                    }

                    if (!foundDirect) 
                    {
                        bool foundIndirect = false;
                        RouteList::RouteNode* indirectCurr = indirectRoutes.getHead();
                        int indirectIndex = 0;

                        while (indirectCurr && !foundIndirect) {
                            LinkedList::FlightNode* indirectLeg = indirectCurr->route.legs.getHead();

                            while (indirectLeg) {
                                if (strcmp(indirectLeg->flight.origin, flightGraph.getCityName(currentCityIndex)) == 0 &&
                                    strcmp(indirectLeg->flight.destination, flightGraph.getCityName(nextCityIndex)) == 0 &&
                                    strcmp(indirectLeg->flight.date, flight->date) == 0 &&
                                    strcmp(indirectLeg->flight.departureTime, flight->departureTime) == 0 &&
                                    strcmp(indirectLeg->flight.arrivalTime, flight->arrivalTime) == 0) {
                                    
                                    lastIndirectLeg = indirectLeg;
                                    foundIndirect = true;
                                    
                                    // Instead of printing here, store the index
                                    foundIndirectIndex = indirectIndex;
                                    break; // Only print first indirect match
                                }
                                indirectLeg = indirectLeg->next;
                            }

                            indirectCurr = indirectCurr->next;
                            indirectIndex++;
                        }
                    }

                    cout << "\n";
                    break;
                }
                edge = edge->next;
            }
        }

        // After finishing all segments, print the indirect route index once if found
        if (foundIndirectIndex != -1) 
        {
            RouteList::RouteNode* route = indirectRoutes.getRouteByIndex(foundIndirectIndex);
            route->shortest = true;
            cout << "\033[1;33m\t→ Indirect Route Index:\033[0m " << foundIndirectIndex << "\n";
        }

    } else {
        cout << "\033[1;31mNo path found between " << originCity << " and " << destinationCity
                << " within the given date range.\033[0m\n";
    }

    delete[] bestPath;
}

// Cheapest flight function
void Route::cheapestFlight(const char* originCity, const char* destinationCity, const char* startDate, const char* endDate, LinkedList& directFlights, RouteList& indirectRoutes)
{
    const int MAX_CITIES = flightGraph.getNumVertices();
    int originIndex = flightGraph.getCityIndex(originCity);
    int destinationIndex = flightGraph.getCityIndex(destinationCity);

    if (originIndex == -1 || destinationIndex == -1) 
    {
        cout << "\033[1;31mInvalid city name(s): " << originCity << " or " << destinationCity << "\033[0m\n";
        return;
    }

    struct QueueElement2 
    {
        int totalCost;
        int cityIndex;
        time_t lastArrivalEpoch;
        int* path;
        int pathLength;

        QueueElement2(int maxCities) 
        {
            path = new int[maxCities];
            pathLength = 0;
            totalCost = INT_MAX;
            lastArrivalEpoch = 0;
        }

        ~QueueElement2() { delete[] path; }
    };

    QueueElement2* priorityQueue[MAX_CITIES];
    int queueSize = 0;
    int minCost[MAX_CITIES];
    for (int i = 0; i < MAX_CITIES; ++i) 
        minCost[i] = INT_MAX;

    minCost[originIndex] = 0;
    priorityQueue[queueSize++] = new QueueElement2(MAX_CITIES);
    priorityQueue[0]->totalCost = 0;
    priorityQueue[0]->cityIndex = originIndex;
    priorityQueue[0]->path[0] = originIndex;
    priorityQueue[0]->pathLength = 1;
    priorityQueue[0]->lastArrivalEpoch = 0;

    int* bestPath = new int[MAX_CITIES];
    int bestPathLength = 0;

    while (queueSize > 0) 
    {
        int minIndex = 0;
        for (int i = 1; i < queueSize; ++i) 
        {
            if (priorityQueue[i]->totalCost < priorityQueue[minIndex]->totalCost) 
            {
                minIndex = i;
            }
        }

        QueueElement2* current = priorityQueue[minIndex];
        queueSize--;
        for (int i = minIndex; i < queueSize; ++i) 
        {
            priorityQueue[i] = priorityQueue[i + 1];
        }

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

        Edge* edge = flightGraph.getVertices()[current->cityIndex].head;
        while (edge) 
        {
            Flight* flight = edge->flightData;
            if (!isWithinDateRange(flight->date, startDate, endDate))
             {
                edge = edge->next;
                continue;
            }

            // Parse departure/arrival times into epochs
            std::tm depTime = {}, arrTime = {};
            {
                string formattedDate = convertDDMMYYYYToYYYYMMDD(flight->date);
                string dep = formattedDate + " " + flight->departureTime;
                string arr = formattedDate + " " + flight->arrivalTime;

                strptime(dep.c_str(), "%Y-%m-%d %H:%M", &depTime);
                strptime(arr.c_str(), "%Y-%m-%d %H:%M", &arrTime);
            }

            time_t depEpoch = mktime(&depTime);
            time_t arrEpoch = mktime(&arrTime);

            if (current->lastArrivalEpoch > 0 && depEpoch < current->lastArrivalEpoch) 
            {
                // Flight departs too early
                edge = edge->next;
                continue;
            }

            int newTotalCost = current->totalCost + flight->price;

            if (newTotalCost < minCost[edge->destination]) 
            {
                minCost[edge->destination] = newTotalCost;
                QueueElement2* newElement = new QueueElement2(MAX_CITIES);
                newElement->totalCost = newTotalCost;
                newElement->cityIndex = edge->destination;
                newElement->lastArrivalEpoch = arrEpoch; // Store arrival epoch
                newElement->pathLength = current->pathLength;
                for (int i = 0; i < current->pathLength; ++i) {
                    newElement->path[i] = current->path[i];
                }
                newElement->path[newElement->pathLength++] = edge->destination;
                priorityQueue[queueSize++] = newElement;
            }

            edge = edge->next;
        }

        delete current;
    }

if (bestPathLength > 0) 
{
        cout << "\033[1;36;1mCheapest Flight Found:\033[0m\n"; // Bold & Cyan

        // Variable to store the indirect index found (if any)
        int foundIndirectIndex = -1;

        for (int i = 0; i < bestPathLength - 1; ++i)
        {
            int currentCityIndex = bestPath[i];
            int nextCityIndex = bestPath[i + 1];

            Edge* edge = flightGraph.getVertices()[currentCityIndex].head;
            while (edge) 
            {
                Flight* flight = edge->flightData;
                if (strcmp(flight->destination, flightGraph.getCityName(nextCityIndex)) == 0) 
                {
                    cout << "\n\033[1;34mCheapest Flight Segment:\033[0m\n";
                    cout << "From: " << flightGraph.getCityName(currentCityIndex) << " → " 
                            << flightGraph.getCityName(nextCityIndex) << "\n"
                            << "Departure: " << flight->departureTime << " | Arrival: " << flight->arrivalTime << "\n"
                            << "Date: " << flight->date << ", Airline: " << flight->airline 
                            << ", Price: $" << flight->price << ", Distance: " << flight->distance << " km\n";

                    // Check direct flights
                    bool foundDirect = false;
                    LinkedList::FlightNode* directCurr = directFlights.getHead();
                    LinkedList::FlightNode* lastIndirectLeg = nullptr; // Track the last leg
                    int index = 0;
                    while (directCurr) 
                    {
                        if (strcmp(directCurr->flight.origin, flightGraph.getCityName(currentCityIndex)) == 0 &&
                            strcmp(directCurr->flight.destination, flightGraph.getCityName(nextCityIndex)) == 0 &&
                            strcmp(directCurr->flight.date, flight->date) == 0 &&
                            strcmp(directCurr->flight.departureTime, flight->departureTime) == 0 &&
                            strcmp(directCurr->flight.arrivalTime, flight->arrivalTime) == 0) 
                            {
                            directCurr->flight.cheapest = true;
                    
                            cout << "\033[1;33m\t→ Direct Flight Index:\033[0m " << index << "\n";
                            
                            foundDirect = true;
                            break;
                        }
                        directCurr = directCurr->next;
                        index++;
                    }

                    // If not direct, check indirect routes (store index once)
                    if (!foundDirect) 
                    {
                        bool foundIndirect = false;
                        RouteList::RouteNode* indirectCurr = indirectRoutes.getHead();
                        int indirectIndex = 0;
                        while (indirectCurr && !foundIndirect) {
                            LinkedList::FlightNode* indirectLeg = indirectCurr->route.legs.getHead();
                            
                            while (indirectLeg)
                            {
                                if (strcmp(indirectLeg->flight.origin, flightGraph.getCityName(currentCityIndex)) == 0 &&
                                    strcmp(indirectLeg->flight.destination, flightGraph.getCityName(nextCityIndex)) == 0 &&
                                    strcmp(indirectLeg->flight.date, flight->date) == 0 &&
                                    strcmp(indirectLeg->flight.departureTime, flight->departureTime) == 0 &&
                                    strcmp(indirectLeg->flight.arrivalTime, flight->arrivalTime) == 0) 
                                    {
                                    
                                    lastIndirectLeg = indirectLeg;
                                    foundIndirect = true;
                                    foundIndirectIndex = indirectIndex; // Store the indirect index
                                    break; // Only print first indirect match
                                }
                                indirectLeg = indirectLeg->next;
                            }
                            indirectCurr = indirectCurr->next;
                            indirectIndex++;
                        }
                    }

                    cout << "\n";
                    break;
                }
                edge = edge->next;
            }
        }

        // After finishing all segments, print the indirect route index once if found
        if (foundIndirectIndex != -1) 
        {
            RouteList::RouteNode* route = indirectRoutes.getRouteByIndex(foundIndirectIndex);
            route->cheapest = true;
            cout << "\033[1;33m\t→ Indirect Route Index:\033[0m " << foundIndirectIndex << "\n";
        }

    } 
    else 
    {
        cout << "\033[1;31mNo path found between " << originCity << " and " << destinationCity
                  << " within the given date range.\033[0m\n";
    }

    delete[] bestPath;
}


//List all direct flights within date range 
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
        string sDate = startDate;
        string eDate = endDate;
        sDate.erase(remove_if(sDate.begin(), sDate.end(), ::isspace), sDate.end());
        eDate.erase(remove_if(eDate.begin(), eDate.end(), ::isspace), eDate.end());
        if (edge->destination == destinationIndex && isWithinDateRange(edge->flightData->date, sDate.c_str(), eDate.c_str())) 
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
            string sDate = startDate;
            string eDate = endDate;
            sDate.erase(remove_if(sDate.begin(), sDate.end(), ::isspace), sDate.end());
            eDate.erase(remove_if(eDate.begin(), eDate.end(), ::isspace), eDate.end());
            if (intermediateEdge->destination == destinationIndex && 
                isWithinDateRange(edge->flightData->date, sDate.c_str(), eDate.c_str()) && 
                isWithinDateRange(intermediateEdge->flightData->date, sDate.c_str(), eDate.c_str())) 
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

//List all direct flights within date range and preferred airline
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
        string sDate = startDate;
        string eDate = endDate;
        sDate.erase(remove_if(sDate.begin(), sDate.end(), ::isspace), sDate.end());
        eDate.erase(remove_if(eDate.begin(), eDate.end(), ::isspace), eDate.end());
        if (edge->destination == destinationIndex && 
            isWithinDateRange(flightData->date, sDate.c_str(), eDate.c_str()) && 
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

    cout << "\n\t\033[1;36mDirect Flights with Airline \"" << airline << "\"\033[0m\n";
    preferredFlights.Display();

    return preferredFlights;
}

//fillter the multiple leg routes by airline
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

//filter the multiple leg routes by transit cities
RouteList Route::filterByTransitCities(RouteList& allRoutes, string* transitCities, int numberOfTransitCities) 
{
    RouteList filteredRoutes;

    RouteList::RouteNode* current = allRoutes.getHead();
    while (current) {
        LinkedList::FlightNode* leg = current->route.legs.getHead();
        int transitIndex = 0;
        bool validRoute = true;

        while (leg && transitIndex < numberOfTransitCities) 
        {
            if (strcmp(leg->flight.destination, transitCities[transitIndex].c_str()) == 0) {
                transitIndex++;
            }
            leg = leg->next;
        }

        if (transitIndex < numberOfTransitCities) 
        {
            validRoute = false;
        }

        if (validRoute) 
        {
            filteredRoutes.insertRoute(current->route);
        }

        current = current->next;
    }

    if (filteredRoutes.hasRoutes()) 
    {
        cout << "\033[1;36m\nFiltered Routes with Transit Cities\033[0m\n";
        filteredRoutes.Display();
    } 
    else 
    {
        cout << "\033[1;31mNo routes found with the specified transit cities.\033[0m\n";
    }

    return filteredRoutes;
}

//filter the multiple leg routes by transit cities and airline
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

//list all indirect routes within date range
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

// Depth-first search to build all indirect routes
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

    // Store the last flight's date and arrival time to ensure chronological progress
    int lastDateComparable = -1;
    const char* lastArrivalTime = NULL;
    if (lastFlight) {
        lastDateComparable = convertDateToComparableFormat(lastFlight->date);
        lastArrivalTime = lastFlight->arrivalTime;
    }

    while (edge) {
        int nextCity = edge->destination;
        if (!visited[nextCity]) {
            Flight* nextFlightData = edge->flightData;

            // Check if next flight's date is within the given range
            if (isWithinDateRange(nextFlightData->date, startDate, endDate)) {
                // Chronological checks:
                int nextDateComparable = convertDateToComparableFormat(nextFlightData->date);

                // If we have a last flight, ensure dates and times make sense
                if (lastFlight) {
                    // If the next flight date is before the last flight date, skip
                    if (nextDateComparable < lastDateComparable) {
                        edge = edge->next;
                        continue;
                    }
                    // If same date, ensure departure is after arrival
                    if (nextDateComparable == lastDateComparable) {
                        if (compareTimes(nextFlightData->departureTime, lastArrivalTime) < 0) {
                            edge = edge->next;
                            continue;
                        }
                    }
                    // If nextDateComparable > lastDateComparable, it's a later date, so it's fine
                }

                // Passed chronological checks, insert this flight and recurse
                Flight nextFlight(
                    flightGraph.getCityName(currentIndex),
                    flightGraph.getCityName(nextCity),
                    nextFlightData->airline,
                    nextFlightData->date,
                    nextFlightData->departureTime,
                    nextFlightData->arrivalTime,
                    nextFlightData->price,
                    nextFlightData->distance
                );

                currentRoute.insert(nextFlight);

                // Recurse deeper
                dfsBuildRoutes(nextCity, destinationIndex, startDate, endDate, visited, currentRoute, allRoutes);

                // Backtrack
                currentRoute.removeLast();
            }
        }
        edge = edge->next;
    }

    visited[currentIndex] = false;
}

// Remove duplicate routes from the list
void Route::removeDuplicates(RouteList &allRoutes) 
{
    RouteList uniqueRoutes;
    RouteList::RouteNode* curr = allRoutes.getHead();
    while (curr) {
        if (!isDuplicateRoute(uniqueRoutes, curr->route.legs)) 
        {
            uniqueRoutes.insertRoute(curr->route);
        }
        curr = curr->next;
    }
    allRoutes.clear();
    allRoutes = uniqueRoutes;
}


//to check for duplicate routes
bool Route::isDuplicateRoute(RouteList &allRoutes, LinkedList &candidate) 
{
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

//for identical routes
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


        aNode = aNode->next;
        bNode = bNode->next;
    }

    // Both routes must be of the same length
    return (aNode == NULL && bNode == NULL);
}

//compare times 
int Route::compareTimes(const char* time1, const char* time2) 
{
    // Convert HH:MM to integer minutes and compare
    int h1, m1, h2, m2;
    sscanf(time1, "%d:%d", &h1, &m1);
    sscanf(time2, "%d:%d", &h2, &m2);
    int total1 = h1*60+m1;
    int total2 = h2*60+m2;
    if (total1 == total2) return 0;
    return (total1 > total2) ? 1 : -1;
}


//convert date to comparable format
int Route::convertDateToComparableFormat(const char* date)
{
    // Trim leading/trailing spaces
    std::string dateStr(date);
    dateStr.erase(std::remove_if(dateStr.begin(), dateStr.end(), ::isspace), dateStr.end());

    int day, month, year;
    char delimiter1, delimiter2;

    stringstream dateStream(dateStr);
    if (!(dateStream >> day >> delimiter1 >> month >> delimiter2 >> year) 
        || delimiter1 != '/' || delimiter2 != '/') {
        // If parsing fails or format is not DD/MM/YYYY, return something large so it's out of range
        return INT_MAX;
    }

    // Convert date into an integer like YYYYMMDD
    return year * 10000 + month * 100 + day;
}


//toc ehck with in date range
bool Route::isWithinDateRange(const char* flightDate, const char* startDate, const char* endDate)
{
    int flight = convertDateToComparableFormat(flightDate);
    int start = convertDateToComparableFormat(startDate);
    int end = convertDateToComparableFormat(endDate);

    return flight >= start && flight <= end;
    
}

//convert date to int
int Route::convertDateToInt(const std::string &dateStr) 
{
    // dateStr format: DD/MM/YYYY
    // Extract day, month, year
    int day, month, year;
    char slash;
    stringstream ss(dateStr);
    ss >> day >> slash >> month >> slash >> year;
    // Convert to an integer of form YYYYMMDD for easy comparison
    return year * 10000 + month * 100 + day;
}

//check if date is in range
bool Route::isDateInRange(const std::string &flightDate, const std::string &fromDate, const std::string &toDate)
{
    int flightVal = convertDateToInt(flightDate);
    int startVal = convertDateToInt(fromDate);
    int endVal = convertDateToInt(toDate);

    return (flightVal >= startVal && flightVal <= endVal);
}

// Function to convert DD/MM/YYYY to YYYY-MM-DD
string Route::convertDDMMYYYYToYYYYMMDD(const char* dateStr) 
{
    int day, month, year;
    char slash;
    stringstream ss(dateStr);
    ss >> day >> slash >> month >> slash >> year;
    
    char buffer[11]; // "YYYY-MM-DD" + null terminator
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", year, month, day);
    return string(buffer);
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
