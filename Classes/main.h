#ifndef MAIN_H
#define MAIN_H

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>
#include<fstream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <SFML/Graphics.hpp>

using namespace std;

struct Airplane 
{
    sf::ConvexShape shape;      // The triangle to represent the airplane
    sf::Vector2f startPosition;
    sf::Vector2f targetPosition;
    float speed;
    bool isMoving;
};

struct Flight 
{
    char origin[50];
    char destination[50];
    char airline[50];
    char date[50];
    char departureTime[50];
    char arrivalTime[50];
    int price;
    int distance;


    Flight()
    {
        strcpy(origin, "");
        strcpy(destination, "");
        strcpy(airline, "");
        strcpy(date, "");
        strcpy(departureTime, "");
        strcpy(arrivalTime, "");
        distance = 0;
        price = 0;
    }

    Flight(const char* origin, const char* destination, const char* airline, const char* date, const char* departureTime, const char* arrivalTime, int price, int distance)
    {
        strcpy(this->origin, origin);
        strcpy(this->destination, destination);
        strcpy(this->airline, airline);
        strcpy(this->date, date);
        strcpy(this->departureTime, departureTime);
        strcpy(this->arrivalTime, arrivalTime);
        this->price = price;
        this->distance = distance;
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
