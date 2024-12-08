#ifndef BOOKING_H
#define BOOKING_H

#include "main.h"
#include "list.h"
#include "RouteList.h"
#include "mainGUI.h"
#include "FlightGraph.h"
#include "utils.h"
#include "layover.h"

class FlightBook
{
public:

    void bookFlightOption(string&,string&,LinkedList &directFlights, RouteList &indirectRoutes);
    bool validateFields(const std::string& cardNumber, const std::string& name, const std::string& expiryDate, const std::string& cvc);
    void confirmBooking(Flight &flight);
    void displayDirectFlightsonMap(Flight&);
    void confirmBooking(LinkedList &legs, RouteList::RouteNode* route);
    float calculateAngleDotted(const sf::Vector2f& start, const sf::Vector2f& end);
    void updateAirplanePositionDotted(Airplane& airplane, float deltaTime);
    float distance(const sf::Vector2f& a, const sf::Vector2f& b);
    void displayInDirectFlightsOnMap(LinkedList::FlightNode*, RouteList::RouteNode*,int,int);
    FlightBook(sf::RenderWindow&,sf::RenderWindow&, MainGUI&, FlightGraph&);
    void showMessage(const string& message,sf::RenderWindow& window,sf::Color);

    MainGUI mainGUI;
    sf::RenderWindow& window;
    sf::RenderWindow& mainWindow;
    FlightGraph& flightGraph;
    Layover layover;
    string origin;
    string destination;

};

#endif