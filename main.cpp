/*
COMMAND FOR COMPILATION MAC NOTE - (change with location of where you installed SFML):
g++ -I/opt/homebrew/opt/sfml/include -L/opt/homebrew/opt/sfml/lib main.cpp Classes/.cpp -o FlightPathVisualizer -lsfml-graphics -lsfml-window -lsfml-system
*/

/* added by Huzaifa for compilation on my system
 g++ -o FlightPathVisualizer main.cpp Classes/FileHandling.cpp -lsfml-graphics -lsfml-window -lsfml-system
*/

#include "Classes/main.h"
#include "Classes/list.h"
#include "Classes/heap.h"
#include "Classes/stack.h"
#include "Classes/queue.h"
#include "Classes/route.h"
FileHandling files(105, 10); // (number of flights, number of hotel cities)

class FlightBookingGUI 
{
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text resultText;

    
    sf::RectangleShape originBox, destBox, dateBox, searchButton;
    sf::Text originText, destText, dateText, searchText;
    string originInput, destInput, dateInput;
    FlightGraph& flightGraph;
    int selectedOriginIndex, selectedDestIndex;

    
    bool isOriginFocused;
    bool isDestFocused;
    bool isDateFocused;

public:
    FlightBookingGUI(sf::RenderWindow& win, FlightGraph& graph) : window(win), flightGraph(graph)
    {
        selectedOriginIndex = -1;
        selectedDestIndex = -1;
        isOriginFocused = true;
        isDestFocused = false;
        isDateFocused = false;
        
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
        dateText.setString("Date (DD/MM/YYYY): ");

        searchButton.setSize(sf::Vector2f(100, 40));
        searchButton.setPosition(100, 250);
        searchButton.setFillColor(sf::Color::Green);
        searchText.setFont(font);
        searchText.setCharacterSize(20);
        searchText.setPosition(120, 260);
        searchText.setFillColor(sf::Color::Black);
        searchText.setString("Search");
    }

    void handleInput() 
    {
        sf::Event event;
        while(window.pollEvent(event)) 
        {
            if(event.type == sf::Event::Closed) 
            {
                window.close();
            }

           if(event.type == sf::Event::TextEntered) 
           {
                if(event.text.unicode < 128) 
                {
                    if(event.text.unicode == 8) 
                    {
                        
                        if(isOriginFocused && !originInput.empty()) 
                        {
                            originInput.pop_back();
                        } 
                        else if(isDestFocused && !destInput.empty()) 
                        {
                            destInput.pop_back();
                        } 
                        else if(isDateFocused && !dateInput.empty()) 
                        {
                            dateInput.pop_back();
                        }
                    } 
                    else if(event.text.unicode == 13) 
                    {
                       
                        if(isOriginFocused)
                        {
                            isOriginFocused = false;
                            isDestFocused = true;
                        } 
                        else if(isDestFocused) 
                        {
                            isDestFocused = false;
                            isDateFocused = true;
                        }
                    } 
                    else 
    {
                        if(isOriginFocused)
                        {
                            originInput += static_cast<char>(event.text.unicode);
                        } 
                        else if(isDestFocused) 
                        {
                            destInput += static_cast<char>(event.text.unicode);
                        } 
                        else if(isDateFocused) 
                        {
                            dateInput += static_cast<char>(event.text.unicode);
                        }
                    }
                }
            }

            
        if(event.type == sf::Event::MouseButtonPressed) 
        {
            
            if(originBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) 
            {
                isOriginFocused = true;
                isDestFocused = false;
                isDateFocused = false;
            }
            
            else if(destBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) 
            {
                isOriginFocused = false;
                isDestFocused = true;
                isDateFocused = false;
            }
            
            else if(dateBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) 
            {
                isOriginFocused = false;
                isDestFocused = false;
                isDateFocused = true;
            }

            
            if(searchButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                handleSearch();
            }
        }
    }
        
    }

    void handleSearch() 
    {
    	 if (originInput.empty() || destInput.empty() || dateInput.empty()) 
    {
        std::cout << "Please fill in all fields.\n";
        return;
    }

    Route route(flightGraph);
    std::ostringstream resultStream;  // For formatting the result

    // Search for the shortest route
    route.displayFlight(originInput.c_str(), destInput.c_str());

    // Update resultText to display the result (you can capture the output in `resultStream`)
    resultText.setString(resultStream.str());

    // Refresh the window to show the updated result
    window.draw(resultText);
            
    }


    void draw() 
    {
        window.clear(sf::Color::White);
        window.draw(originBox);
        window.draw(originText);
        window.draw(destBox);
        window.draw(destText);
        window.draw(dateBox);
        window.draw(dateText);
        window.draw(searchButton);
        window.draw(searchText);

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
        inputDateText.setPosition(300, 210);
        inputDateText.setFillColor(sf::Color::Black);
        window.draw(inputDateText);

    
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


int main() {
    FileHandling fileHandler;
    FlightGraph graph(50, fileHandler, 50);
    
    graph.populateGraph();

    sf::Texture mapTexture;
    if(!mapTexture.loadFromFile("Assets/world_map.png")) 
    {
        std::cerr << "Error loading world_map.png. Ensure the file exists in the 'Assets' folder.\n";
        return -1;
    }

    sf::RenderWindow mapWindow(sf::VideoMode(1024, 768), "Flight Graph Visualization");
    sf::RenderWindow appWindow(sf::VideoMode(800, 600), "Flight Booking GUI");
    FlightBookingGUI app(appWindow, graph);

    while(mapWindow.isOpen() && appWindow.isOpen()) 
    {
        sf::Event event;

        while(mapWindow.pollEvent(event)) 
        {
            if(event.type == sf::Event::Closed)
                mapWindow.close();
        }
        mapWindow.clear();
        graph.displayOnMap(mapWindow,mapTexture); 
        mapWindow.display();

        while(appWindow.pollEvent(event)) 
        {
            if(event.type == sf::Event::Closed)
                appWindow.close();
        }
        appWindow.clear(sf::Color::White);
        app.run();
        appWindow.display();
    }
}
