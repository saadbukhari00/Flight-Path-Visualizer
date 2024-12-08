#include "menu.h"


// Display the header of the program
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

// Take transit cities from user
string * Menu::takeTransitCities(int &n)
{
    string *cities = new string[100];
    cin.ignore();
    do
    {
        cout << "\033[1;34m\n\tEnter transit city " << n + 1 << ":  (\"stop\" to stop input)\033[0m";
        getline(cin, cities[n]);
        if(cities[n] == "stop")
            break;
        n++;
    }
    while(true);
    
    cout << "\n\033[1;32mCities entered:\033[0m" << endl;
    for (int i = 0; i < n; i++) 
    {
        cout << cities[i] << endl;
    }
    
    return cities;
}
