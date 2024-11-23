#include "main.h"
#include "FileHandling.h"

class FlightGraph {
private:
    struct Edge {
        int destination;
        Flight* flightData;
        Edge* next;
        Edge(int dest, Flight* data):destination(dest),flightData(data),next(nullptr){}
    };

    struct Vertex {
        char city[50];
        Edge* head;
        Vertex():head(nullptr){city[0]='\0';}
    };

    struct CityCoordinate {
        char city[50];
        sf::Vector2f position;
    };

    Vertex* vertices;
    CityCoordinate* coordinates;
    int vertexCount, coordinateCount;
    FileHandling& fileHandler;

    int getCityIndex(const char* city);
    sf::Vector2f getCityPosition(const char* city);

public:
    FlightGraph(int size,FileHandling& fileHandler,int maxCoordinates);
    void initializeCityCoordinates();
    void addCity(const char* city,sf::Vector2f position);
    void addFlight(const char* origin,const char* destination,const char* airline,const char* date,const char* flyingTime,const char* landingTime,int price);
    void populateGraph();
    void displayGraph() const;
    void adjustForOverlaps();
    void displayOnMap(sf::RenderWindow& window,const sf::Texture& mapTexture);
    ~FlightGraph();
};