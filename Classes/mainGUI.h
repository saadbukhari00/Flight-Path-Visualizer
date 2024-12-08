#ifndef MAINGUI_H
#define MAINGUI_H


#include"main.h"
#include"FlightGraph.h"

class MainGUI 
{
private:
    sf::RenderWindow& mainWindow;
    sf::Font font;
    sf::RectangleShape bookFlightButton;
    sf::Text buttonText;
    FlightGraph& flightGraph;
    Airplane airplane; // Adding airplane as a member variable

public:
    MainGUI(sf::RenderWindow& window, FlightGraph& graph)
        : mainWindow(window), flightGraph(graph), airplane() 
        {
        font.loadFromFile("Assets/Aller_Bd.ttf");

        // Configure the Book Flight button
        bookFlightButton.setSize(sf::Vector2f(150, 50));
        bookFlightButton.setPosition(10, 10);
        bookFlightButton.setFillColor(sf::Color::Green);

        buttonText.setFont(font);
        buttonText.setCharacterSize(20);
        buttonText.setFillColor(sf::Color::Black);
        buttonText.setString("Book Flight");
        buttonText.setPosition(25, 20);
    }

    bool handleEvents();

    void addAirplane(const Airplane& newAirplane);

    void draw();
};

#endif