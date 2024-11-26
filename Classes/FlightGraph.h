#ifndef FLIGHT_GRAPH_H
#define FLIGHT_GRAPH_H

#include "main.h"
#include "FileHandling.h"

struct Edge {
    int destination;
    Flight* flightData;
    Edge* next;

    Edge(int dest, Flight* data, Edge* nextEdge)
        : destination(dest), flightData(data), next(nextEdge) {}
};

struct Vertex {
    char city[50];
    Edge* head;
};

struct CityCoordinate {
    char city[50];
    sf::Vector2f position;

    // Default constructor
    CityCoordinate() : position(0, 0) {
        strcpy(city, "");
    }

    // Parameterized constructor
    CityCoordinate(float x, float y, const char* name) {
        position = sf::Vector2f(x, y);
        strcpy(city, name);
    }
};

class FlightGraph {
private:
    Vertex* vertices;
    CityCoordinate* coordinates;
    int vertexCount;
    int maxVertices;
    int coordinateCount;
    FileHandling& fileHandler;

public:
    sf::Texture mapTexture;
    FlightGraph(int size, FileHandling& fileHandler);
    ~FlightGraph();

    Vertex* getVertices();
    int getCityIndex(const char* cityName) const; 
    int getNumVertices() const;

    void initializeCityCoordinates();
    sf::Vector2f getCityPosition(const char* cityName) ;

    int findOrAddCity(const char* cityName);
    void addCity(const char* city, sf::Vector2f position);
    void addFlight(const char* origin, const char* destination, const char* airline, const char* date,
                   const char* departureTime, const char* arrivalTime, int price, int distance);
    
    void populateGraph();
    void displayGraph() const;

    void validateCity(const FlightGraph& graph, const char* city);

    void displayOnMap(sf::RenderWindow& window, const sf::Texture& mapTexture);
    void displayHighlightedRoutes(sf::RenderWindow& window);

};

#endif 