#include "menu.h"

void Menu::displayHeader()
{
    cout << "\033[1;32m";
    cout << "\t\t  _____ _            _   _                         ______" << endl;
    cout << "\t\t / ____| |          | \\ | |                 /\\    |_    _|" << endl;
    cout << "\t\t| (___ | |_         |  \\| | __ _           /  \\     |  |" << endl;
    cout << "\t\t \\___ \\|  __/\\    | |   ` |/ `_ |\\      / / -  \\    |  |" << endl;
    cout << "\t\t ____) | ||   \\   | | |\\  | (_| | \\    / / ___  \\  _|  |_" << endl;
    cout << "\t\t|_____/ \\__\\   |__| |_| \\_|\\__,_|  \\__/ /_/   \\_ \\|______|" << endl;
    cout << "\t\t               /  /" << endl;
    cout << "\t\t              /  /" << endl;
    cout << "\t\t             |__/ " << endl;
    cout << "\033[1m";
    cout << "\033[0m";
}

void Menu::displayFlightHeader(string origin, string destination)
{
    cout << "\033[1;36m\033[1m\033[1;36m";
    cout << "\t\tDirect Flights from " << origin << " to " << destination << endl << endl;
        cout << "  \t _____________________________________________________________________\n";
        cout << "\t| Date     | Airline | Departure Time| Arrival Time| Price | Distance |\n";
        cout << "\t|__________|_________|_______________|_____________|_______|__________|\n";
}

void Menu::displaySCheader(string startCity, string endCity)
{
        cout << "\033[1;33m";
        cout << "\n\t\tCheapest Route from " << startCity << " to " << endCity << "\n\n";
        cout << "  \t _____________________________________________________________________\n";
        cout << "\t| Date     | Airline | Departure Time| Arrival Time| Price | Distance |\n";
        cout << "\t|__________|_________|_______________|_____________|_______|__________|\n";
}

void Menu::displayIndFlightHeader(string origin, string dest)
{
        cout << "\033[1;36m";
        cout << "\n\t\tIndirect Flights from " << origin << " to " << dest << "\n\n";
        cout << "  \t _____________________________________________________________________\n";
        cout << " First Leg Date | Second Leg Date | Airline \n";
}