#include "mainGUI.h"



void MainGUI::draw() 
    {
        mainWindow.clear(sf::Color::White);

        // Draw flight graph
        sf::Texture mapTexture;
        mapTexture.loadFromFile("Assets/world_map.png");
        flightGraph.displayOnMap(mainWindow, mapTexture);

        // Draw Book Flight button
        mainWindow.draw(bookFlightButton);
        mainWindow.draw(buttonText);
        mainWindow.display();
    }

    bool MainGUI::handleEvents() 
    {
        sf::Event event;
        while (mainWindow.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) 
            {
                mainWindow.close();
                return false;
            }
            if (event.type == sf::Event::MouseButtonPressed) 
            {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (bookFlightButton.getGlobalBounds().contains(mousePos)) 
                {
                    return true; // Switch to booking window
                }
            }
        }
        return false;
    }

 void MainGUI::addAirplane(const Airplane& newAirplane)
    {
        airplane = newAirplane; // Store the airplane passed from the search
    }