#include "main.h"

class Menu{
public:
    void displayHeader();
    
    void displayFlightHeader(string origin, string destination);
    void displaySCheader(string startCity, string endCity);
    void displayIndFlightHeader(string, string);


    void displayPreferences();

    void displayExit();
};