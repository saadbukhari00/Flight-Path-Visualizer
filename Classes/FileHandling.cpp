#include"FileHandling.h"

    // Constructor
    FileHandling::FileHandling(int flightCapacity, int hotelCapacity) {
        maxFlights = flightCapacity;
        maxHotels = hotelCapacity;
        flights = new Flight[maxFlights];
        hotels = new Hotel[maxHotels];
        flightCount = 0;
        hotelCount = 0;
    }

    // Read Flights File
    void FileHandling::readFlightsFile() {
        const char* fileName = "flights.txt";
        ifstream file(fileName);
        if (!file.is_open()) {
            cout << "Error: Could not open the flights file.\n";
            return;
        }
        char line[256];
        while (file.getline(line, sizeof(line))) {
            if (flightCount >= maxFlights) {
                cout << "Error: Maximum flight capacity reached.\n";
                break;
            }
            sscanf(line, "%s %s %s %s %s %d %s", flights[flightCount].origin, flights[flightCount].destination, flights[flightCount].date,
                flights[flightCount].departureTime, flights[flightCount].arrivalTime, &flights[flightCount].price, flights[flightCount].airline);
            flightCount++;
        }
        file.close();
    }

    // Read Hotels File
    void FileHandling::readHotelsFile() {
        const char* fileName = "HotelCharges_perday.txt";
        ifstream file(fileName);
        if (!file.is_open()) {
            cout << "Error: Could not open the hotels file.\n";
            return;
        }
        char line[256];
        while (file.getline(line, sizeof(line))) {
            if (hotelCount >= maxHotels) {
                cout << "Error: Maximum hotel capacity reached.\n";
                break;
            }
            sscanf(line, "%s %d", hotels[hotelCount].city, &hotels[hotelCount].chargePerDay);
            hotelCount++;
        }
        file.close();
    }

    // Display Flights
    void FileHandling::displayFlights() const {
        for (int i = 0; i < flightCount; i++) {
            cout << "Flight " << i + 1 << ": Origin: " << flights[i].origin << ", Destination: " << flights[i].destination
                << ", Date: " << flights[i].date << ", Departure: " << flights[i].departureTime
                << ", Arrival: " << flights[i].arrivalTime << ", Price: " << flights[i].price
                << ", Airline: " << flights[i].airline << "\n";
        }
    }

    // Display Hotels
    void FileHandling::displayHotels() const {
        for (int i = 0; i < hotelCount; i++) {
            cout << "Hotel " << i + 1 << ": City: " << hotels[i].city << ", Charge per Day: " << hotels[i].chargePerDay << "\n";
        }
    }

    // Get Flight by Index
    Flight* FileHandling::getFlightByIndex(int index) const {
        return (index >= 0 && index < flightCount) ? &flights[index] : nullptr;
    }

    // Get Hotel by Index
    Hotel* FileHandling::getHotelByIndex(int index) const {
        return (index >= 0 && index < hotelCount) ? &hotels[index] : nullptr;
    }

    // Search Hotel by City
    void FileHandling::searchHotelByCity(const char* city) const {
        for (int i = 0; i < hotelCount; i++) {
            if (strcmp(hotels[i].city, city) == 0) {
                cout << "Hotel in " << hotels[i].city << ": Charge per Day: " << hotels[i].chargePerDay << "\n";
            }
        }
    }


    // Search Hotel by Price Range
    void FileHandling::searchHotelByPriceRange(int minPrice, int maxPrice) const {
        for (int i = 0; i < hotelCount; i++) {
            if (hotels[i].chargePerDay >= minPrice && hotels[i].chargePerDay <= maxPrice) {
                cout << "Hotel in " << hotels[i].city << ": Charge per Day: " << hotels[i].chargePerDay << "\n";
            }
        }
    }

    // Destructor
    FileHandling::~FileHandling() {
        delete[] flights;
        delete[] hotels;
    }