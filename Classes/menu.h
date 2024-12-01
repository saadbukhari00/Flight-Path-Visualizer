#include "main.h"

class Menu{
public:
    void displayHeader();
    void displayDirectFlights(string, string);
    void displayIndirectFlights(string, string);

    void displayShortestRoute(string, string);
    void displayCheapestRoute(string, string);

    void displayPreferences();

    void displayExit();
};