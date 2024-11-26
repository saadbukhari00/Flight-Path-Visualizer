#include "FlightGraph.h"

FlightGraph::FlightGraph(int size, FileHandling& fileHandler)
    : fileHandler(fileHandler), vertexCount(0), coordinateCount(0), maxVertices(size) {
    vertices = new Vertex[size];
    coordinates = new CityCoordinate[size];
    if (!mapTexture.loadFromFile("Assets/world_map.png")) 
    {
    std::cerr << "Error loading map texture.\n";
    }
    initializeCityCoordinates();
}

// Returns the list of vertices (cities)
Vertex* FlightGraph::getVertices() {
    return vertices;
}

int FlightGraph::getCityIndex(const char* cityName) const {
    for (int i = 0; i < vertexCount; i++) {
        if (strcasecmp(vertices[i].city, cityName) == 0) { // Use case-insensitive comparison
            return i;
        }
    }
    return -1;
}

// Returns the number of vertices (cities) in the graph
int FlightGraph::getNumVertices() const {
    return vertexCount;
}

void FlightGraph::addCity(const char* city, sf::Vector2f position) {
    for (int i = 0; i < coordinateCount; i++) {
        if (strcmp(coordinates[i].city, city) == 0)
            return; // City already added
    }
    strcpy(coordinates[coordinateCount].city, city);
    coordinates[coordinateCount].position = position;
    coordinateCount++;
}

void FlightGraph::initializeCityCoordinates() {
    // Ensure you populate the coordinates for each city
    if (coordinateCount > maxVertices) {
        std::cout << "Error: Too many coordinates for the graph to handle." << std::endl;
        return;
    }

    // Example: Initialize with city positions
    coordinates[0] = CityCoordinate(100.0f, 200.0f, "Islamabad");
    coordinates[1] = CityCoordinate(300.0f, 400.0f, "Newyork");
    coordinates[2] = CityCoordinate(150.0f, 250.0f, "Paris");
    coordinates[3] = CityCoordinate(350.0f, 450.0f, "Tokyo");
    coordinates[4] = CityCoordinate(400.0f, 150.0f, "London");
    coordinates[5] = CityCoordinate(200.0f, 300.0f, "Berlin");
    coordinates[6] = CityCoordinate(600.0f, 500.0f, "Singapore");
    coordinates[7] = CityCoordinate(500.0f, 100.0f, "Amsterdam");
    coordinates[8] = CityCoordinate(250.0f, 350.0f, "Sydney");
    coordinates[9] = CityCoordinate(450.0f, 550.0f, "Seoul");
    coordinates[10] = CityCoordinate(550.0f, 600.0f, "HongKong");

    coordinateCount = 11; // Adjust according to the number of cities initialized
}

sf::Vector2f FlightGraph::getCityPosition(const char* cityName) {
    for (int i = 0; i < vertexCount; i++) {
        if (strcmp(vertices[i].city, cityName) == 0) {
            return coordinates[i].position; // Use position directly
        }
    }
    std::cerr << "City not found: " << cityName << std::endl;
    return sf::Vector2f(-1, -1); // Return invalid position for debugging
}

int FlightGraph::findOrAddCity(const char* cityName) {
    // Search for the city
    for (int i = 0; i < vertexCount; i++) {
        if (strcmp(vertices[i].city, cityName) == 0) {
            return i; // City found
        }
    }

    // Add the city if not found
    if (vertexCount < maxVertices) {
        strcpy(vertices[vertexCount].city, cityName);
        vertices[vertexCount].head = nullptr;
        return vertexCount++;
    }

    cout << "Error: Maximum number of vertices reached.\n";
    return -1; // Error: Max vertices reached
}

void FlightGraph::addFlight(const char* origin, const char* destination, const char* airline, const char* date, const char* departureTime, const char* arrivalTime, int price, int distance) {
    int originIndex = findOrAddCity(origin);
    int destIndex = findOrAddCity(destination);

    // Check for duplicate flights
    Edge* current = vertices[originIndex].head;
    while (current) {
        if (strcmp(current->flightData->destination, destination) == 0 &&
            strcmp(current->flightData->airline, airline) == 0 &&
            strcmp(current->flightData->date, date) == 0)
            return; // Duplicate flight
        current = current->next;
    }

    // Create and add the flight
    Flight* newFlight = new Flight(origin, destination, airline, date, departureTime, arrivalTime, price, distance);
    Edge* newEdge = new Edge(destIndex, newFlight, vertices[originIndex].head);
    vertices[originIndex].head = newEdge;
}

void FlightGraph::populateGraph() {
    fileHandler.readFlightsFile();
    int flightCount = fileHandler.getFlightCount();

    for (int i = 0; i < flightCount; i++) {
        Flight* flight = fileHandler.getFlightByIndex(i);
        if (flight) {
            addFlight(flight->origin, flight->destination, flight->airline, flight->date,
                      flight->departureTime, flight->arrivalTime, flight->price, flight->distance);
        }
    }
}

void FlightGraph::displayGraph() const {
    for (int i = 0; i < vertexCount; i++) {
        if (vertices[i].city[0] == '\0') continue;

        std::cout << "City: " << vertices[i].city << " -> ";
        Edge* edge = vertices[i].head;
        while (edge) {
            std::cout << vertices[edge->destination].city
                      << " (Airline: " << edge->flightData->airline
                      << ", Date: " << edge->flightData->date
                      << ", Price: " << edge->flightData->price << "), "
                      << "Distance: " << edge->flightData->distance << " km, ";
            edge = edge->next;
        }
        std::cout << "\n";
    }
}

void FlightGraph::displayOnMap(sf::RenderWindow& window, const sf::Texture& mapTexture) {
    sf::Sprite map(mapTexture);
    window.draw(map);

    // Draw cities
    for (int i = 0; i < vertexCount; i++) {
        if (vertices[i].city[0] == '\0') continue;

        sf::CircleShape cityMarker(5);
        cityMarker.setFillColor(sf::Color::Red);
        cityMarker.setPosition(coordinates[i].position.x - 2.5, coordinates[i].position.y - 2.5);
        window.draw(cityMarker);
    }

    // Draw routes
    for (int i = 0; i < vertexCount; i++) {
        Edge* edge = vertices[i].head;
        while (edge) {
            int destIndex = edge->destination;
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(coordinates[i].position.x, coordinates[i].position.y), sf::Color::Blue),
                sf::Vertex(sf::Vector2f(coordinates[destIndex].position.x, coordinates[destIndex].position.y), sf::Color::Blue)
            };
            window.draw(line, 2, sf::Lines);
            edge = edge->next;
        }
    }
    window.display();
}

void FlightGraph::displayHighlightedRoutes(sf::RenderWindow& window) {
    
    for(int i = 0; i < vertexCount; i++)
    {
        if(vertices[i].city[0] == '\0') continue;
        sf::CircleShape cityMarker(5);
        cityMarker.setFillColor(sf::Color::Red);
        cityMarker.setPosition(coordinates[i].position.x - 2.5, coordinates[i].position.y - 2.5);
        window.draw(cityMarker);
    }
}

void FlightGraph::validateCity(const FlightGraph &graph, const char *city)
{
    int index = graph.getCityIndex(city);
    if (index == -1) {
        std::cerr << "City " << city << " not found in the graph.\n";
    }
}

FlightGraph::~FlightGraph() {
    for (int i = 0; i < vertexCount; i++) {
        Edge* edge = vertices[i].head;
        while (edge) {
            Edge* temp = edge;
            edge = edge->next;
            delete temp->flightData;
            delete temp;
        }
    }
    delete[] vertices;
    delete[] coordinates;
}