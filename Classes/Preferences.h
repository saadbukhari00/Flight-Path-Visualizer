#include "main.h"

class Preferences{
private:
    string airline;
    string * cities;
public:
    Preferences(string = "", string * = NULL);

    string getAirline();
    string * getCities();

    void setAirline(string);
    void setCities(string *);

    string * takeCitiesInput();

    void displayPreferences();
};