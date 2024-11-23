#ifndef MAIN_H
#define MAIN_H

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>
#include<fstream>
#include <cstring>
#include <SFML/Graphics.hpp>

using namespace std;
struct Flight 
{
    char origin[50];
    char destination[50];
    char airline[50];
    char date[50];
    char departureTime[50];
    char arrivalTime[50];
    int price;

    Flight()
    {
        strcpy(origin, "");
        strcpy(destination, "");
        strcpy(airline, "");
        strcpy(date, "");
        strcpy(departureTime, "");
        strcpy(arrivalTime, "");
        price = 0;
    }

    Flight(const char* airline, const char* date, const char* departureTime, const char* arrivalTime, int price)
    {
        strcpy(this->airline, airline);
        strcpy(this->date, date);
        strcpy(this->departureTime, departureTime);
        strcpy(this->arrivalTime, arrivalTime);
        this->price = price;
    }

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