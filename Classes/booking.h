#include "main.h"
#include "list.h"
#include "RouteList.h"

class FlightBook{
public:
    void bookFlightOption(LinkedList &directFlights, RouteList &indirectRoutes);
    void confirmBooking(Flight &flight);
    void confirmBooking(LinkedList &legs);
};