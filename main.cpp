/*
COMMAND FOR COMPILATION MAC NOTE - (change with location of where you installed SFML):
g++ -I/opt/homebrew/opt/sfml/include -L/opt/homebrew/opt/sfml/lib main.cpp Classes/.cpp -o FlightPathVisualizer -lsfml-graphics -lsfml-window -lsfml-system
*/

/* added by Huzaifa for compilation on my system
g++ -o FlightPathVisualizer main.cpp Classes/*.cpp -lsfml-graphics -lsfml-window -lsfml-system
*/

//23I-2001 Saad Bukhari
//23I-2017 Huzaifa Malik
//23I-2034 Abdul Rafay

#include "Classes/main.h"
#include "Classes/heap.h"
#include "Classes/stack.h"
#include "Classes/route.h"
#include "Classes/FlightGraph.h"
#include "Classes/menu.h"
#include "Classes/queue.h"
#include "Classes/layover.h" 
#include "Classes/mainGUI.h"
#include "Classes/booking.h"
#include "Classes/HotelsList.h"
#include "Classes/HotelBooking.h"
#include "Classes/utils.h"





class FlightBookingGUI 
{
private:
    sf::RenderWindow& window;
    sf::RenderWindow& mainWindow;
    MainGUI mainGUI;
    sf::Font font;
    sf::Text resultText;

    
    sf::RectangleShape originBox, destBox, dateBox,dateBox1, searchButton;
    sf::Text originText, destText, dateText,dateText1, searchText;
    string originInput, destInput, dateInput, dateInput1;
    FlightGraph& flightGraph;
    int selectedOriginIndex, selectedDestIndex;

    
    bool isOriginFocused;
    bool isDestFocused;
    bool isDateFocused;
    bool isDateFocused1;

public:
    FlightBookingGUI(sf::RenderWindow& mainwin,sf::RenderWindow& win, MainGUI& mainGUI ,FlightGraph& graph) : mainWindow(mainwin), window(win),mainGUI(mainGUI),flightGraph(graph)
    {
        selectedOriginIndex = -1;
        selectedDestIndex = -1;
        isOriginFocused = true;
        isDestFocused = false;
        isDateFocused = false;
        isDateFocused1 = false;
        
        font.loadFromFile("Assets/Aller_Bd.ttf");

        
        originBox.setSize(sf::Vector2f(200, 30));
        originBox.setPosition(100, 100);
        originBox.setFillColor(sf::Color::White);
        originText.setFont(font);
        originText.setCharacterSize(20);
        originText.setPosition(110, 110);
        originText.setFillColor(sf::Color::Black);
        originText.setString("Origin: ");

        destBox.setSize(sf::Vector2f(200, 30));
        destBox.setPosition(100, 150);
        destBox.setFillColor(sf::Color::White);
        destText.setFont(font);
        destText.setCharacterSize(20);
        destText.setPosition(110, 160);
        destText.setFillColor(sf::Color::Black);
        destText.setString("Destination: ");

        dateBox.setSize(sf::Vector2f(200, 30));
        dateBox.setPosition(100, 200);
        dateBox.setFillColor(sf::Color::White);
        dateText.setFont(font);
        dateText.setCharacterSize(20);
        dateText.setPosition(110, 210);
        dateText.setFillColor(sf::Color::Black);
        dateText.setString("From Date (DD/MM/YYYY): ");

        dateBox1.setSize(sf::Vector2f(200, 30));
        dateBox1.setPosition(100, 250);
        dateBox1.setFillColor(sf::Color::White);
        dateText1.setFont(font);
        dateText1.setCharacterSize(20);
        dateText1.setPosition(110, 260);
        dateText1.setFillColor(sf::Color::Black);
        dateText1.setString("To Date (DD/MM/YYYY): ");

        searchButton.setSize(sf::Vector2f(100, 40));
        searchButton.setPosition(100, 300);
        searchButton.setFillColor(sf::Color::Green);
        searchText.setFont(font);
        searchText.setCharacterSize(20);
        searchText.setPosition(120, 310);
        searchText.setFillColor(sf::Color::Black);
        searchText.setString("Search");
    }

    string getOriginInput() const { return originInput; }
    string getDestInput() const { return destInput; }
    string getFromDateInput() const { return dateInput; }
    string getToDateInput() const { return dateInput1; }

    void handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }

            // Text input handling
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 8) {  // Backspace
                        if (isOriginFocused && !originInput.empty()) {
                            originInput.pop_back();
                        } else if (isDestFocused && !destInput.empty()) {
                            destInput.pop_back();
                        } else if (isDateFocused && !dateInput.empty()) {
                            dateInput.pop_back();
                        } else if (isDateFocused1 && !dateInput1.empty()) {
                            dateInput1.pop_back();
                        }
                    } else if (event.text.unicode == 13) {  // Enter key
                        if (isOriginFocused) {
                            isOriginFocused = false;
                            isDestFocused = true;
                        } else if (isDestFocused) {
                            isDestFocused = false;
                            isDateFocused = true;
                        } else if (isDateFocused) {
                            isDateFocused = false;
                            isDateFocused1 = true;
                        }
                    } else {
                        // Add character to the focused input field
                        if (isOriginFocused) {
                            originInput += static_cast<char>(event.text.unicode);
                        } else if (isDestFocused) {
                            destInput += static_cast<char>(event.text.unicode);
                        } else if (isDateFocused) {
                            dateInput += static_cast<char>(event.text.unicode);
                        } else if (isDateFocused1) {
                            dateInput1 += static_cast<char>(event.text.unicode);
                        }
                    }
                }
            }

            // Handle mouse click events for shifting focus
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                // Check if click is within the Origin field
                if (originBox.getGlobalBounds().contains(mousePos)) {
                    isOriginFocused = true;
                    isDestFocused = false;
                    isDateFocused = false;
                    isDateFocused1 = false;
                }
                // Destination field
                else if (destBox.getGlobalBounds().contains(mousePos)) {
                    isOriginFocused = false;
                    isDestFocused = true;
                    isDateFocused = false;
                    isDateFocused1 = false;
                }
                // Date field 1
                else if (dateBox.getGlobalBounds().contains(mousePos)) {
                    isOriginFocused = false;
                    isDestFocused = false;
                    isDateFocused = true;
                    isDateFocused1 = false;
                }
                // Date field 2
                else if (dateBox1.getGlobalBounds().contains(mousePos)) {
                    isOriginFocused = false;
                    isDestFocused = false;
                    isDateFocused = false;
                    isDateFocused1 = true;
                }
                // Search button
                else if (searchButton.getGlobalBounds().contains(mousePos))
                {
                    if (originInput.empty() || destInput.empty() || dateInput.empty() || dateInput1.empty())
                     {
                        showErrorMessage("Please fill in all fields!");
                     } 
                    else 
                    {
                        window.close();
                    }
                }
            }
        }
    }

    void showErrorMessage(const string& message) 
    {
    sf::RectangleShape popup(sf::Vector2f(400, 100));
    popup.setFillColor(sf::Color(255, 0, 0, 180)); 
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

void displayDirectFlightsonMap(string origin, string destination, LinkedList& directFlight) 
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
    if (!font.loadFromFile("Assets/Aller_Bd.ttf")) {
        std::cerr << "Error loading font\n";
        return;
    }

    // Create a text object for displaying details
    sf::Text flightDetails;
    flightDetails.setFont(font);
    flightDetails.setCharacterSize(20);
    flightDetails.setFillColor(sf::Color::Blue);
    flightDetails.setPosition(windowSize.x - 225.f, 10.f); // Top-right corner

    LinkedList::FlightNode* currentNode = directFlight.getHead();

    while (currentNode) // Loop through each flight
    {
        airplane.speed = 100.f;
        string currentOrigin = currentNode->flight.origin;
        string currentDestination = currentNode->flight.destination;

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

        // Update flight details text
        flightDetails.setString(
            "Origin: " + currentOrigin + "\n" +
            "Destination: " + currentDestination + "\n" +
            "AirLine: " + currentNode->flight.airline + "\n" +
            "Date: " + currentNode->flight.date + "\n" +
            "Shortest: " + (currentNode->flight.shortest ? "Yes" : "No") + "\n" +
            "Cheapest: " + (currentNode->flight.cheapest ? "Yes" : "No")
        );

        // Initialize the path for the current flight
        sf::VertexArray path(sf::LineStrip);

        // Determine path color
        sf::Color pathColor;
        if (currentNode->flight.shortest && currentNode->flight.cheapest) {
            pathColor = sf::Color::Green;
            airplane.speed = 50.f;
        } else if (currentNode->flight.shortest) {
            pathColor = sf::Color::Magenta; // Shortest flight
            airplane.speed = 50.f;
        } else if (currentNode->flight.cheapest) {
            airplane.speed = 50.f;
            pathColor = sf::Color::Red; // Cheapest flight
        } else {
            pathColor = sf::Color::Black; // Default
        }

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
            mainWindow.draw(flightDetails); // Draw flight details
            mainWindow.display();

            sf::sleep(sf::milliseconds(1)); // Control update frequency
        }

        currentNode = currentNode->next; // Move to the next flight
    }
}

void displayInDirectFlightsOnMap(string origin, string destination, RouteList& indirectRoutes)
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

    // Load font for displaying text
    sf::Font font;
    if (!font.loadFromFile("Assets/Aller_Bd.ttf")) {
        std::cerr << "Error loading font\n";
        return;
    }

    // Create a text object for displaying details
    sf::Text flightDetails;
    flightDetails.setFont(font);
    flightDetails.setCharacterSize(20);
    flightDetails.setFillColor(sf::Color::Blue);
    flightDetails.setPosition(windowSize.x - 225.f, 10.f); // Top-right corner

    RouteList::RouteNode* curr = indirectRoutes.getHead();

    while (curr) // Loop through each route
    {
        airplane.speed = 100.f;
        LinkedList::FlightNode* leg = curr->route.legs.getHead();
        sf::VertexArray path(sf::LineStrip); 
        while (leg) 
        {
            airplane.speed = 100.f;

            string currentOrigin = leg->flight.origin;
            string currentDestination = leg->flight.destination;
            // Get origin and destination positions
            sf::Vector2f positionOrigin = flightGraph.getCityPosition(leg->flight.origin);
            sf::Vector2f positionDestination = flightGraph.getCityPosition(leg->flight.destination);
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
            "Date: " + leg->flight.date + "\n" +
            "AirLine: " + leg->flight.airline + "\n" +
            "Shortest: " + (curr->shortest ? "Yes" : "No") + "\n" +
            "Cheapest: " + (curr->cheapest ? "Yes" : "No")
        );
            
            airplane.isMoving = true;

            // Determine path color
            sf::Color pathColor;
            if (curr->cheapest && curr->shortest)
            {
                pathColor = sf::Color::Green;
                airplane.speed = 50.f;
            }
            else if (curr->cheapest)
            {
                pathColor = sf::Color::Red;
                airplane.speed = 50.f;
            }
            else if(curr->shortest)
            {
                pathColor = sf::Color::Magenta;
                airplane.speed = 50.f;
            }
            else
                pathColor = sf::Color::Black;

            // Animate the airplane for this leg
            sf::Clock clock;
            while (airplane.isMoving)
            {
                float deltaTime = clock.restart().asSeconds();
                updateAirplanePosition(airplane, deltaTime);

                // Add current airplane position to path
                path.append(sf::Vertex(airplane.shape.getPosition(), pathColor));

                // Clear the window and render everything
                mainWindow.clear();
                mainGUI.draw();              // Draw GUI
                mainWindow.draw(path);       // Draw the path for the current leg
                mainWindow.draw(airplane.shape); // Draw the airplane
                mainWindow.draw(flightDetails);
                mainWindow.display();

                sf::sleep(sf::milliseconds(1)); // Control update frequency
            }

            leg = leg->next; // Move to the next leg
        }

        curr = curr->next; // Move to the next route
    }
}

void displayTransitCitiesAndIndirectFlights(string origin, string destination, RouteList& indirectRoutes, string* transitCities)
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

    // Load font for displaying text
    sf::Font font;
    if (!font.loadFromFile("Assets/Aller_Bd.ttf")) {
        std::cerr << "Error loading font\n";
        return;
    }

    // Create a text object for displaying details
    sf::Text flightDetails;
    flightDetails.setFont(font);
    flightDetails.setCharacterSize(20);
    flightDetails.setFillColor(sf::Color::Blue);
    flightDetails.setPosition(windowSize.x - 225.f, 10.f); // Top-right corner
    

    RouteList::RouteNode* curr = indirectRoutes.getHead();

    // Set up the city point for drawing
    sf::CircleShape cityPoint(5.f);

    while (curr) // Loop through each route
    {
        airplane.speed = 100.f;
        LinkedList::FlightNode* leg = curr->route.legs.getHead();
        sf::VertexArray path(sf::LineStrip); 

        while (leg) 
        {
            airplane.speed = 100.f;

            string currentOrigin = leg->flight.origin;
            string currentDestination = leg->flight.destination;

            // Get origin and destination positions
            sf::Vector2f positionOrigin = flightGraph.getCityPosition(leg->flight.origin);
            sf::Vector2f positionDestination = flightGraph.getCityPosition(leg->flight.destination);
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
            "AirLine: " + leg->flight.airline + "\n" +
            "Date: " + leg->flight.date + "\n" +
            "Shortest: " + (curr->shortest ? "Yes" : "No") + "\n" +
            "Cheapest: " + (curr->cheapest ? "Yes" : "No")
        );

            airplane.isMoving = true;

            // Determine path color
            sf::Color pathColor;
            if (curr->cheapest && curr->shortest)
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

            // Animate the airplane for this leg
            sf::Clock clock;
            while (airplane.isMoving)
            {
                float deltaTime = clock.restart().asSeconds();
                updateAirplanePosition(airplane, deltaTime);

                // Add current airplane position to path
                path.append(sf::Vertex(airplane.shape.getPosition(), pathColor));

                // Clear the window and render everything
                mainWindow.clear();
                mainGUI.draw();              // Draw GUI
                mainWindow.draw(path);       // Draw the path for the current leg
                mainWindow.draw(flightDetails);
                mainWindow.draw(airplane.shape); // Draw the airplane

                // Draw the city points for the transit cities in a different color
                for (int i = 0; transitCities[i] != ""; i++) // Assuming the array is null-terminated
                {
                    if (leg->flight.origin == transitCities[i] || leg->flight.destination == transitCities[i]) 
                    {
                        cityPoint.setFillColor(sf::Color::Green); // Change color for transit cities
                    } else 
                    {
                        cityPoint.setFillColor(sf::Color::Red); // Default color for other cities
                    }

                    sf::Vector2f cityPos = flightGraph.getCityPosition(transitCities[i]);
                    sf::Vector2f scaledCityPos = sf::Vector2f(cityPos.x * windowSize.x, cityPos.y * windowSize.y);
                    cityPoint.setPosition(scaledCityPos - sf::Vector2f(cityPoint.getRadius(), cityPoint.getRadius()));
                    mainWindow.draw(cityPoint);
                }

                mainWindow.display();
                sf::sleep(sf::milliseconds(1)); // Control update frequency
            }

            leg = leg->next; // Move to the next leg
        }

        curr = curr->next; // Move to the next route
    }
}


    

void displayAvailableOptions(LinkedList &directFlights, RouteList &indirectRoutes, const string &origin, const string &destination) {
    
    if(directFlights.isEmpty()) {
        cout << "\033[1;31mNo Direct Flights available\033[0m\n";
    } 
    else {
        cout << "\n\033[1;36mDirect Flights from " << origin << " to " << destination << "\033[0m\n";
        directFlights.Display();
        displayDirectFlightsonMap(origin,destination,directFlights);
        pauseForTakeOff(5);
    }

    if(indirectRoutes.countRoutes() == 0) {
        cout << "\033[1;31mNo Indirect Flights available\033[0m\n";
        return;
    }
    else 
    {
        cout << "\n\033[1;36mIndirect Flights from " << origin << " to " << destination << "\033[0m\n";
        indirectRoutes.Display();
        displayInDirectFlightsOnMap(origin,destination,indirectRoutes);
        pauseForTakeOff(5);
    }
}

// Example preference application logic
void applyPreferences(Route &route, BookingState &currentState, RouteList &indirectRoutes) {
    int preferenceType;
    cout << "\n\t\tPreferences Menu:\n"
         << "1. Number of Transit Cities\n"
         << "2. Preferred Airline\n"
         << "3. Both Transit Cities and Airline\n"
         << "Enter your choice: ";
    cin >> preferenceType;

    string preferredAirline;
    string *transitCities = nullptr;
    int transitCount = 0;
    Menu menu;
    int cases = 0;

    LinkedList filteredFlights;    // If needed
    RouteList filteredRoutes;      // If you decide to filter multi-leg routes as well
    LinkedList directFlights; // empty
    switch (preferenceType) {
        case 1:
            transitCities = menu.takeTransitCities(transitCount);
            // Direct flights are not filtered by transit cities, only by airline

            // For multi-leg routes, you'd need to filter by transit cities
            filteredRoutes = route.filterByTransitCities(indirectRoutes, transitCities, transitCount);
            currentState.indirectRoutes = filteredRoutes;
            
            if(!filteredRoutes.isEmpty())
            {
                cases = 1;
            }
            break;
    
        case 2:
            cout << "Enter the preferred airline: ";
            cin.ignore();
            getline(cin, preferredAirline);
            
            // Filter direct flights by airline
            filteredFlights = route.listDirectFlightsWithinDataRangeandPreferredAirline(
                currentState.origin.c_str(), currentState.destination.c_str(),
                currentState.fromDate.c_str(), currentState.toDate.c_str(),
                preferredAirline
            );
            currentState.directFlights = filteredFlights;
              
            filteredRoutes = route.filterMultiLegRoutesByAirline(indirectRoutes, preferredAirline);
            currentState.indirectRoutes = filteredRoutes;

            if(!filteredRoutes.isEmpty() || (!filteredFlights.isEmpty()))
            {
                cases = 2;
            }
            break;
        case 3:
            transitCities = menu.takeTransitCities(transitCount);
            cout << "Enter the preferred airline: ";
            cin.ignore();
            getline(cin, preferredAirline);

            // Filter direct flights
            // Direct flights are not filtered by transit cities, only by airline
            filteredFlights = route.listDirectFlightsWithinDataRangeandPreferredAirline(
                currentState.origin.c_str(), currentState.destination.c_str(),
                currentState.fromDate.c_str(), currentState.toDate.c_str(),
                preferredAirline
            );
            currentState.directFlights = filteredFlights;

            // Filter multi-leg routes
            filteredRoutes = route.filterByTransitCitiesAndAirline(indirectRoutes, preferredAirline, transitCities, transitCount);
            currentState.indirectRoutes = filteredRoutes;

            if(!filteredRoutes.isEmpty())
            {
                cases = 3;             
            }    

            break;
        default:
            cout << "Invalid choice!\n";
            return;
    }

    // creating sub graph
    currentState.directFlights = filteredFlights;
    currentState.indirectRoutes = filteredRoutes;

    FlightGraph subGraph(flightGraph.getNumVertices(), flightGraph.getFileHandler());
    subGraph.clear();

    {
        LinkedList::FlightNode* fnode = filteredFlights.getHead();
        while (fnode) {
            subGraph.addFlight(
                fnode->flight.origin,
                fnode->flight.destination,
                fnode->flight.airline,
                fnode->flight.date,
                fnode->flight.departureTime,
                fnode->flight.arrivalTime,
                fnode->flight.price,
                fnode->flight.distance
            );
            fnode = fnode->next;
        }
    }

    {
        RouteList::RouteNode* rnode = filteredRoutes.getHead();
        while (rnode) {
            LinkedList::FlightNode* leg = rnode->route.legs.getHead();
            while (leg) {
                subGraph.addFlight(
                    leg->flight.origin,
                    leg->flight.destination,
                    leg->flight.airline,
                    leg->flight.date,
                    leg->flight.departureTime,
                    leg->flight.arrivalTime,
                    leg->flight.price,
                    leg->flight.distance
                );
                leg = leg->next;
            }
            rnode = rnode->next;
             
        }
    }

    // Create a new Route object tied to subGraph
    Route subRoute(subGraph);

    // Recalculate shortest and cheapest paths on the subgraph
    cout << "\nRecalculating shortest and cheapest paths on the filtered subgraph...\n";
    subRoute.shortestPath(
        currentState.origin.c_str(),
        currentState.destination.c_str(),
        currentState.fromDate.c_str(),
        currentState.toDate.c_str(),
        currentState.directFlights,
        currentState.indirectRoutes
    );

    subRoute.cheapestFlight(
        currentState.origin.c_str(),
        currentState.destination.c_str(),
        currentState.fromDate.c_str(),
        currentState.toDate.c_str(),
        currentState.directFlights,
        currentState.indirectRoutes
    );

    if(cases == 1 || cases == 3)
    {
        displayTransitCitiesAndIndirectFlights(originInput.c_str(),destInput.c_str(),currentState.indirectRoutes,transitCities);
    }
    else if(cases == 2)
    {
        if(!currentState.directFlights.isEmpty())
            displayDirectFlightsonMap(originInput.c_str(),destInput.c_str(),currentState.directFlights);
        if(!currentState.indirectRoutes.isEmpty())
            displayInDirectFlightsOnMap(originInput.c_str(),destInput.c_str(),currentState.indirectRoutes);
    }

}

void handleSearch(const string& origin, const string& destination, const string& fromDate, const string& toDate, Stack& bookingStack) {
    Route route(flightGraph);
    FlightBook flightBook(mainWindow,window, mainGUI,flightGraph);

    // Initial State
    BookingState currentState(origin, destination, fromDate, toDate);

    // Get initial direct flights and multi-leg routes
    LinkedList directFlights = route.listDirectFlightsWithinDateRange(origin.c_str(), destination.c_str(), fromDate.c_str(), toDate.c_str());
    RouteList indirectRoutes = route.listIndirectRoutesWithinDateRange(origin.c_str(), destination.c_str(), fromDate.c_str(), toDate.c_str());

    // Store in currentState
    currentState.directFlights = directFlights;
    currentState.indirectRoutes = indirectRoutes;

    bookingStack.Push(currentState);

    if (directFlights.isEmpty() && (indirectRoutes.countRoutes() == 0)) {
        cout << "No flights or routes found for the given criteria\n";
        return;
    }

    route.shortestPath(origin.c_str(), destination.c_str(), fromDate.c_str(), toDate.c_str(), currentState.directFlights, currentState.indirectRoutes);
    route.cheapestFlight(origin.c_str(), destination.c_str(), fromDate.c_str(), toDate.c_str(), currentState.directFlights, currentState.indirectRoutes);
    displayAvailableOptions(currentState.directFlights, currentState.indirectRoutes, origin, destination);

preferencesundone:

    // Ask if user wants preferences
    cout << "\nWould you like to apply any preferences? (Y/n): ";
    char choice;
    cin >> choice;
    if (tolower(choice) == 'y') 
    {
        applyPreferences(route, currentState, indirectRoutes);
        bookingStack.Push(currentState);
        // After applying preferences, display updated options
       //displayAvailableOptions(currentState.directFlights, currentState.indirectRoutes, origin, destination);
    }
        
    // Undo Preferences
    cout << "Would you like to undo preferences? (Y/n): ";
    cin >> choice;
    if (tolower(choice) == 'y') 
    {
        if (!bookingStack.IsEmpty()) 
        {
            bookingStack.Pop(); // remove current (with prefs)
            if (!bookingStack.IsEmpty()) 
            {
                currentState = bookingStack.Top();;
                cout << "Preferences undone. Showing flights/routes from the previous state:\n";
                route.shortestPath(origin.c_str(), destination.c_str(), fromDate.c_str(), toDate.c_str(), currentState.directFlights, currentState.indirectRoutes);
                route.cheapestFlight(origin.c_str(), destination.c_str(), fromDate.c_str(), toDate.c_str(), currentState.directFlights, currentState.indirectRoutes);
                displayAvailableOptions(currentState.directFlights, currentState.indirectRoutes, origin, destination);
                goto preferencesundone;
            } 
            else 
            {
                cout << "No previous state to revert to.\n";
            }
        } else {
            cout << "No preferences to undo.\n";
        }
    }

    // Now let the user book
    cout << "\nWould you like to book a flight/route? (Y/n): ";
    cin >> choice;
    if (tolower(choice) == 'y') 
    {
        flightBook.bookFlightOption(originInput,destInput,currentState.directFlights, currentState.indirectRoutes);
    }
}
    
    
    void draw() 
    {
        window.clear(sf::Color::White);
        // Draw the UI elements
        window.draw(originBox);
        window.draw(originText);
        window.draw(destBox);
        window.draw(destText);
        window.draw(dateBox);
        window.draw(dateText);
        window.draw(dateBox1);
        window.draw(dateText1);
        window.draw(searchButton);
        window.draw(searchText);

        // Draw the input text in the boxes
        sf::Text inputOriginText;
        inputOriginText.setFont(font);
        inputOriginText.setCharacterSize(20);
        inputOriginText.setString(originInput);
        inputOriginText.setPosition(200, 110);
        inputOriginText.setFillColor(sf::Color::Black);
        window.draw(inputOriginText);

        sf::Text inputDestText;
        inputDestText.setFont(font);
        inputDestText.setCharacterSize(20);
        inputDestText.setString(destInput);
        inputDestText.setPosition(240, 160);
        inputDestText.setFillColor(sf::Color::Black);
        window.draw(inputDestText);

        sf::Text inputDateText;
        inputDateText.setFont(font);
        inputDateText.setCharacterSize(20);
        inputDateText.setString(dateInput);
        inputDateText.setPosition(350, 210);
        inputDateText.setFillColor(sf::Color::Black);
        window.draw(inputDateText);

        sf::Text inputDateText1;
        inputDateText1.setFont(font);
        inputDateText1.setCharacterSize(20);
        inputDateText1.setString(dateInput1);
        inputDateText1.setPosition(350, 260);
        inputDateText1.setFillColor(sf::Color::Black);
        window.draw(inputDateText1);

        window.display();
    }

    void run() 
    {
        handleInput();
        draw();
    }
};



class FlightVisualizerApp {
private:
    sf::RenderWindow mainWindow; 
    FlightGraph flightGraph;
    FileHandling fileHandler;

public:
    FlightVisualizerApp()
        : mainWindow(sf::VideoMode(1024, 768), "Flight Graph Visualization"),
          flightGraph(50, fileHandler),
          fileHandler(200, 50) {
        // Initialize graph
        flightGraph.populateGraph();
    }

    void run() 
    {
        MainGUI mainGUI(mainWindow, flightGraph);
       

        while (mainWindow.isOpen()) 
        {
            // Display main graph with Book Flight option
            mainGUI.draw();
            bool bookFlightPressed = mainGUI.handleEvents();
            

            if (bookFlightPressed) 
            {
                // display new window after button is pressed
                sf::RenderWindow bookingWindow(sf::VideoMode(800, 600), "Flight Booking");
                FlightBookingGUI bookingGUI(mainWindow,bookingWindow,mainGUI, flightGraph);
                
                while (bookingWindow.isOpen())
                {
                    bookingGUI.run();
                }
                
                // Retrieve inputs after booking window closes
                string origin = bookingGUI.getOriginInput();
                string destination = bookingGUI.getDestInput();
                string fromDate = bookingGUI.getFromDateInput();
                string toDate = bookingGUI.getToDateInput();

                Stack bookingStack;
                bookingGUI.handleSearch(origin, destination, fromDate, toDate , bookingStack);
               }
                }
            
        }
    
};

int main() {
    Menu menu;

    menu.displayHeader();

    FlightVisualizerApp app;
    app.run();
}
