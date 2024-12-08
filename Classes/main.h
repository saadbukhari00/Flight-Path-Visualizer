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
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstdio>
#include <SFML/Graphics.hpp>

using namespace std;



// Airplane structure
struct Airplane 
{
    sf::ConvexShape shape;      // The triangle to represent the airplane
    sf::Vector2f startPosition;
    sf::Vector2f targetPosition;
    float speed;
    bool isMoving;
};


// Flight structure
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
    bool cheapest;
    bool shortest;

    // Default constructor
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
        cheapest = false;
        shortest = false;
    }

    // Parameterized constructor
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
        this->cheapest = false;
        this->shortest = false;
    }

    // Display the flight details
    void display()
    {
         cout <<" Origin: " << origin << ", Destination: " << destination
                << ", Date: " << date << ", Departure: " << departureTime
                << ", Arrival: " << arrivalTime << ", Price: " << price
                << ", Airline: " << airline << "\n";
                cout<<endl;
    }
    
    

};


// Hotel structure
struct Hotel
{
public:
    char city[50];
    char name[100];
    float pricePerNight;

    Hotel()
    {
        strcpy(city, "");
        strcpy(name, "");
        pricePerNight = 0;
    }

    Hotel(const char* city, const char* name, float pricePerNight)
    {
        strcpy(this->city, city);
        strcpy(this->name, name);
        this->pricePerNight = pricePerNight;
    }

    // Display the hotel details
    void allocateHotelNamesRandomly(const char * city)
    {
        if(strcmp(city, "Islamabad") == 0)
        {
            strcpy(name, "Serena Hotel");
        }
        else if(strcmp(city, "NewYork") == 0)
        {
            strcpy(name, "Hilton Hotel");
        }
        else if(strcmp(city, "Paris") == 0)
        {
            strcpy(name, "Ritz Hotel");
        }
        else if(strcmp(city, "Tokyo") == 0)
        {
            strcpy(name, "Tokyo Hotel");
        }
        else if(strcmp(city, "London") == 0)
        {
            strcpy(name, "London Hotel");
        }
        else if(strcmp(city, "Singapore") == 0)
        {
            strcpy(name, "Singapore Hotel");
        }
        else if(strcmp(city, "HongKong") == 0)
        {
            strcpy(name, "HongKong Hotel");
        }
        else if(strcmp(city, "Berlin") == 0)
        {
            strcpy(name, "Berlin Hotel");
        }
        else if(strcmp(city, "Seoul") == 0)
        {
            strcpy(name, "Seoul Hotel");
        }
        else if(strcmp(city, "Amsterdam") == 0)
        {
            strcpy(name, "Amsterdam Hotel");
        }
    }
};

#endif
