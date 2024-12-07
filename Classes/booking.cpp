#include "booking.h"
#include "layover.h"       // Ensure Layover class is included
#include "HotelsList.h"    // Ensure HotelsList class is included
#include "HotelBooking.h"  // Ensure HotelBooking class is included
#include "FileHandling.h"  // For fileHandler if needed

void FlightBook::bookFlightOption(LinkedList &directFlights, RouteList &indirectRoutes) {
    // Display direct flights
    cout << "\nAvailable Direct Flights:\n";
    directFlights.Display();

    // Count how many direct flights
    int directCount = directFlights.size();

    // Display indirect routes
    cout << "\nAvailable Indirect Routes (Multi-leg Journeys):\n";
    indirectRoutes.Display(); 

    int totalOptions = directCount + indirectRoutes.countRoutes();

    if (totalOptions == 0) {
        cout << "No flights/routes available to book.\n";
        return;
    }

    cout << "\nEnter the index of the flight/route you want to book: ";
    int choice;
    cin >> choice;

    if (choice < 0 || choice >= totalOptions) {
        cout << "Invalid choice.\n";
        return;
    }

    if (choice < directCount) {
        // It's a direct flight
        LinkedList::FlightNode* selectedFlightNode = directFlights.getNodeAt(choice);
        if (!selectedFlightNode) {
            cout << "Invalid flight index.\n";
            return;
        }

        Flight &selectedFlight = selectedFlightNode->flight;
        // Book this single flight
        confirmBooking(selectedFlight); 
    } else {
        // It's an indirect route (multi-leg journey)
        int routeIndex = choice - directCount;
        RouteList::RouteNode* routeNode = indirectRoutes.getRouteByIndex(routeIndex);
        if (!routeNode) {
            cout << "Invalid route index.\n";
            return;
        }

        // Book all flights in this multi-leg route
        confirmBooking(routeNode->route.legs); 
    }
}

void FlightBook::confirmBooking(Flight &flight) {
    cout << "\nBooking Details:\n";
    cout << "From: " << flight.origin << "\n";
    cout << "To: " << flight.destination << "\n";
    cout << "Date: " << flight.date << "\n";
    cout << "Price: $" << flight.price << "\n";
    cout << "Airline: " << flight.airline << "\n";
    cout << "Flight booked successfully!\n";
    // No layovers for a direct flight, so no hotel booking needed here.
}

void FlightBook::confirmBooking(LinkedList &legs) {
    cout << "\nBooking Multi-Leg Route Details:\n";
    int totalPrice = 0;
    int totalDistance = 0;

    LinkedList::FlightNode* curr = legs.getHead();
    while (curr) {
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
    cout << "Multi-leg route booked successfully!\n";

    // Now handle layovers:
    // We need to check time difference between arrival of one flight and departure of the next.
    if (legs.size() > 1) {
        cout << "\n\033[1;36mLayover Information:\033[0m\n";
        Layover layoverCalculator; // For calculating layover times
        LinkedList::FlightNode* leg = legs.getHead();

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