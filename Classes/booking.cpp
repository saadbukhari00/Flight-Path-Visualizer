#include "booking.h"
#include "layover.h"       // Ensure Layover class is included
#include "HotelsList.h"    // Ensure HotelsList class is included
#include "HotelBooking.h"  // Ensure HotelBooking class is included
#include "FileHandling.h"  // For fileHandler if needed


//Constructor
FlightBook::FlightBook(sf::RenderWindow& main, sf::RenderWindow& win, MainGUI& mainG, FlightGraph& flightG) 
    : mainWindow(main), window(win), mainGUI(mainG), flightGraph(flightG) {} 


// Function to validate fields for payment system
bool FlightBook::validateFields(const std::string& cardNumber, const std::string& name, const std::string& expiryDate, const std::string& cvc) 
{
    if (cardNumber.empty() || name.empty() || expiryDate.empty() || cvc.empty()) 
    {
        return false;
    }

    return true;
}

// Function to book a flight option
void FlightBook::bookFlightOption(string& org, string& dest, LinkedList &directFlights, RouteList &indirectRoutes) 
{
    origin = org;
    destination = dest;

    // Display direct flights
    cout << "\n\033[1;36mAvailable Direct Flights:\033[0m\n";
    directFlights.Display();
    int directCount = directFlights.size();

    if (directCount <= 0) 
    {
        cout << "\033[1;31mNo Direct Flights available.\033[0m\n";
    }

    // Display indirect routes
    cout << "\n\033[1;36mAvailable Indirect Routes (Multi-leg Journeys):\033[0m\n";
    indirectRoutes.DisplayWithIndexOffset(directCount);
    int routeCount = indirectRoutes.countRoutes();

    if(routeCount <= 0) 
    {
        cout << "\033[1;31mNo Indirect Routes available.\033[0m\n";
    }

    int totalOptions = directCount + routeCount;
    if (totalOptions == 0) 
    {
        cout << "No flights/routes available to book.\n";
        return;
    }

    cout << "\nEnter the index of the flight/route you want to book: ";
    int choice;
    cin >> choice;

    if (choice < 0 || choice >= totalOptions) 
    {
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
        if (!routeNode)
         {
            cout << "\033[1;31mInvalid route index.\033[0m\n";
            return;
        }

        // Book all flights in this multi-leg route
        confirmBooking(routeNode->route.legs, routeNode); 
    }

   // Create the main window
sf::RenderWindow window(sf::VideoMode(800, 600), "Payment Form");

sf::Font font;
if (!font.loadFromFile("Assets/Aller_Bd.ttf")) 
{
    std::cerr << "Error loading font\n";
    return;
}

sf::RectangleShape cardNumberBox, nameBox, expiryDateBox, cvcBox, submitButton;
sf::Text cardNumberText, nameText, expiryDateText, cvcText, submitText, feedback;

// Set up the card number box and label
cardNumberBox.setSize(sf::Vector2f(300, 30));
cardNumberBox.setPosition(100, 100);
cardNumberBox.setFillColor(sf::Color::White);
cardNumberText.setFont(font);
cardNumberText.setCharacterSize(20);
cardNumberText.setPosition(110, 110);
cardNumberText.setFillColor(sf::Color::Black);
cardNumberText.setString("Card Number: ");

// Set up the name box and label
nameBox.setSize(sf::Vector2f(300, 30));
nameBox.setPosition(100, 150);
nameBox.setFillColor(sf::Color::White);
nameText.setFont(font);
nameText.setCharacterSize(20);
nameText.setPosition(110, 160);
nameText.setFillColor(sf::Color::Black);
nameText.setString("Name on Card: ");

// Set up the expiry date box and label
expiryDateBox.setSize(sf::Vector2f(150, 30));
expiryDateBox.setPosition(100, 200);
expiryDateBox.setFillColor(sf::Color::White);
expiryDateText.setFont(font);
expiryDateText.setCharacterSize(20);
expiryDateText.setPosition(110, 210);
expiryDateText.setFillColor(sf::Color::Black);
expiryDateText.setString("Expiry Date (MM/YY): ");

// Set up the CVC box and label
cvcBox.setSize(sf::Vector2f(100, 30));
cvcBox.setPosition(100, 250);
cvcBox.setFillColor(sf::Color::White);
cvcText.setFont(font);
cvcText.setCharacterSize(20);
cvcText.setPosition(110, 260);
cvcText.setFillColor(sf::Color::Black);
cvcText.setString("CVC: ");

// Set up the submit button
submitButton.setSize(sf::Vector2f(100, 40));
submitButton.setPosition(100, 300);
submitButton.setFillColor(sf::Color::Green);
submitText.setFont(font);
submitText.setCharacterSize(20);
submitText.setPosition(120, 310);
submitText.setFillColor(sf::Color::Black);
submitText.setString("Submit");

// Set up the feedback message
feedback.setFont(font);
feedback.setCharacterSize(30);
feedback.setPosition(50, 500);
feedback.setFillColor(sf::Color::White);

// Input variables
string cardNumber, name, expiryDate, cvc;

// Input focus flags
bool isCardNumberFocused = true;
bool isNameFocused = false;
bool isExpiryDateFocused = false;
bool isCVCFocused = false;

while (window.isOpen()) 
{
    sf::Event event;
    while (window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) 
        {
            window.close();
        } 
        else if (event.type == sf::Event::TextEntered) 
        {
            if (event.text.unicode < 128) 
            { // ASCII character check
                if (event.text.unicode == 8) 
                { // Backspace
                    if (isCardNumberFocused && !cardNumber.empty())
                    {
                        cardNumber.pop_back();
                    } 
                    else if (isNameFocused && !name.empty()) 
                    {
                        name.pop_back();
                    } 
                    else if (isExpiryDateFocused && !expiryDate.empty()) 
                    {
                        expiryDate.pop_back();
                    } 
                    else if (isCVCFocused && !cvc.empty()) 
                    {
                        cvc.pop_back();
                    }
                } 
                else if (event.text.unicode == 13)
                 { // Enter key
                    if (isCardNumberFocused) 
                    {
                        isCardNumberFocused = false;
                        isNameFocused = true;
                    } 
                    else if (isNameFocused) 
                    {
                        isNameFocused = false;
                        isExpiryDateFocused = true;
                    } 
                    else if (isExpiryDateFocused) 
                    {
                        isExpiryDateFocused = false;
                        isCVCFocused = true;
                    }
                } 
                else 
                {
                    if (isCardNumberFocused) 
                    {
                        cardNumber += static_cast<char>(event.text.unicode);
                    } 
                    else if (isNameFocused) 
                    {
                        name += static_cast<char>(event.text.unicode);
                    } 
                    else if (isExpiryDateFocused) 
                    {
                        expiryDate += static_cast<char>(event.text.unicode);
                    } 
                    else if (isCVCFocused) 
                    {
                        cvc += static_cast<char>(event.text.unicode);
                    }
                }
            }
        } 
        else if (event.type == sf::Event::MouseButtonPressed)
         {
            // Handle focus shifts based on mouse click
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (cardNumberBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                isCardNumberFocused = true;
                isNameFocused = false;
                isExpiryDateFocused = false;
                isCVCFocused = false;
            } else if (nameBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                isCardNumberFocused = false;
                isNameFocused = true;
                isExpiryDateFocused = false;
                isCVCFocused = false;
            } else if (expiryDateBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                isCardNumberFocused = false;
                isNameFocused = false;
                isExpiryDateFocused = true;
                isCVCFocused = false;
            } else if (cvcBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                isCardNumberFocused = false;
                isNameFocused = false;
                isExpiryDateFocused = false;
                isCVCFocused = true;
            } else if (submitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                if (validateFields(cardNumber, name, expiryDate, cvc)) {
                    showMessage("Payment Successful",window,sf::Color::Green);
                    window.close();
                } else {
                    showMessage("Fill up all fields",window,sf::Color::Red);
                }
            }
        }
    }

    // Clear the window
    window.clear(sf::Color::White);

    // Draw UI elements
    window.draw(cardNumberBox);
    window.draw(cardNumberText);
    window.draw(nameBox);
    window.draw(nameText);
    window.draw(expiryDateBox);
    window.draw(expiryDateText);
    window.draw(cvcBox);
    window.draw(cvcText);
    window.draw(submitButton);
    window.draw(submitText);

    // Draw the input text in the boxes
    sf::Text inputOriginText;
    inputOriginText.setFont(font);
    inputOriginText.setCharacterSize(20);
    inputOriginText.setString(cardNumber);
    inputOriginText.setPosition(250, 110);
    inputOriginText.setFillColor(sf::Color::Black);
    window.draw(inputOriginText);

    sf::Text inputDestText;
    inputDestText.setFont(font);
    inputDestText.setCharacterSize(20);
    inputDestText.setString(name);
    inputDestText.setPosition(250, 160);
    inputDestText.setFillColor(sf::Color::Black);
    window.draw(inputDestText);

    sf::Text inputDateText;
    inputDateText.setFont(font);
    inputDateText.setCharacterSize(20);
    inputDateText.setString(expiryDate);
    inputDateText.setPosition(310, 210);
    inputDateText.setFillColor(sf::Color::Black);
    window.draw(inputDateText);

    sf::Text inputDateText1;
    inputDateText1.setFont(font);
    inputDateText1.setCharacterSize(20);
    inputDateText1.setString(cvc);
    inputDateText1.setPosition(200, 260);
    inputDateText1.setFillColor(sf::Color::Black);
    window.draw(inputDateText1);


    // Display everything
    window.display();
}


}


void FlightBook::showMessage(const string& message,sf::RenderWindow& window,sf::Color color) 
    {
        sf::Font font;
        if (!font.loadFromFile("Assets/Aller_Bd.ttf")) {
            cerr << "Error loading font\n";
            return;
        }
    sf::RectangleShape popup(sf::Vector2f(400, 100));
    popup.setFillColor(color); 
    popup.setPosition(100, 200);  

   
    sf::Text errorText(message, font, 20);
    errorText.setFillColor(sf::Color::White);
    errorText.setPosition(110, 220); 

    window.draw(popup);
    window.draw(errorText);
    window.display();

    sf::Clock clock; 
    while (clock.getElapsedTime().asSeconds() < 5) 
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
        }
    }
    
    window.clear(sf::Color::White);  // Clear the window and proceed to next frame
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

    // Load font for displaying text
    sf::Font font;
    if (!font.loadFromFile("Assets/Aller_Bd.ttf")) 
    {
        std::cerr << "Error loading font\n";
        return;
    }

    // Create a text object for displaying details
    sf::Text flightDetails;
    flightDetails.setFont(font);
    flightDetails.setCharacterSize(20);
    flightDetails.setFillColor(sf::Color::Blue);
    flightDetails.setPosition(windowSize.x - 225.f, 10.f); // Top-right corner

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

        flightDetails.setString(
            "Origin: " + currentOrigin + "\n" +
            "Destination: " + currentDestination + "\n" +
            "AirLine: " + flight.airline + "\n" +
            "Date: " + flight.date + "\n" +
            "Arrival Time: " + flight.arrivalTime + "\n" +
            "Departure Time: " + flight.departureTime + "\n" +
            "Shortest: " + (flight.shortest ? "Yes" : "No") + "\n" +
            "Cheapest: " + (flight.cheapest ? "Yes" : "No")
        );

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
            mainWindow.draw(flightDetails);
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

void FlightBook::confirmBooking(LinkedList &legs, RouteList::RouteNode* routeNode) 
{
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
        
        Layover layoverCalculator; // For calculating layover times
        LinkedList::FlightNode* leg = legs.getHead();
    
        // Initialize HotelsList and HotelBooking for possible hotel booking
        FileHandling fileHandler(200,50); 
        HotelsList hotelsList(fileHandler);
        hotelsList.populateHotelsList();
        HotelBooking hotelBooking(hotelsList);
        int hours;
        int minutes;
        while (leg && leg->next) 
        {
            
            Flight &currentFlight = leg->flight;
            Flight &nextFlight = leg->next->flight;

            int layoverTime = layoverCalculator.calculateLayoverTime(currentFlight.arrivalTime, nextFlight.departureTime);
            hours = layoverTime / 60;
            minutes = layoverTime % 60;

            layover.enqueue(leg->flight);

            displayInDirectFlightsOnMap(leg,routeNode,hours,minutes);
            
            
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

            if (!layover.isEmpty()) 
            {
                Flight flight = layover.dequeue();
               
            } 
            else 
            {
                    cout << "Layover queue is empty, cannot dequeue." << endl;
            }
            leg = leg->next;
        }
        layover.enqueue(leg->flight);
        displayInDirectFlightsOnMap(leg,routeNode,0,0);
        pauseForTakeOff(2);
        cout << "\n\tDestination REACHED";
    }
}
 
void FlightBook::displayInDirectFlightsOnMap(LinkedList::FlightNode* leg, RouteList::RouteNode* curr, int hours, int minutes)
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

    // Load font for displaying text
    sf::Font font;
    if (!font.loadFromFile("Assets/Aller_Bd.ttf")) 
    {
        std::cerr << "Error loading font\n";
        return;
    }

    // Create a text object for displaying details
    sf::Text flightDetails;
    flightDetails.setFont(font);
    flightDetails.setCharacterSize(20);
    flightDetails.setFillColor(sf::Color::Blue);
    flightDetails.setPosition(windowSize.x - 225.f, 10.f); // Top-right corner

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
    if (curr->cheapest && curr->shortest)
    {
        pathColor = sf::Color::Green;
        airplane.speed = 25.f;
    }
    else if (curr->cheapest)
    {
        pathColor = sf::Color::Red;
        airplane.speed = 25.f;
    }
    else if (curr->shortest)
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
    string destination = leg->flight.destination;
    string currentOrigin = leg->flight.origin;
    // Create the text object for displaying details
    sf::Text layoverDetails;
    layoverDetails.setFont(font); // Make sure the font is set properly
    layoverDetails.setCharacterSize(20);
    layoverDetails.setFillColor(sf::Color::Red);
    layoverDetails.setString("Layover at " + destination + ": \n" + 
                        to_string(hours) + " hours " + 
                        to_string(minutes) + " minutes.");

    flightDetails.setString(
            "Origin: " + currentOrigin + "\n" +
            "Destination: " + destination + "\n" +
            "AirLine: " + leg->flight.airline + "\n" +
            "Date: " + leg->flight.date + "\n" +
            "Arrival Time: " + leg->flight.arrivalTime + "\n" +
            "Departure Time: " + leg->flight.departureTime + "\n" +
            "Shortest: " + (curr->shortest ? "Yes" : "No") + "\n" +
            "Cheapest: " + (curr->cheapest ? "Yes" : "No")
        );

    
    layoverDetails.setPosition(windowSize.x - 350.f, 10.f); // Top-right corner

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
        
        // Draw the flight details text after the airplane stops moving
        if (!airplane.isMoving)
        {
            mainWindow.draw(layoverDetails);
        }
        else
        {
            mainWindow.draw(flightDetails);
        }

        mainWindow.display();
        sf::sleep(sf::milliseconds(1)); // Small delay for smoother animation
    }
    pauseForTakeOff(5);
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
