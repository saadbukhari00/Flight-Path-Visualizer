#include "HotelsList.h"

void HotelsList::addHotel(Hotel hotel)
{
    HotelNode * newNode = new HotelNode;
    newNode->hotel = hotel;
    newNode->next = nullptr;

    if (head == nullptr)
    {
        head = newNode;
    }
    else
    {
        HotelNode * temp = head;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void HotelsList::displayHotels()
{
    if (head == nullptr)
    {
        cout << "No hotels to display" << endl;
        return;
    }

    HotelNode * temp = head;
    while (temp != nullptr)
    {
        cout << "Hotel Name: " << temp->hotel.cityName << endl;
        cout << "Hotel Price: $" << temp->hotel.price << endl;
    }
}

void HotelsList::deleteHotel(const char* cityName)
{
    if (head == nullptr)
    {
        cout << "No hotels to delete" << endl;
        return;
    }

    HotelNode * temp = head;
    HotelNode * prev = nullptr;
    while (temp != nullptr)
    {
        if (strcmp(temp->hotel.cityName, cityName) == 0)
        {
            if (prev == nullptr)
            {
                head = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            delete temp;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    cout << "Hotel not found" << endl;
}

void HotelsList::deleteAllHotels()
{
    if (head == nullptr)
    {
        cout << "No hotels to delete" << endl;
        return;
    }

    HotelNode * temp = head;
    HotelNode * next = nullptr;
    while (temp != nullptr)
    {
        next = temp->next;
        delete temp;
        temp = next;
    }
    head = nullptr;
}

void HotelsList::populateHotelsList()
{
    Hotel* hotels = fileHandler.readHotelsFile();
    if (hotels == nullptr)
    {
        cout << "Error reading hotels file" << endl;
        return;
    }

    for (int i = 0; i < fileHandler.getHotelCount(); i++)
    {
        addHotel(hotels[i]);
    }
}