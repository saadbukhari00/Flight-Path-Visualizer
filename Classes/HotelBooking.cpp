#include "HotelBooking.h"

HotelBooking::HotelBooking(HotelsList &hList) : hotelsList(hList) {}

void HotelBooking::searchHotelsInCity(const char* cityName) {
    HotelsList::HotelNode* curr = hotelsList.getHead();
    bool found = false;
    std::cout << "\n\033[1;36mHotels available in " << cityName << ":\033[0m\n";
    std::cout << "________________________________________________________________________________\n";
    std::cout << "   IDX | Hotel Name                     | City           | Price/Night\n";
    std::cout << "-------|--------------------------------|----------------|-------------\n";
    int index = 0;
    while (curr) {
        if (strcmp(curr->hotel.city, cityName) == 0) {
            found = true;
            std::cout << "   " << index << "   | " << std::setw(30) << curr->hotel.name
                      << " | " << std::setw(14) << curr->hotel.city
                      << " | " << curr->hotel.pricePerNight << "\n";
            // Since only one hotel per city, we can break after we find it.
            break;
        }
        curr = curr->next;
        index++;
    }
    if (!found) {
        std::cout << "\033[1;31mNo hotels found in " << cityName << ".\033[0m\n";
    }
    std::cout << "________________________________________________________________________________\n\n";
}

void HotelBooking::bookHotelInCity(const char* cityName) {
    // Since only one hotel per city, we just find it and confirm the booking.
    HotelsList::HotelNode* curr = hotelsList.getHead();
    curr->hotel.allocateHotelNamesRandomly(cityName);
    while (curr) {
        if (strcmp(curr->hotel.city, cityName) == 0) {
            // Found the hotel for that city
            Hotel &selectedHotel = curr->hotel;
            std::cout << "\n\033[1;32mHotel Booking Confirmed!\033[0m\n";
            
            std::cout << "Hotel Name: " << selectedHotel.name << "\n";
            std::cout << "City: " << selectedHotel.city << "\n";
            std::cout << "Price Per Night: " << selectedHotel.pricePerNight << " USD\n";
            std::cout << "Enjoy your stay!\n\n";
            return;
        }
        curr = curr->next;
    }

    // If we reach here, no hotel found for that city
    std::cout << "\033[1;31mNo hotels available in " << cityName << ".\033[0m\n";
}