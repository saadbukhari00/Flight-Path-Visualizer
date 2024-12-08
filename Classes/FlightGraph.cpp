#include "FlightGraph.h"

FlightGraph::FlightGraph(int size, FileHandling& fileHandler)
    : fileHandler(fileHandler), vertexCount(0), coordinateCount(0), maxVertices(size) 
{
    vertices = new Vertex[size];
    coordinates = new CityCoordinate[120];
    if (!mapTexture.loadFromFile("Assets/world_map.png")) 
    {
    std::cerr << "Error loading map texture.\n";
    }
    initializeCityCoordinates();
}

const char* FlightGraph::getCityName(int cityIndex) const 
{
    if (cityIndex >= 0 && cityIndex < vertexCount) 
    {
        return vertices[cityIndex].city;
    } 
    else {
    
        cout<<"Invalid city index.";
        return 0;
    }
}



// Returns the list of vertices (cities)
Vertex* FlightGraph::getVertices() 
{
    return vertices;
}

// Returns the index of a city in the graph, or -1 if not found
int FlightGraph::getCityIndex(const char* cityName) 
{
    for (int i = 0; i < vertexCount; i++) {
        if (strcmp(vertices[i].city, cityName) == 0) 
        {
            return i;
        }
    }
    return -1;
}

// Returns the number of vertices (cities) in the graph
int FlightGraph::getNumVertices() const 
{
    return vertexCount;
}

void FlightGraph::addCity(const char* city, sf::Vector2f position) 
{
    for (int i = 0; i < coordinateCount; i++) {
        if (strcmp(coordinates[i].city, city) == 0)
            return; // City already added
    }
    strcpy(coordinates[coordinateCount].city, city);
    coordinates[coordinateCount].position = position;
    coordinateCount++;
}

void FlightGraph::initializeCityCoordinates() 
{
    addCity("Islamabad", sf::Vector2f(0.74f, 0.45f));
    addCity("Newyork", sf::Vector2f(0.35f, 0.35f));
    addCity("Paris", sf::Vector2f(0.53f, 0.38f));
    addCity("London", sf::Vector2f(0.51f, 0.36f));
    addCity("Berlin", sf::Vector2f(0.55f, 0.35f));
    addCity("Seoul", sf::Vector2f(0.90f, 0.45f));
    addCity("Amsterdam", sf::Vector2f(0.53f, 0.36f));
    addCity("Singapore", sf::Vector2f(0.84f, 0.64f));
    addCity("Tokyo", sf::Vector2f(0.95f, 0.45f));
    addCity("Hongkong", sf::Vector2f(0.87f, 0.53f));
    addCity("Sydney", sf::Vector2f(0.98f, 0.84f));
}

sf::Vector2f FlightGraph::getCityPosition(const std::string& city) 
{
    for (int i = 0; i < coordinateCount; i++) 
    {
        if (strcmp(coordinates[i].city, city.c_str()) == 0) {
            return coordinates[i].position;
        }
    }
    return sf::Vector2f(0, 0); // Return a default position if city is not found
}

// Find a city in the graph, or add it if not found
int FlightGraph::findOrAddCity(const char* cityName) 
{
    // Search for the city
    for (int i = 0; i < vertexCount; i++) {
        if (strcmp(vertices[i].city, cityName) == 0) 
        {
            return i; // City found
        }
    }

    // Add the city if not found
    if (vertexCount < maxVertices) 
    {
        strcpy(vertices[vertexCount].city, cityName);
        vertices[vertexCount].head = nullptr;
        return vertexCount++;
    }

    cout << "Error: Maximum number of vertices reached.\n";
    return -1; // Error: Max vertices reached
}

// Add a flight to the graph
void FlightGraph::addFlight(const char* origin, const char* destination, const char* airline, const char* date,const char* departureTime, const char* arrivalTime, int price, int distance) 
{
    int originIndex = findOrAddCity(origin);
    int destIndex = findOrAddCity(destination);

    // Check for duplicate flights
    Edge* current = vertices[originIndex].head;
    while (current) 
    {
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

//Check for duplicate flights
bool FlightGraph::isDuplicateFlight(const char* origin, const char* destination, const char* airline, const char* date) 
{
    int originIndex = getCityIndex(origin);
    if (originIndex == -1) return false;

    Edge* current = vertices[originIndex].head;
    while (current) {
        if (strcmp(current->flightData->destination, destination) == 0 &&
            strcmp(current->flightData->airline, airline) == 0 &&
            strcmp(current->flightData->date, date) == 0) {
            return true; // Duplicate flight found
        }
        current = current->next;
    }
    return false; // No duplicate flight found
}

//To fill the graph
void FlightGraph::populateGraph() 
{
    fileHandler.readFlightsFile();
    int flightCount = fileHandler.getFlightCount();

    for (int i = 0; i < flightCount; i++) 
    {
        Flight* flight = fileHandler.getFlightByIndex(i);

        if(flight && !isDuplicateFlight(flight->origin, flight->destination, flight->airline, flight->date)) 
        {
            addFlight(flight->origin, flight->destination, flight->airline, flight->date, flight->departureTime, flight->arrivalTime, flight->price, flight->distance);
        }
    }
}

void FlightGraph::displayGraph() const 
{
    for (int i = 0; i < vertexCount; i++) {
        if (vertices[i].city[0] == '\0') continue;

        cout << "City: " << vertices[i].city << " -> ";
        Edge* edge = vertices[i].head;
        while (edge) 
        {
            cout << vertices[edge->destination].city
                      << " (Airline: " << edge->flightData->airline
                      << ", Date: " << edge->flightData->date
                      << ", Price: " << edge->flightData->price << "), "
                      << "Distance: " << edge->flightData->distance << " km, ";
            edge = edge->next;
        }
        cout << "\n";
    }
}

void FlightGraph::displayOnMap(sf::RenderWindow& window, const sf::Texture& mapTexture) 
{
    sf::Sprite map(mapTexture);
    sf::Vector2u windowSize = window.getSize();
    map.setScale(static_cast<float>(windowSize.x) / mapTexture.getSize().x,
                 static_cast<float>(windowSize.y) / mapTexture.getSize().y);
    window.draw(map);

    sf::CircleShape cityPoint(5.f);
    cityPoint.setFillColor(sf::Color::Red);

    sf::Font font;
    if (!font.loadFromFile("Assets/arial.ttf")) 
    {
        std::cerr << "Error: Failed to load font.\n";
        return;
    }

    for (int i = 0; i < coordinateCount; i++) 
    {
        sf::Vector2f position = coordinates[i].position;
        sf::Vector2f scaledPos = sf::Vector2f(position.x * windowSize.x, position.y * windowSize.y);
        cityPoint.setPosition(scaledPos - sf::Vector2f(cityPoint.getRadius(), cityPoint.getRadius()));
        window.draw(cityPoint);

        
        sf::Text cityName(coordinates[i].city, font, 12);
        cityName.setFillColor(sf::Color::Black);

        if (strcmp(coordinates[i].city,"London")==0)
        {
            
            cityName.setPosition(scaledPos - sf::Vector2f(50, 10));  
        }
        else if (strcmp(coordinates[i].city,"Paris" )== 0)
        {
            
            cityName.setPosition(scaledPos + sf::Vector2f(0, 10));  
        }
        else if (strcmp(coordinates[i].city,"Berlin") == 0)
        {       
            
            cityName.setPosition(scaledPos - sf::Vector2f(0, 20));  
        }
        else
        {          
            cityName.setPosition(scaledPos + sf::Vector2f(10, -10));  
        }
        
        window.draw(cityName);
    }
}

// Display the graph with highlighted routes
void FlightGraph::displayHighlightedRoutes(sf::RenderWindow& window) 
{
    for (int i = 0; i < vertexCount; ++i) {
        Edge* edge = vertices[i].head;
        while (edge) {
            int destIndex = edge->destination;

            // Highlight the route in red or another color
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(coordinates[i].position.x, coordinates[i].position.y), sf::Color::Red),
                sf::Vertex(sf::Vector2f(coordinates[destIndex].position.x, coordinates[destIndex].position.y), sf::Color::Red)
            };

            window.draw(line, 2, sf::Lines);
            edge = edge->next;
        }
    }
}

// Clear the graph
void FlightGraph::clear()
{
    for (int i = 0; i < vertexCount; i++) 
    {
        Edge* edge = vertices[i].head;
        while (edge) 
        {
            Edge* temp = edge;
            edge = edge->next;
            delete temp->flightData;
            delete temp;
        }
    }
    vertexCount = 0;
}

// Destructor
FlightGraph::~FlightGraph() 
{
    for (int i = 0; i < vertexCount; i++) 
    {
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
