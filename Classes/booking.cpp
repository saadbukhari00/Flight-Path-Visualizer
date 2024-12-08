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

void FlightBook::displayDirectFlightsonMap(Flight& flight)
{
    sf::Vector2u windowSize = mainWindow.getSize();

    // Initialize the airplane
    Airplane airplane;
    airplane.shape.setPointCount(3);
    airplane.shape.setPoint(0, sf::Vector2f(0, -10)); // Top vertex
    airplane.shape.setPoint(1, sf::Vector2f(-5, 10)); // Bottom left
    airplane.shape.setPoint(2, sf::Vector2f(5, 10)); // Bottom right
    airplane.shape.setFillColor(sf::Color::Red);
    airplane.speed = 100.f; // Speed of the airplane

        airplane.speed = 100.f;
        string currentOrigin = flight.origin;
        string currentDestination = flight.destination;

        // Get positions of the origin and destination
        sf::Vector2f positionOrigin = flightGraph.getCityPosition(currentOrigin);
        sf::Vector2f positionDestination = flightGraph.getCityPosition(currentDestination);
        sf::Vector2f scaledPos = sf::Vector2f(positionOrigin.x * windowSize.x, positionOrigin.y * windowSize.y);
        sf::Vector2f scaledDes = sf::Vector2f(positionDestination.x * windowSize.x, positionDestination.y * windowSize.y);

        // Set airplane starting and target positions
        airplane.startPosition = scaledPos;
        airplane.targetPosition = scaledDes;
        airplane.shape.setPosition(airplane.startPosition);
        airplane.shape.setRotation(calculateAngle(airplane.startPosition, airplane.targetPosition));

        // Initialize the path for the current flight
        sf::VertexArray path(sf::LineStrip);

        // Determine path color
        sf::Color pathColor;
        if(flight.shortest && flight.cheapest)
        {
            pathColor = sf::Color::Green;
            airplane.speed = 50.f;
        }
        else if (flight.shortest)
        {
            pathColor = sf::Color::Magenta; // Shortest flight
            airplane.speed = 50.f;
        }
        else if (flight.cheapest)
        {
            airplane.speed = 50.f;
            pathColor = sf::Color::Red; // Cheapest flight
        }
        else
            pathColor = sf::Color::Black; // Default

        // Animate the airplane for this flight
        sf::Clock clock;
        airplane.isMoving = true;
        while (airplane.isMoving)
        {
            float deltaTime = clock.restart().asSeconds();
            updateAirplanePosition(airplane, deltaTime);

            // Add the current position to the path
            path.append(sf::Vertex(airplane.shape.getPosition(), pathColor));

            // Clear the window and render everything
            mainWindow.clear();
            mainGUI.draw();              
            mainWindow.draw(path);       
            mainWindow.draw(airplane.shape); 
            mainWindow.display();

            sf::sleep(sf::milliseconds(1)); // Control update frequency
        }

}


void FlightBook::confirmBooking(Flight &flight) 
{
    cout << "\n\033[1;32mBooking Details:\033[0m\n";
    cout << "From: " << flight.origin << "\n";
    cout << "To: " << flight.destination << "\n";
    cout << "Date: " << flight.date << "\n";
    cout << "Price: $" << flight.price << "\n";
    cout << "Airline: " << flight.airline << "\n";
    cout << "\033[1;32mFlight booked successfully!\033[0m\n";
    displayDirectFlightsonMap(flight);
    cout << "\n\tDestination REACHED";
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
    
        // Initialize HotelsList and HotelBooking for possible hotel booking
        FileHandling fileHandler(200,50); // adjust as needed
        HotelsList hotelsList(fileHandler);
        hotelsList.populateHotelsList(); // ensure this populates one hotel per city
        HotelBooking hotelBooking(hotelsList);

        while (leg && leg->next) 
        {
            
            Flight &currentFlight = leg->flight;
            Flight &nextFlight = leg->next->flight;

            int layoverTime = layoverCalculator.calculateLayoverTime(currentFlight.arrivalTime, nextFlight.departureTime);
            int hours = layoverTime / 60;
            int minutes = layoverTime % 60;

            layover.enqueue(leg->flight);

            displayInDirectFlightsOnMap(leg,routeNode);
            
            cout << "Layover at " << currentFlight.destination << ": " 
                 << hours << " hours " << minutes << " minutes.\n";
            
            pauseForTakeOff(5);
            // If layover > 12 hours (720 minutes), offer hotel booking
            if (layoverTime > 720) 
            {
                cout << "\033[1;33mLong layover detected in " << currentFlight.destination << " ("
                     << hours << "h " << minutes << "m). Would you like to book a hotel? (Y/n): \033[0m";
                char hChoice;
                cin >> hChoice;
                if (tolower(hChoice) == 'y') {
                    // Book a hotel in this city
                    hotelBooking.bookHotelInCity(currentFlight.destination);
                }
            }

            if (!layover.isEmpty()) {
                //Flight flight = layover.dequeue();
                // Process the flight (e.g., print details or add to another data structure)
                } else {
                    cout << "Layover queue is empty, cannot dequeue." << endl;
                }
            leg = leg->next;
        }
        layover.enqueue(leg->flight);
        displayInDirectFlightsOnMap(leg,routeNode);
        pauseForTakeOff(2);
        cout << "\n\tDestination REACHED";
    }
}

// Note: The user said "dont change the displayIndirect functions and keep it as same before" 
// so we are NOT modifying displayInDirectFlightsOnMap code other than the booking index issue.
// The function remains as before, just ensures no other logic changed.

void FlightBook::displayInDirectFlightsOnMap(LinkedList::FlightNode* leg, RouteList::RouteNode* curr)
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


        sf::Vector2f positionOrigin = flightGraph.getCityPosition(leg->flight.origin);
        sf::Vector2f positionDestination = flightGraph.getCityPosition(leg->flight.destination);
        sf::Vector2f scaledPos = sf::Vector2f(positionOrigin.x * windowSize.x, positionOrigin.y * windowSize.y);
        sf::Vector2f scaledDes = sf::Vector2f(positionDestination.x * windowSize.x, positionDestination.y * windowSize.y);

        airplane.startPosition = scaledPos;
        airplane.targetPosition = scaledDes;
        airplane.shape.setPosition(airplane.startPosition);
        airplane.shape.setRotation(calculateAngleDotted(airplane.startPosition, airplane.targetPosition));
        
        airplane.isMoving = true;

        // Direction vector (normalized) for the dotted line
        sf::Vector2f direction = scaledDes - scaledPos;
        float totalLength = sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= totalLength; // Normalize direction

        
        const int maxDots = 1000; 
        sf::CircleShape dots[maxDots];
        float dotSpacing = 20.0f;
        float dotRadius = 3.0f;
        float currentLength = 0.0f;

        sf::Color pathColor;
        if (leg->flight.shortest && leg->flight.cheapest)
        {
            pathColor = sf::Color::Green;
            airplane.speed = 25.f;
        }
        else if (leg->flight.cheapest)
        {
            pathColor = sf::Color::Red;
            airplane.speed = 25.f;
        }
        else if (leg->flight.shortest)
        {
            pathColor = sf::Color::Magenta;
            airplane.speed = 25.f;
        }
        else
        {
            pathColor = sf::Color::Black;
            airplane.speed = 50.f;
        }

        int activeDots = 0;

        sf::Clock clock;
        while (airplane.isMoving)
        {
            float deltaTime = clock.restart().asSeconds();
            currentLength += airplane.speed * deltaTime;

            // Update airplane position
            updateAirplanePositionDotted(airplane, deltaTime);

            // Add dots up to the current length
            activeDots = 0;
            for (float length = 0.0f; length < currentLength && length <= totalLength && activeDots < maxDots; length += dotSpacing) 
            {
                sf::Vector2f position = scaledPos + direction * length;
                dots[activeDots].setRadius(dotRadius);
                dots[activeDots].setFillColor(pathColor);
                dots[activeDots].setPosition(position - sf::Vector2f(dotRadius, dotRadius)); // Center the dot
                activeDots++;
            }

            // Stop movement if the airplane reaches the destination
            if (distance(airplane.shape.getPosition(), airplane.targetPosition) < 1.0f)
            {
                airplane.isMoving = false;
            }

            // Render the scene
            mainWindow.clear();
            mainGUI.draw();

            // Draw the growing dotted path
            for (int i = 0; i < activeDots; i++) {
                mainWindow.draw(dots[i]);
            }

            mainWindow.draw(airplane.shape); // Draw the airplane
            mainWindow.display();

            sf::sleep(sf::milliseconds(1)); // Small delay for smoother animation
        }

    
}



// Calculate the angle for airplane rotation
float FlightBook::calculateAngleDotted(const sf::Vector2f& start, const sf::Vector2f& end)
{
    sf::Vector2f delta = end - start;
    return atan2(delta.y, delta.x) * 180.f / 3.14159f; // Convert radians to degrees
}

// Update airplane position based on its speed and direction
void FlightBook::updateAirplanePositionDotted(Airplane& airplane, float deltaTime)
{
    sf::Vector2f direction = airplane.targetPosition - airplane.shape.getPosition();
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0.0f)
    {
        direction /= distance; // Normalize direction
        sf::Vector2f moveVector = direction * airplane.speed * deltaTime;

        if (distance < sqrt(moveVector.x * moveVector.x + moveVector.y * moveVector.y))
        {
            // Snap to target if overshooting
            airplane.shape.setPosition(airplane.targetPosition);
        }
        else
        {
            airplane.shape.move(moveVector);
        }
    }
}

// Calculate distance between two points
float FlightBook::distance(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}
