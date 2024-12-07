#include "booking.h"

void FlightBook::bookFlightOption(LinkedList &directFlights, RouteList &indirectRoutes) {
    // Display direct flights
    cout << "\nAvailable Direct Flights:\n";
    directFlights.Display();

    // Count how many direct flights
    int directCount = directFlights.size();

    // Display indirect routes
    cout << "\nAvailable Indirect Routes (Multi-leg Journeys):\n";
    // We'll write a display function for RouteList that shows each route as a single option
    indirectRoutes.Display(); 
    // assume displaySummaries() prints something like:
    // Route #0: Islamabad -> Dubai -> London (2 legs)
    // Route #1: New York -> Paris -> Berlin -> Tokyo (3 legs)

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

        // The routeNode->route.legs contains all the flights for this multi-leg journey
        // Book them all
        confirmBooking(routeNode->route.legs); 
        // where confirmBooking(LinkedList&) loops over all flights in the route and finalizes them
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
    // Here you could store booking in a stack, queue, or any booking log as required.
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
    // Again, store the booking if required.
}