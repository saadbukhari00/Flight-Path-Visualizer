#include"FileHandling.h" 

    FileHandling::FileHandling(int flightCapacity, int hotelCapacity) 
    {
        maxFlights = flightCapacity;
        maxHotels = hotelCapacity;
        flights = new Flight[maxFlights];
        hotels = new Hotel[maxHotels];
        flightCount = 0;
        hotelCount = 0;
    }

    int FileHandling::getFlightCount() const 
    {
        return flightCount;
    }

    int FileHandling::getHotelCount() const 
    {
        return hotelCount;
    }

    Flight* FileHandling::readFlightsFile() 
    {
        const char* fileName = "Assets/flights.txt";
        ifstream file(fileName);
        if (!file.is_open()) 
        {
            cout << "Could not open the flights file.\n";
            return nullptr;
        }

        char line[256];
        while (file.getline(line, sizeof(line))) 
        {
            if (flightCount >= maxFlights) 
            {
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

            if (parsed == 8) 
            {
                flightCount++;
            } 
            else 
            {
                cout << "Error parsing line: " << line << "\n";
            }
        }

        file.close();
        return flights;
    }
    
    Hotel* FileHandling::readHotelsFile() 
    {
        const char* fileName = "Assets/HotelCharges_perday.txt";
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
            sscanf(line, "%s %f", hotels[hotelCount].city, &hotels[hotelCount].pricePerNight);
            hotelCount++;
        }
        file.close();
        return hotels;
    }

    Flight* FileHandling::getFlightByIndex(int index) 
    {
        if (index < 0 || index >= flightCount) 
        {
            return nullptr;
        }
        return &flights[index];
    }

    Hotel* FileHandling::getHotelByIndex(int index) 
    {
        if (index < 0 || index >= hotelCount) 
        {
            return nullptr;
        }
        return &hotels[index];
    }

    FileHandling::~FileHandling() 
    {
        delete[] flights;
        delete[] hotels;
    }
