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
};

class FlightGraph {
private:
    Vertex* vertices;
    CityCoordinate* coordinates;
    int vertexCount;
    int maxVertices;
    int coordinateCount;
    FileHandling& fileHandler;
    sf::Texture mapTexture;

public:
    FlightGraph(int size, FileHandling& fileHandler);
    ~FlightGraph();

    Vertex* getVertices();
    int getCityIndex(const char* cityName);
    int getNumVertices() const;

    void initializeCityCoordinates();
    sf::Vector2f getCityPosition(const std::string& city);

    int findOrAddCity(const char* cityName);
    void addCity(const char* city, sf::Vector2f position);
    void addFlight(const char* origin, const char* destination, const char* airline, const char* date,
                   const char* departureTime, const char* arrivalTime, int price);
    
    void populateGraph();
    void displayGraph() const;
    void displayOnMap(sf::RenderWindow& window, const sf::Texture& mapTexture);

};

#endif 