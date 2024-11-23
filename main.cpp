/*
COMMAND FOR COMPILATION MAC NOTE - (change with location of where you installed SFML):
g++ -I/opt/homebrew/opt/sfml/include -L/opt/homebrew/opt/sfml/lib main.cpp Classes/.cpp -o FlightPathVisualizer -lsfml-graphics -lsfml-window -lsfml-system
*/

/* added by Huzaifa for compilation on my system
 g++ -o FlightPathVisualizer main.cpp Classes/FileHandling.cpp -lsfml-graphics -lsfml-window -lsfml-system
*/

#include "Classes/main.h"
#include "Classes/list.h"
#include "Classes/heap.h"
#include "Classes/stack.h"
#include "Classes/queue.h"
#include "Classes/FlightGraph.h"

FileHandling files(105, 10); // (number of flights, number of hotel cities)

class Preferences {
public:
    // Add attributes and methods for user preferences
};

class RouteFinder {
public:
    // Implement route-finding algorithms here
};

class LayoverManager {
    Queue flightQueue;

public:
    // Implement layover management here
};

class UserInterface {
private:
    // Add UI-related attributes and methods
};


int main() {
    FileHandling fileHandler;
    FlightGraph graph(50, fileHandler, 50);

    graph.populateGraph();

    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("Assets/world_map.png")) {
        std::cerr << "Error loading world_map.png. Ensure the file exists in the 'Assets' folder.\n";
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Flight Graph Visualization");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        graph.displayOnMap(window, mapTexture);
        window.display();
    }

    return 0;
}