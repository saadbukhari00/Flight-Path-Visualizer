# SkyNav AI: Flight Path Visualizer

## Overview
SkyNav AI is an advanced flight navigation tracker that visualizes and optimizes flight routes across multiple destinations. The system processes complex flight data, including origin and destination cities, dates, flight durations, airlines, and layover times, providing users with the best possible travel options.

### Core Features:
1. **Flight Data Representation**: Visualizes flights between cities using graphs.
2. **Flight Booking**: Allows booking flights with layover feasibility.
3. **Shortest and Cheapest Route Finder**: Implements Dijkstra’s and A* algorithms.
4. **Custom Flight Paths and Preferences**: Filters flights based on user preferences.
5. **Layover Management**: Efficiently manages layover durations using queues.
6. **Advanced Route Generation**: Handles multi-leg journeys with linked lists.
7. **Graphical Subgraph Generation**: Displays focused visualizations based on queries.

---

## File Structure
The repository contains the following files:

### Source Code:
- **Main File**: `main.cpp`
- **Supporting Classes**:
  - `FileHandling.cpp`, `FileHandling.h`
  - `FlightGraph.cpp`, `FlightGraph.h`
  - `HotelBooking.cpp`, `HotelBooking.h`
  - `HotelsList.cpp`, `HotelsList.h`
  - `RouteList.cpp`, `RouteList.h`
  - `booking.cpp`, `booking.h`
  - `heap.cpp`, `heap.h`
  - `layover.cpp`, `layover.h`
  - `list.cpp`, `list.h`
  - `mainGUI.cpp`, `mainGUI.h`
  - `menu.cpp`, `menu.h`
  - `queue.cpp`, `queue.h`

### Assets:
- **Input Files**: `Flights.txt`, `HotelCharges_perday.txt`
- **Graphical Data**: Stored in the `Assets` folder.

---

## Prerequisites

Ensure the following dependencies are installed on your Linux system:
1. **g++ Compiler**
2. **SFML (Simple and Fast Multimedia Library)**

### Installing SFML
To install SFML, run the following command:
```bash
sudo apt-get install libsfml-dev
```

---

## How to Run

### Steps:
1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd <repository_name>
   ```

2. Compile the project:
   ```bash
   g++ main.cpp Classes/*.cpp -o SkyNavAI -lsfml-graphics -lsfml-window -lsfml-system
   ```
   Note: Replace `Classes/*.cpp` with the path to the folder containing all supporting `.cpp` files if the folder structure is modified.

3. Run the executable:
   ```bash
   ./SkyNavAI
   ```

4. Ensure `Assets` folder is in the same directory as the executable.

---

## Features and Functionality
### Graph Visualization:
- Nodes represent cities.
- Edges represent flight routes.
- Tooltips display flight details like airline, ticket price, and times.

### Booking System:
- Highlighted routes for direct and connecting flights.
- Layover feasibility calculated dynamically.

### Route Optimization:
- Shortest routes determined using Dijkstra’s algorithm.
- Cheapest routes determined using a custom priority queue implementation.

### Preferences:
- Set preferred airlines and transit cities.
- Filtered subgraphs visualized for user queries.

### Layover Management:
- Queues ensure smooth layovers with minimum waiting time.

---

## Future Improvements
- Implement support for real-time flight data.
- Enhance graphical interface with 3D visualizations.

---

## Contact
For queries or suggestions, please reach out to:
- **Saad Bukhari**: [GitHub Profile](https://github.com/saadbukhari00)
- **Huzaifa Malik**: [GitHub Profile](https://github.com/huzzaaaaifa)
- **Abdul Rafay**: [GitHub Profile](https://github.com/abdulrafay1-4)

---

Thank you for using SkyNav AI!

