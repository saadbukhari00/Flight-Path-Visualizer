/*
COMMAND FOR COMPILATION MAC NOTE - (change with location of where you installed SFML):
g++ -I/opt/homebrew/opt/sfml/include -L/opt/homebrew/opt/sfml/lib main.cpp Classes/.cpp -o FlightPathVisualizer -lsfml-graphics -lsfml-window -lsfml-system
*/

/* added by Huzaifa for compilation on my system
 g++ -o FlightPathVisualizer main.cpp Classes/FileHandling.cpp Classes/FlightGraph.cpp -lsfml-graphics -lsfml-window -lsfml-system
*/

#include "Classes/main.h"
#include "Classes/list.h"
#include "Classes/heap.h"
#include "Classes/stack.h"
#include "Classes/queue.h"
#include "Classes/route.h"
#include "Classes/FlightGraph.h"

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
                } else {
                    handleSearch();  
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
        Route route(flightGraph, window, flightGraph.getMapTexture());

        // Display flights in terminal
        route.listAllFlightsWithinDateRange(originInput.c_str(), destInput.c_str(), dateInput.c_str(), dateInput1.c_str());
        route.displayFlight(originInput.c_str(), destInput.c_str());
        
        route.listShortestAndCheapest(originInput.c_str(), destInput.c_str());

        // Highlight the shortest route on the map
        route.highlightShortestOrCheapest(originInput.c_str(), destInput.c_str(), true);
        
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

    void run() {
        while(window.isOpen()) 
        {
            handleInput();
            draw();
        }
    }
};

class MainGUI {
private:
    sf::RenderWindow& mainWindow;
    sf::Font font;
    sf::RectangleShape bookFlightButton;
    sf::Text buttonText;
    FlightGraph& flightGraph;

public:
    MainGUI(sf::RenderWindow& window, FlightGraph& graph)
        : mainWindow(window), flightGraph(graph) {
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

    void draw() {
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
    sf::RenderWindow bookingWindow;
    FlightGraph flightGraph;
    FileHandling fileHandler;

public:
    FlightVisualizerApp()
        : mainWindow(sf::VideoMode(1024, 768), "Flight Graph Visualization"),
          bookingWindow(sf::VideoMode(800, 600), "Flight Booking"),
          flightGraph(50, fileHandler),
          fileHandler(200, 50) {
        // Initialize graph
        flightGraph.populateGraph();
    }

    void run() {
        MainGUI mainGUI(mainWindow, flightGraph);
        FlightBookingGUI bookingGUI(bookingWindow, flightGraph);

        while (mainWindow.isOpen()) {
            // Display main graph with Book Flight option
            bool bookFlightPressed = mainGUI.handleEvents();
            mainGUI.draw();

            if (bookFlightPressed) {
                bookingWindow.setVisible(true);

                while (bookingWindow.isOpen()) {
                    bookingGUI.handleInput();
                    bookingGUI.draw();

                    if (!bookingWindow.isOpen()) {
                        // After booking, highlight flights on the main window
                        std::string origin = bookingGUI.getOriginInput();
                        std::string destination = bookingGUI.getDestInput();
                        std::string fromDate = bookingGUI.getFromDateInput();
                        std::string toDate = bookingGUI.getToDateInput();

                        // highlight flights on the main window
                        Route route(flightGraph, mainWindow, flightGraph.getMapTexture());
                        route.listAllFlightsWithinDateRange(origin.c_str(), destination.c_str(), fromDate.c_str(), toDate.c_str());
                        route.displayFlight(origin.c_str(), destination.c_str());
                        route.listShortestAndCheapest(origin.c_str(), destination.c_str());
                        route.highlightShortestOrCheapest(origin.c_str(), destination.c_str(), true);
                        
                    }
                }
            }
        }
    }
};

int main() {

    FlightVisualizerApp app;
    app.run();
    
}