#include "FlightGraph.h"

    FlightGraph::FlightGraph(int size, FileHandling& fileHandler, int maxCoordinates) : fileHandler(fileHandler) {
        vertexCount = size;
        vertices = new Vertex[vertexCount];
        coordinates = new CityCoordinate[maxCoordinates];
        coordinateCount = 0;

        initializeCityCoordinates();
    }

    void FlightGraph::addCity(const char* city,sf::Vector2f position){
        for(int i=0;i<coordinateCount;i++){
            if(strcmp(coordinates[i].city,city)==0)
                return;
        }
        strcpy(coordinates[coordinateCount].city,city);
        coordinates[coordinateCount].position=position;
        coordinateCount++;
    }

    void FlightGraph::initializeCityCoordinates() {
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

    sf::Vector2f FlightGraph::getCityPosition(const char* city){
        for(int i=0;i<coordinateCount;i++){
            if(strcmp(coordinates[i].city,city)==0){
                return coordinates[i].position;
            }
        }
        return sf::Vector2f(-1,-1); // Invalid position
    }

    int FlightGraph::getCityIndex(const char* city){
        for(int i=0;i<vertexCount;i++){
            if(strcmp(vertices[i].city,city)==0){
                return i;
            }
        }
        return -1;
    }

    void FlightGraph::addFlight(const char* origin,const char* destination,const char* airline,const char* date,const char* flyingTime,const char* landingTime,int price){
        int originIndex=getCityIndex(origin);
        int destinationIndex=getCityIndex(destination);

        if(originIndex==-1){
            addCity(origin,sf::Vector2f(-1,-1));
            originIndex=getCityIndex(origin);
        }

        if(destinationIndex==-1){
            addCity(destination,sf::Vector2f(-1,-1)); 
            destinationIndex=getCityIndex(destination);
        }

        Flight* flight=new Flight(airline,date,flyingTime,landingTime,price);
        Edge* newEdge=new Edge(destinationIndex,flight);
        newEdge->next=vertices[originIndex].head;
        vertices[originIndex].head=newEdge;
    }

    void FlightGraph::populateGraph(){
        fileHandler.readFlightsFile();
        int flightCount=fileHandler.getFlightCount();

        for(int i=0;i<flightCount;i++){
            Flight* flight=fileHandler.getFlightByIndex(i);
            if(flight){
                addFlight(flight->origin,flight->destination,flight->airline,flight->date,flight->departureTime,flight->arrivalTime,flight->price);
            }
        }
    }

    void FlightGraph::displayGraph() const{
        for(int i=0;i<vertexCount;i++){
            if(vertices[i].city[0]=='\0')continue;
            std::cout<<"City: "<<vertices[i].city<<" -> ";
            Edge* edge=vertices[i].head;
            while(edge){
                std::cout<<vertices[edge->destination].city<<" (Airline: "<<edge->flightData->airline<<", Price: "<<edge->flightData->price<<"), ";
                edge=edge->next;
            }
            std::cout<<"\n";
        }
    }

    void FlightGraph::adjustForOverlaps() {
        for (int i = 0; i < coordinateCount; i++) {
            for (int j = i + 1; j < coordinateCount; j++) {
                if (coordinates[i].position == coordinates[j].position) {
                    coordinates[j].position.x += 0.02f; // Offset slightly
                    coordinates[j].position.y += 0.02f;
                }
            }
        }
    }

    void FlightGraph::displayOnMap(sf::RenderWindow& window, const sf::Texture& mapTexture) {
        sf::Sprite map(mapTexture);

        sf::Vector2u textureSize = mapTexture.getSize();
        sf::Vector2u windowSize = window.getSize();
        map.setScale(static_cast<float>(windowSize.x) / textureSize.x, static_cast<float>(windowSize.y) / textureSize.y);
        window.draw(map);

        sf::CircleShape cityPoint(5.f);
        cityPoint.setFillColor(sf::Color::Red);

        for (int i = 0; i < coordinateCount; i++) {
            sf::Vector2f normalizedPos = coordinates[i].position;
            sf::Vector2f scaledPos = sf::Vector2f(
                normalizedPos.x * windowSize.x,
                normalizedPos.y * windowSize.y
            );

            cityPoint.setPosition(scaledPos.x - cityPoint.getRadius(), scaledPos.y - cityPoint.getRadius());
            window.draw(cityPoint);

            sf::Font font;
            if (!font.loadFromFile("Assets/arial.ttf")) {
                std::cerr << "Error loading arial.ttf. Ensure the file exists in the 'Assets' folder.\n";
                continue;
            }

            sf::Text cityName(coordinates[i].city, font, 12);
            if (strcmp(coordinates[i].city, "London") == 0) {
        // Adjust the position for Newyork (move the text down more)
        cityName.setPosition(scaledPos.x - 25, scaledPos.y + 10); // Move text below the dot
    }
    else if (strcmp(coordinates[i].city, "Amsterdam") == 0) {
        // Adjust the position for Amsterdam (move the text above the dot)
        cityName.setPosition(scaledPos.x - 20, scaledPos.y - 25);
        }
         else {
        // For all other cities, set the normal position
        cityName.setPosition(scaledPos.x - 7, scaledPos.y + 10); // Normal distance
    }
            cityName.setFillColor(sf::Color::Black);
            window.draw(cityName);
        }
    }

    FlightGraph::~FlightGraph(){
        for(int i=0;i<vertexCount;i++){
            Edge* edge=vertices[i].head;
            while(edge){
                Edge* temp=edge;
                edge=edge->next;
                delete temp->flightData;
                delete temp;
            }
        }
        delete[] vertices;
        delete[] coordinates;
    }
    
    
    
    int FlightGraph::getNumVertices() const {
    return vertexCount;
}

FlightGraph::Vertex* FlightGraph::getVertices() {
    return vertices;
}
