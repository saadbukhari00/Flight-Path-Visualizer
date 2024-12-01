#include "menu.h"

void Menu::displayHeader()
{
    cout << "\033[1;32m";
    cout << "\t  _____ _            _   _                         ______" << endl;
    cout << "\t / ____| |          | \\ | |                 /\\    |_    _|" << endl;
    cout << "\t| (___ | |_         |  \\| | __ _           /  \\     |  |" << endl;
    cout << "\t \\___ \\|  __/\\    | |   ` |/ `_ |\\      / / -  \\    |  |" << endl;
    cout << "\t ____) | ||   \\   | | |\\  | (_| | \\    / / ___  \\  _|  |_" << endl;
    cout << "\t|_____/ \\__\\   |__| |_| \\_|\\__,_|  \\__/ /_/   \\_ \\|______|" << endl;
    cout << "\t               /  /" << endl;
    cout << "\t              /  /" << endl;
    cout << "\t             |__/ " << endl;
    cout << "\033[1m";
    cout << "\033[0m";
}

void Menu::displayDirectFlights(string origin, string destination)
{
    cout << "\033[1;36m\033[1m\033[1;36m";
    cout << "\t\tDirect Flights from " << origin << " to " << destination << endl << endl;
    cout << "  \t______________________________________________________________________" << endl;
    cout << "\t| Date     | Airline | Departure Time| Arrival Time| Price | Distance |" << endl;
    cout << "\033[0m";
}

void Menu::displayIndirectFlights(string origin, string destination)
{
    cout << "\033[1;35m\033[1m\033[1;36m";
    cout << "\t\tIndirect Flights from " << origin << " to " << destination << endl << endl;
    cout << "  \t______________________________________________________________________" << endl;
    cout << "\t| Date     | Airline | Departure Time| Arrival Time| Price | Distance |" << endl;
    cout << "\033[0m";
}

void Menu::displayShortestRoute(string origin, string destination)
{
    cout << "\033[1;34m\033[1m\033[1;36m";
    cout << "\t\tShortest Route from " << origin << " to " << destination << endl << endl;
    cout << "  \t______________________________________________________________________" << endl;
    cout << "\t| Date     | Airline | Departure Time| Arrival Time| Price | Distance |" << endl;
    cout << "\033[0m";
}

void Menu::displayCheapestRoute(string origin, string destination)
{
    cout << "\033[1;33m\033[1m\033[1;36m";
    cout << "\t\tCheapest Route from " << origin << " to " << destination << endl << endl;
    cout << "  \t______________________________________________________________________" << endl;
    cout << "\t| Date     | Airline | Departure Time| Arrival Time| Price | Distance |" << endl;
    cout << "\033[0m";
}