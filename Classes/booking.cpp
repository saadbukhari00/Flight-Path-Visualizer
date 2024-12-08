#include "booking.h"
#include "layover.h"       // Ensure Layover class is included
#include "HotelsList.h"    // Ensure HotelsList class is included
#include "HotelBooking.h"  // Ensure HotelBooking class is included
#include "FileHandling.h"  // For fileHandler if needed


FlightBook::FlightBook(sf::RenderWindow& main, sf::RenderWindow& win, MainGUI& mainG, FlightGraph& flightG) 
    : mainWindow(main), window(win), mainGUI(mainG), flightGraph(flightG) {} 

void FlightBook::bookFlightOption(string& org, string& dest, LinkedList &directFlights, RouteList &indirectRoutes) {
    origin = org;
    destination = dest;

    // Display direct flights
    cout << "\n\033[1;36mAvailable Direct Flights:\033[0m\n";
    directFlights.Display();
    int directCount = directFlights.size();
    if (directCount <= 0) {
        cout << "\033[1;31mNo Direct Flights available.\033[0m\n";
    }

    // Display indirect routes
    cout << "\n\033[1;36mAvailable Indirect Routes (Multi-leg Journeys):\033[0m\n";
    indirectRoutes.DisplayWithIndexOffset(directCount);
    int routeCount = indirectRoutes.countRoutes();
    if(routeCount <= 0) {
        cout << "\033[1;31mNo Indirect Routes available.\033[0m\n";
    }

    int totalOptions = directCount + routeCount;
    if (totalOptions == 0) {
        cout << "No flights/routes available to book.\n";
        return;
    }

    cout << "\nEnter the index of the flight/route you want to book: ";
    int choice;
    cin >> choice;

    if (choice < 0 || choice >= totalOptions) {
        cout << "\033[1;31mInvalid choice.\033[0m\n";
        return;
    }

    if (choice < directCount) 
    {
        // It's a direct flight
        LinkedList::FlightNode* selectedFlightNode = directFlights.getNodeAt(choice);
        if (!selectedFlightNode) 
        {
            cout << "\033[1;31mInvalid flight index.\033[0m\n";
            return;
        }

        Flight &selectedFlight = selectedFlightNode->flight;
        // Book this single flight
        confirmBooking(selectedFlight); 
    } 
    else 
    {
        // It's an indirect route (multi-leg journey)
        int routeIndex = choice - directCount;
        RouteList::RouteNode* routeNode = indirectRoutes.getRouteByIndex(routeIndex);
        if (!routeNode) {
            cout << "\033[1;31mInvalid route index.\033[0m\n";
            return;
        }

        // Book all flights in this multi-leg route
        confirmBooking(routeNode->route.legs, routeNode); 
    }
}

void FlightBook::confirmBooking(Flight &flight) {
    cout << "\n\033[1;32mBooking Details:\033[0m\n";
    cout << "From: " << flight.origin << "\n";
    cout << "To: " << flight.destination << "\n";
    cout << "Date: " << flight.date << "\n";
    cout << "Price: $" << flight.price << "\n";
    cout << "Airline: " << flight.airline << "\n";
    cout << "\033[1;32mFlight booked successfully!\033[0m\n";
    // No layovers for a direct flight, so no hotel booking needed here.
}

void FlightBook::confirmBooking(LinkedList &legs, RouteList::RouteNode* routeNode) {
    cout << "\n\033[1;32mBooking Multi-Leg Route Details:\033[0m\n";
    int totalPrice = 0;
    int totalDistance = 0;

    LinkedList::FlightNode* curr = legs.getHead();
    while (curr) 
    {
        cout << curr->flight.origin << " -> " << curr->flight.destination << "\n";
        cout << "Date: " << curr->flight.date << ", Airline: " << curr->flight.airline
             << ", Departure: " << curr->flight.departureTime 
             << ", Arrival: " << curr->flight.arrivalTime 
             << ", Price: " << curr->flight.price 
             << ", Distance: " << curr->flight.distance << "\n\n";
        totalPrice += curr->flight.price;
        totalDistance += curr->flight.distance;
        curr = curr->next;
    }

    cout << "Total Price: $" << totalPrice << "\n";
    cout << "Total Distance: " << totalDistance << " km\n";
    cout << "\033[1;32mMulti-leg route booked successfully!\033[0m\n";


    // Now handle layovers:
    if (legs.size() > 1) 
    {
        cout << "\n\033[1;36mLayover Information:\033[0m\n";
        Layover layoverCalculator; // For calculating layover times
        LinkedList::FlightNode* leg = legs.getHead();

        displayInDirectFlightsOnMap(legs,routeNode);
        // Initialize HotelsList and HotelBooking for possible hotel booking
        FileHandling fileHandler(200,50); // adjust as needed
        HotelsList hotelsList(fileHandler);
        hotelsList.populateHotelsList(); // ensure this populates one hotel per city
        HotelBooking hotelBooking(hotelsList);

        while (leg && leg->next) {
            Flight &currentFlight = leg->flight;
            Flight &nextFlight = leg->next->flight;

            int layoverTime = layoverCalculator.calculateLayoverTime(currentFlight.arrivalTime, nextFlight.departureTime);
            int hours = layoverTime / 60;
            int minutes = layoverTime % 60;

            cout << "Layover at " << currentFlight.destination << ": " 
                 << hours << " hours " << minutes << " minutes.\n";

            // If layover > 12 hours (720 minutes), offer hotel booking
            if (layoverTime > 720) {
                cout << "\033[1;33mLong layover detected in " << currentFlight.destination << " ("
                     << hours << "h " << minutes << "m). Would you like to book a hotel? (Y/n): \033[0m";
                char hChoice;
                cin >> hChoice;
                if (tolower(hChoice) == 'y') {
                    // Book a hotel in this city
                    hotelBooking.bookHotelInCity(currentFlight.destination);
                }
            }

            leg = leg->next;
        }
    }
}

// Note: The user said "dont change the displayIndirect functions and keep it as same before" 
// so we are NOT modifying displayInDirectFlightsOnMap code other than the booking index issue.
// The function remains as before, just ensures no other logic changed.

void FlightBook::displayInDirectFlightsOnMap(LinkedList& legs, RouteList::RouteNode* curr)
{
    sf::Vector2u windowSize = mainWindow.getSize();

    // Initialize the airplane
    Airplane airplane;
    airplane.shape.setPointCount(3);
    airplane.shape.setPoint(0, sf::Vector2f(0, -10)); // Top vertex of the triangle
    airplane.shape.setPoint(1, sf::Vector2f(-5, 10)); // Bottom left vertex
    airplane.shape.setPoint(2, sf::Vector2f(5, 10)); // Bottom right vertex
    airplane.shape.setFillColor(sf::Color::Red);
    airplane.speed = 100.f; // Speed of the airplane

    LinkedList::FlightNode* leg = legs.getHead();
    while (leg) 
    {
        sf::Vector2f positionOrigin = flightGraph.getCityPosition(leg->flight.origin);
        sf::Vector2f positionDestination = flightGraph.getCityPosition(leg->flight.destination);
        sf::Vector2f scaledPos = sf::Vector2f(positionOrigin.x * windowSize.x, positionOrigin.y * windowSize.y);
        sf::Vector2f scaledDes = sf::Vector2f(positionDestination.x * windowSize.x, positionDestination.y * windowSize.y);

        airplane.startPosition = scaledPos;
        airplane.targetPosition = scaledDes;
        airplane.shape.setPosition(airplane.startPosition);
        airplane.shape.setRotation(calculateAngle(airplane.startPosition, airplane.targetPosition));
        
        airplane.isMoving = true;

        // Create dotted path
        sf::VertexArray dottedPath(sf::LinesStrip);
        float dotSpacing = 10.f;
        float totalDistance = sqrt(pow(scaledDes.x - scaledPos.x, 2) + pow(scaledDes.y - scaledPos.y, 2));
        int dotCount = static_cast<int>(totalDistance / dotSpacing);

        sf::Color pathColor;
        if (curr->shortest && curr->cheapest)
        {
            pathColor = sf::Color::Green;
            airplane.speed = 50.f;
        }
        else if (curr->cheapest)
        {
            pathColor = sf::Color::Red;
            airplane.speed = 50.f;
        }
        else if (curr->shortest)
        {
            pathColor = sf::Color::Magenta;
            airplane.speed = 50.f;
        }
        else
            pathColor = sf::Color::Black;

        for (int i = 0; i <= dotCount; ++i) {
            float t = i * dotSpacing / totalDistance;
            sf::Vector2f dotPosition = scaledPos + t * (scaledDes - scaledPos);
            if (i % 2 == 0) {
                dottedPath.append(sf::Vertex(dotPosition, pathColor));
            }
        }

        sf::Clock clock;
        while (airplane.isMoving)
        {
            float deltaTime = clock.restart().asSeconds();
            updateAirplanePosition(airplane, deltaTime);

            mainWindow.clear();
            mainGUI.draw();        
            mainWindow.draw(dottedPath);
            mainWindow.draw(airplane.shape);
            mainWindow.display();

            sf::sleep(sf::milliseconds(1));
        }

        leg = leg->next;
    }
}