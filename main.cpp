/*
COMMAND FOR COMPILATION MAC NOTE - (change with location of where you installed SFML):
g++ -I/opt/homebrew/opt/sfml/include -L/opt/homebrew/opt/sfml/lib main.cpp Classes/.cpp -o FlightPathVisualizer -lsfml-graphics -lsfml-window -lsfml-system
*/

#include "Classes/main.h"
#include "Classes/list.h"
#include "Classes/FileHandling.h"
#include "Classes/heap.h"
#include "Classes/stack.h"
#include "Classes/queue.h"
#include <SFML/Graphics.hpp>

FileHandling files(105, 10); // (number of flights, number of hotel cities)

class Preferences {
public:
    // Add attributes and methods for user preferences
};

class FlightGraph {
private:
    LinkedList* flights; // Array of linked lists to store flights
    int vertices;

public:
    FlightGraph() {
        files.readFlightsFile();
        vertices = files.getFlightCount();
        flights = new LinkedList[vertices];
    }

    void addFlight() {
        for (int i = 0; i < files.getFlightCount(); i++) {
            Flight* flight = files.getFlightByIndex(i);
            if (!flight) {
                cout << "Error: Flight data not found for index " << i << endl;
                continue;
            }
            int originIndex = getCityIndex(flight->origin);
            if (originIndex == -1) {
                cout << "Error: Origin city not mapped: " << flight->origin << endl;
                continue;
            }
            flights[originIndex].insert(*flight);
        }
    }

    int getCityIndex(const char* city) {
        for (int i = 0; i < vertices; i++) {
            if (!flights[i].isEmpty() && strcmp(flights[i].getHeadFlight().origin, city) == 0) {
                return i;
            }
        }
        return -1;
    }

    void displayFlights() {
        for (int i = 0; i < vertices; i++) {
            if (flights[i].isEmpty()) continue;
            cout << "Flights from " << flights[i].getHeadFlight().origin << ":\n";
            flights[i].Display();
        }
    }

    ~FlightGraph() {
        delete[] flights;
    }
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
    FlightGraph graph;
    graph.addFlight();
    graph.displayFlights();

    // Uncomment for SFML Test
    /*
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test Window");

    sf::CircleShape circle(100.f); // Radius 100
    circle.setFillColor(sf::Color::Green); // Green fill color
    circle.setPosition(350.f, 250.f); // Position at center

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Blue);
        window.draw(circle);
        window.display();
    }
    */

    return 0;
}