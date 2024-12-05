/*
COMMAND FOR COMPILATION MAC NOTE - (change with location of where you installed SFML):
g++ -I/opt/homebrew/opt/sfml/include -L/opt/homebrew/opt/sfml/lib main.cpp Classes/.cpp -o FlightPathVisualizer -lsfml-graphics -lsfml-window -lsfml-system
*/

/* added by Huzaifa for compilation on my system
 g++ -o FlightPathVisualizer main.cpp Classes/FileHandling.cpp Classes/FlightGraph.cpp Classes/route.cpp Classes/list.cpp Classes/menu.cpp -lsfml-graphics -lsfml-window -lsfml-system
*/

#include "Classes/main.h"
#include "Classes/heap.h"
#include "Classes/stack.h"
#include "Classes/queue.h"
#include "Classes/route.h"
#include "Classes/FlightGraph.h"
#include "Classes/menu.h"



// airplane struct to store airplane information
struct Airplane 
{
    sf::ConvexShape shape;      // The triangle to represent the airplane
    sf::Vector2f startPosition;
    sf::Vector2f targetPosition;
    float speed;
    bool isMoving;
};

// Function to calculate the angle (in degrees) between two points
float calculateAngle(const sf::Vector2f& start, const sf::Vector2f& target) 
{
    return atan2(target.y - start.y, target.x - start.x) * 180.f / 3.14159f;
}

// Function to normalize a vector (make its magnitude = 1)
sf::Vector2f normalize(const sf::Vector2f& vector) 
{
    float magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);
    return magnitude == 0 ? sf::Vector2f(0, 0) : sf::Vector2f(vector.x / magnitude, vector.y / magnitude);
}

// Update airplane position based on direction and speed
void updateAirplanePosition(Airplane& airplane, float deltaTime) {
    if (airplane.isMoving) 
    {
        sf::Vector2f direction = normalize(airplane.targetPosition - airplane.shape.getPosition());
        airplane.shape.move(direction * airplane.speed * deltaTime);

        // Check if the airplane has reached or is very close to the target position
        sf::Vector2f currentPosition = airplane.shape.getPosition();
        if (fabs(currentPosition.x - airplane.targetPosition.x) < 1.f &&
            fabs(currentPosition.y - airplane.targetPosition.y) < 1.f) 
            {
            airplane.shape.setPosition(airplane.targetPosition);
            airplane.isMoving = false;
        }
    }
}


class FlightBookingGUI 
{
private:
    sf::RenderWindow& window;
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
    FlightBookingGUI(sf::RenderWindow& win, FlightGraph& graph) : window(win), flightGraph(graph)
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
            if (event.type == sf::Event::Closed) {
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
                        handleSearch();
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

    void handleSearch()
    {
        Route route(flightGraph);

        // Display flights in terminal
        LinkedList indrectFlights = route.listAllFlightsWithinDateRange(originInput.c_str(), destInput.c_str(), dateInput.c_str(), dateInput1.c_str());
        
        route.listShortestAndCheapest(originInput.c_str(), destInput.c_str());

        Menu menu;
        char choice;
        
        cout << "\033[1;34m\n\tDo you have any preferences on your flight (Y/n) ?\033[0m";
        cin >> choice;

        if(choice == 'Y' || choice == 'y')
        {
            int ch;
            string airline;
            string * TC;
            int count = 0;
            cout << "\033[1;33m\n\t\tPREFERENCES";
            cout << "\n\t\t1. Number of Transit Cities";
            cout << "\n\t\t2. Preferred Airline";
            cout << "\n\t\t3. Both";
            cout << "\n\t\tEnter your choice: \033[0m";
            cin >> ch;

            switch(ch)
            {
                case 1:
                    TC = menu.takeTransitCities(count);
                    route.findFlightsWithTransitCities(originInput.c_str(), destInput.c_str(), dateInput.c_str(), dateInput1.c_str(), TC, count);
                    break;

                case 2:
                    cout << "\033[1;34m\n\tEnter the name of the airline: \033[0m";
                    getline(cin, airline);
                    route.listAllFlightsWithinDataRangeandPreferredAirline(originInput.c_str(), destInput.c_str(), dateInput.c_str(), dateInput1.c_str(), airline);
                    break;
                
                case 3:
                    TC = menu.takeTransitCities(count);
                    cout << "\033[1;34m\n\tEnter the name of the airline: \033[0m";
                    getline(cin, airline);
                    route.filterByTransitCitiesAndAirline(originInput.c_str(), destInput.c_str(), dateInput.c_str(), dateInput1.c_str(), TC, count, airline);
                    break;

                default:
                    cout << "\033[1;31m\n\tInvalid choice!\033[0m";
                    break;
            }
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

class MainGUI {
private:
    sf::RenderWindow& mainWindow;
    sf::Font font;
    sf::RectangleShape bookFlightButton;
    sf::Text buttonText;
    FlightGraph& flightGraph;
    Airplane airplane; // Adding airplane as a member variable

public:
    MainGUI(sf::RenderWindow& window, FlightGraph& graph)
        : mainWindow(window), flightGraph(graph), airplane() {
        font.loadFromFile("Assets/Aller_Bd.ttf");

        // Configure the Book Flight button
        bookFlightButton.setSize(sf::Vector2f(150, 50));
        bookFlightButton.setPosition(10, 10);
        bookFlightButton.setFillColor(sf::Color::Green);

        buttonText.setFont(font);
        buttonText.setCharacterSize(20);
        buttonText.setFillColor(sf::Color::Black);
        buttonText.setString("Book Flight");
        buttonText.setPosition(25, 20);
    }

    bool handleEvents() {
        sf::Event event;
        while (mainWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mainWindow.close();
                return false;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (bookFlightButton.getGlobalBounds().contains(mousePos)) {
                    return true; // Switch to booking window
                }
            }
        }
        return false;
    }

    void addAirplane(const Airplane& newAirplane)
    {
        airplane = newAirplane; // Store the airplane passed from the search
    }


    void draw() 
    {
        mainWindow.clear(sf::Color::White);

        // Draw flight graph
        sf::Texture mapTexture;
        mapTexture.loadFromFile("Assets/world_map.png");
        flightGraph.displayOnMap(mainWindow, mapTexture);

        // Draw Book Flight button
        mainWindow.draw(bookFlightButton);
        mainWindow.draw(buttonText);
        mainWindow.display();
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
                FlightBookingGUI bookingGUI(bookingWindow, flightGraph);
                
                while (bookingWindow.isOpen())
                {
                    bookingGUI.run();
                }
                
                // Retrieve inputs after booking window closes
                string origin = bookingGUI.getOriginInput();
                string destination = bookingGUI.getDestInput();
                string fromDate = bookingGUI.getFromDateInput();
                string toDate = bookingGUI.getToDateInput();

                Route route(flightGraph);
                LinkedList indirectFlights = route.listAllFlightsWithinDateRange(origin.c_str(), destination.c_str(), fromDate.c_str(), toDate.c_str());
                cout<<"here";
                indirectFlights.Display();

                sf::Vector2u windowSize = mainWindow.getSize();


if (!indirectFlights.isEmpty())
{
    // Initialize the airplane
    Airplane airplane;
    airplane.shape.setPointCount(3);
    airplane.shape.setPoint(0, sf::Vector2f(0, -10)); // Top vertex of the triangle
    airplane.shape.setPoint(1, sf::Vector2f(-5, 10)); // Bottom left vertex
    airplane.shape.setPoint(2, sf::Vector2f(5, 10)); // Bottom right vertex
    airplane.shape.setFillColor(sf::Color::Red);
    airplane.speed = 200.f; // Speed of the airplane
    airplane.isMoving = true;

    sf::VertexArray path(sf::LineStrip); // To trace the path

    // Linked list traversal
    LinkedList::FlightNode* currentNode = indirectFlights.getHead(); // Start from the head of the linked list

    while (currentNode) // While there are waypoints
    {
        sf::Vector2f positionOrigin = flightGraph.getCityPosition(currentNode->flight.origin);        // Origin
        sf::Vector2f positionDestination = flightGraph.getCityPosition(currentNode->flight.destination); // Destination
        
        sf::Vector2f scaledPos = sf::Vector2f(positionOrigin.x * windowSize.x, positionOrigin.y * windowSize.y);
        sf::Vector2f scaledDes = sf::Vector2f(positionDestination.x * windowSize.x, positionDestination.y * windowSize.y);

        airplane.startPosition = scaledPos;
        airplane.targetPosition = scaledDes;
        airplane.shape.setPosition(airplane.startPosition);
        airplane.shape.setRotation(calculateAngle(airplane.startPosition, airplane.targetPosition));

        // Move airplane from current origin to destination
        sf::Clock clock;
        airplane.isMoving= true;
        while (airplane.isMoving)
        {
            float deltaTime = clock.restart().asSeconds();
            updateAirplanePosition(airplane, deltaTime);

            // Add current position to path for visualization
            path.append(sf::Vertex(airplane.shape.getPosition(), sf::Color::Black));

            // Clear the window and draw the airplane
            mainWindow.clear();
            mainGUI.draw();
            mainWindow.draw(path);         // Draw the path
            mainWindow.draw(airplane.shape); // Draw the airplane
            mainWindow.display();

            sf::sleep(sf::milliseconds(1)); // Control update frequency
        }

        // Move to the next node in the linked list
        currentNode = currentNode->next;
    }
}               
                char ch;
                cout<<"Proceed to see direct path (y)";
                cin >> ch;
                Airplane airplane;
                airplane.shape.setPointCount(3);
                airplane.shape.setPoint(0, sf::Vector2f(0, -10)); // Top vertex of the triangle
                airplane.shape.setPoint(1, sf::Vector2f(-5, 10)); // Bottom left vertex
                airplane.shape.setPoint(2, sf::Vector2f(5, 10)); // Bottom right vertex
                airplane.shape.setFillColor(sf::Color::Red);
                sf::Vector2f positionOrigin = flightGraph.getCityPosition(origin);
                sf::Vector2f positionDestination = flightGraph.getCityPosition(destination);
                sf::Vector2f scaledPos = sf::Vector2f(positionOrigin.x * windowSize.x, positionOrigin.y * windowSize.y);
                sf::Vector2f scaledDes = sf::Vector2f(positionDestination.x * windowSize.x, positionDestination.y * windowSize.y);
                airplane.speed = 25.f; // Speed of the airplane
                airplane.isMoving = true;
                airplane.targetPosition = scaledDes;
                airplane.startPosition = scaledPos;
                airplane.shape.setPosition(airplane.startPosition);
                airplane.shape.setRotation(calculateAngle(airplane.startPosition, airplane.targetPosition));

                mainGUI.addAirplane(airplane);
                sf::Clock clock;
                sf::VertexArray path(sf::LineStrip); // To trace the path

                while (airplane.isMoving) // Main loop for updating and rendering
                {
                    float deltaTime = clock.restart().asSeconds();
                    updateAirplanePosition(airplane, deltaTime);

                    // Clear the window and draw the airplane
                    path.append(sf::Vertex(airplane.shape.getPosition(), sf::Color::Blue));
                    mainGUI.draw();
                    mainWindow.draw(path);
                    mainWindow.draw(airplane.shape);
                    mainWindow.display();

                    sf::sleep(sf::milliseconds(1)); // sleep to control frequency
                }
                cout<<"Proceed";
                cin>>ch;
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
