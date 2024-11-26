#include"FileHandling.h"

    FileHandling::FileHandling(int flightCapacity, int hotelCapacity) {
        maxFlights = flightCapacity;
        maxHotels = hotelCapacity;
        flights = new Flight[maxFlights];
        hotels = new Hotel[maxHotels];
        flightCount = 0;
        hotelCount = 0;
    }

    int FileHandling::getFlightCount() const {
        return flightCount;
    }

    int FileHandling::getHotelCount() const {
        return hotelCount;
    }


    Flight* FileHandling::readFlightsFile() {
        const char* fileName = "Assets/flights.txt";
        ifstream file(fileName);
        if (!file.is_open()) {
            cout << "Could not open the flights file.\n";
            return nullptr;
        }

        char line[256];
        while (file.getline(line, sizeof(line))) {
            if (flightCount >= maxFlights) {
                cout << "Error: Maximum flight capacity reached.\n";
                break;
            }

            int parsed = sscanf(line, "%s %s %s %s %s %d %s %d", 
                flights[flightCount].origin, 
                flights[flightCount].destination, 
                flights[flightCount].date,
                flights[flightCount].departureTime, 
                flights[flightCount].arrivalTime, 
                &flights[flightCount].price, 
                flights[flightCount].airline,
                &flights[flightCount].distance);

            if (parsed == 8) {
                flightCount++;
            } else {
                cout << "Error parsing line: " << line << "\n";
            }
        }

        file.close();
        return flights;
    }
    Hotel* FileHandling::readHotelsFile() {
        const char* fileName = "HotelCharges_perday.txt";
        ifstream file(fileName);
        if(!file.is_open()) 
        {
            cout << "Error: Could not open the hotels file.\n";
            return nullptr;
        }
        char line[256];
        while(file.getline(line, sizeof(line))) 
        {
            if(hotelCount >= maxHotels) 
            {
                cout << "Error: Maximum hotel capacity reached.\n";
                break;
            }
            sscanf(line, "%s %d", hotels[hotelCount].city, &hotels[hotelCount].chargePerDay);
            hotelCount++;
        }
        file.close();
        return hotels;
    }

    void FileHandling::displayFlights() const {
        for (int i = 0; i < flightCount; i++) 
        {
            cout << "Flight " << i + 1 << ": Origin: " << flights[i].origin 
                << ", Destination: " << flights[i].destination
                << ", Date: " << flights[i].date 
                << ", Departure: " << flights[i].departureTime
                << ", Arrival: " << flights[i].arrivalTime 
                << ", Price: " << flights[i].price
                << ", Airline: " << flights[i].airline
                << ", Distance: " << flights[i].distance << " km\n"; // Include distance
        }
    }

    void FileHandling::displayHotels() const {
        for(int i = 0; i < hotelCount; i++) 
        {
            cout << "Hotel " << i + 1 << ": City: " << hotels[i].city << ", Charge per Day: " << hotels[i].chargePerDay << "\n";
        }
    }

    void FileHandling::searchByOrigin(const char* origin) const {
        for(int i = 0; i < flightCount; i++)
            if(strcmp(flights[i].origin, origin) == 0)
                cout << "Flight " << i + 1 << ": Destination: " << flights[i].destination << ", Date: " << flights[i].date
                    << ", Departure: " << flights[i].departureTime << ", Arrival: " << flights[i].arrivalTime
                    << ", Price: " << flights[i].price << ", Airline: " << flights[i].airline << "\n";
    }
    void FileHandling::searchByDestination(const char* destination) const {
        for(int i = 0; i < flightCount; i++)
            if(strcmp(flights[i].destination, destination) == 0)
                cout << "Flight " << i + 1 << ": Origin: " << flights[i].origin << ", Date: " << flights[i].date
                    << ", Departure: " << flights[i].departureTime << ", Arrival: " << flights[i].arrivalTime
                    << ", Price: " << flights[i].price << ", Airline: " << flights[i].airline << "\n";
    }
    Flight* FileHandling::getFlightByIndex(int index) const 
    {
        return (index >= 0 && index < flightCount) ? &flights[index] : nullptr;
    }
    void FileHandling::searchByDate(const char* date) const {
        for (int i = 0; i < flightCount; i++)
            if (strcmp(flights[i].date, date) == 0)
                cout << "Flight " << i + 1 << ": Origin: " << flights[i].origin << ", Destination: " << flights[i].destination
                    << ", Departure: " << flights[i].departureTime << ", Arrival: " << flights[i].arrivalTime
                    << ", Price: " << flights[i].price << ", Airline: " << flights[i].airline << "\n";
    }
    void FileHandling::searchByAirline(const char* airline) const {
        for (int i = 0; i < flightCount; i++)
            if (strcmp(flights[i].airline, airline) == 0)
                cout << "Flight " << i + 1 << ": Origin: " << flights[i].origin << ", Destination: " << flights[i].destination
                    << ", Date: " << flights[i].date << ", Departure: " << flights[i].departureTime
                    << ", Arrival: " << flights[i].arrivalTime << ", Price: " << flights[i].price << "\n";
    }
    void FileHandling::searchByPriceRange(int minPrice, int maxPrice) const {
        for (int i = 0; i < flightCount; i++)
            if (flights[i].price >= minPrice && flights[i].price <= maxPrice)
                cout << "Flight " << i + 1 << ": Origin: " << flights[i].origin << ", Destination: " << flights[i].destination
                    << ", Date: " << flights[i].date << ", Departure: " << flights[i].departureTime
                    << ", Arrival: " << flights[i].arrivalTime << ", Price: " << flights[i].price
                    << ", Airline: " << flights[i].airline << "\n";
    }
    
    Hotel* FileHandling::getHotelByIndex(int index) const 
    {
        return (index >= 0 && index < hotelCount) ? &hotels[index] : nullptr;
    }
    void FileHandling::searchHotelByCity(const char* city) const 
    {
        for(int i = 0; i < hotelCount; i++) 
        {
            if(strcmp(hotels[i].city, city) == 0) 
            {
                cout << "Hotel in " << hotels[i].city << ": Charge per Day: " << hotels[i].chargePerDay << "\n";
                return;
            }
        }
        cout << "No hotel found in " << city << "\n";
    }
    void FileHandling::searchHotelByPriceRange(int minPrice, int maxPrice) const {
        for(int i = 0; i < hotelCount; i++) 
        {
            if(hotels[i].chargePerDay >= minPrice && hotels[i].chargePerDay <= maxPrice) 
            {
                cout << "Hotel in " << hotels[i].city << ": Charge per Day: " << hotels[i].chargePerDay << "\n";
                return;
            }
        }
        cout << "No hotel found in the given price range\n";
    }

    FileHandling::~FileHandling() {
        delete[] flights;
        delete[] hotels;
    }