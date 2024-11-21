/*
COMMAND FOR COMPILATION MAC //NOTE - (change with location of where you installed SFML):
g++ -I/opt/homebrew/opt/sfml/include -L/opt/homebrew/opt/sfml/lib main.cpp Classes/*.cpp -o FlightPathVisualizer -lsfml-graphics -lsfml-window -lsfml-system
*/


#include "Classes/main.h"
#include "Classes/list.h"
#include "Classes/FileHandling.h"
#include "Classes/heap.h"
#include "Classes/stack.h"
#include "Classes/queue.h"
#include <SFML/Graphics.hpp>
FileHandling files(105, 10);    //(number of flights, number of hotel cities)

class Preferences {
public:

};

class FlightGraph {
private:
    LinkedList * flights;
    int vertices;
};

class RouteFinder {
public:
};

// LayoverManager class
class LayoverManager {
public:
};

// UserInterface class
class UserInterface {
private:
};

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test Window");

    // Create a circle shape
    sf::CircleShape circle(100.f); // Radius 100
    circle.setFillColor(sf::Color::Green); // Green fill color
    circle.setPosition(350.f, 250.f); // Position at center

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window if close event is triggered
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window with a color
        window.clear(sf::Color::Blue);

        // Draw the circle
        window.draw(circle);

        // Display everything in the window
        window.display();
    }

    return 0;
}