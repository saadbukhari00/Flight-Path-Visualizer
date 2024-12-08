#ifndef BOOKING_H
#define BOOKING_H

#include "main.h"
#include "list.h"
#include "RouteList.h"
#include "mainGUI.h"
#include "FlightGraph.h"
#include "utils.h"

class FlightBook{
public:
    void bookFlightOption(string&,string&,LinkedList &directFlights, RouteList &indirectRoutes);
    void confirmBooking(Flight &flight);
    void confirmBooking(LinkedList &legs,RouteList::RouteNode*);
    void displayInDirectFlightsOnMap(LinkedList&, RouteList::RouteNode*);
    FlightBook(sf::RenderWindow&,sf::RenderWindow&, MainGUI&, FlightGraph&);

    MainGUI mainGUI;
    sf::RenderWindow& window;
    sf::RenderWindow& mainWindow;
    FlightGraph& flightGraph;

    string origin;
    string destination;

};

#endif