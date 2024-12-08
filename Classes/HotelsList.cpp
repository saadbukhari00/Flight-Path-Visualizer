#include "HotelsList.h"

// Add a hotel to the list
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

// Display all hotels in the list           
void HotelsList::displayHotels()
{
    if (head == nullptr)
    {
        cout << "No hotels to display" << endl;
        return;
    }

    HotelNode * temp = head;
    cout << "Hotels List:" << endl;
    while (temp != nullptr)
    {
        cout << "Hotel Name: " << temp->hotel.name << endl;
        cout << "City Name: " << temp->hotel.city << endl;
        cout << "Price Per Night: " << temp->hotel.pricePerNight << endl;
        cout << "--------------------------------" << endl;
        temp = temp->next;
    }
}

// Delete a hotel from the list
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
        if (strcmp(temp->hotel.city, cityName) == 0)
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

// Delete all hotels from the list
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


// Populate the hotels list from the file
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