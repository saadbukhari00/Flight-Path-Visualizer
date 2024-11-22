#ifndef MAIN_H
#define MAIN_H

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>
#include<fstream>
#include <cstring>

using namespace std;
struct Flight 
{
    char origin[50], destination[50], date[20], departureTime[10], arrivalTime[10], airline[50];
    int price;

    void display()
    {
         cout <<" Origin: " << origin << ", Destination: " << destination
                << ", Date: " << date << ", Departure: " << departureTime
                << ", Arrival: " << arrivalTime << ", Price: " << price
                << ", Airline: " << airline << "\n";
                cout<<endl;
    }

};

#endif