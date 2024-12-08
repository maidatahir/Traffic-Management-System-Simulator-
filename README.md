Traffic Simulation System
Project Overview
The Traffic Simulation System is designed to simulate and manage the flow of traffic in a city. The system models the road network, vehicles, traffic signals, and congestion dynamically. It supports emergency vehicle routing, traffic signal adjustments, and congestion monitoring, while utilizing advanced algorithms for efficient traffic management.

Key Features:
Graph Representation of Road Network: The city's road network is represented as a graph using adjacency lists, where intersections are nodes, and roads are directed edges with associated travel times.

Dijkstra’s Algorithm: Used to calculate the shortest paths for regular vehicles between intersections.

A Algorithm*: Specially implemented for emergency vehicles to find optimal routes considering both travel time and a heuristic (distance to the goal).

Traffic Signal Adjustment: Traffic signals are dynamically adjusted based on real-time congestion levels. A priority queue (min-heap) is used to determine which intersections require the most attention.

Congestion Monitoring: Real-time congestion levels are tracked using a 2D array to store vehicle counts on each road segment. High congestion roads are monitored, and vehicles are rerouted using BFS/DFS.

BFS/DFS for Congestion and Inaccessibility Detection:

BFS (Breadth-First Search) is used to detect congestion zones based on vehicle count thresholds.
DFS (Depth-First Search) is used to detect and report inaccessible paths due to accidents or road closures.
Technologies Used:
C++ Programming Language
Standard C++ Libraries:
iostream for console input and output
fstream for file handling (loading data from CSV files)
sstream for string parsing
queue and stack for BFS/DFS implementations
unordered_map for vehicle counts tracking (hash table functionality)
Data Files Required:
The simulation requires the following CSV files to be provided:

road_network.csv: Defines the city’s road network with intersections and roads.

Example format: from_intersection, to_intersection, travel_time
vehicles.csv: Defines the regular vehicles and their routes (from intersection to destination).

Example format: vehicle_id, from_intersection, to_intersection
emergency_vehicles.csv: Defines the emergency vehicles and their routes.

Example format: vehicle_id, from_intersection, to_intersection
traffic_signal_timings.csv: Defines the green light timings for each intersection.

Example format: intersection, green_time
accidents_or_closures.csv: Defines roads that are closed due to accidents or other reasons.

Example format: from_intersection, to_intersection
How the System Works:
Initialization:

The system loads the road network, vehicles, traffic signal timings, accidents, and closures from the corresponding CSV files.
Simulating Traffic:

Vehicles move from their starting intersections to the destination intersections.
Dijkstra’s algorithm is used for regular vehicles to calculate the shortest path.
The A* algorithm is used for emergency vehicles to calculate the optimal path.
Congestion Detection:

The vehicle counts on each road segment are updated dynamically as vehicles move.
If congestion exceeds a certain threshold, traffic signals are adjusted, and congestion events are reported.
Traffic Signal Adjustments:

A priority queue (min-heap) is used to adjust traffic signals at intersections based on real-time congestion. Intersections with higher congestion receive a longer green signal.
Congestion and Inaccessibility:

BFS is used to detect congestion zones and reroute vehicles accordingly.
DFS is used to detect inaccessible paths due to accidents or road closures.
How to Run the Simulation:
Step 1: Prepare the Data Files
Ensure that the following CSV files are present in the same directory as the executable or provide the correct file paths in the program:

road_network.csv
vehicles.csv
emergency_vehicles.csv
traffic_signal_timings.csv
accidents_or_closures.csv
Step 2: Compile the Program
Compile the program using a C++ compiler (e.g., g++):

bash
Copy code
g++ -o traffic_simulation traffic_simulation.cpp
Step 3: Run the Simulation
Run the compiled program with the following command:

bash
Copy code
./traffic_simulation
Step 4: View the Results
The program will output the following information:

Road Network: The intersections and their connected roads.
Vehicle Positions: The positions and routes of vehicles.
Traffic Signal Status: The current status (Green or Red) of traffic signals.
Congestion Detection: The congested roads and rerouted vehicles.
Traffic Signal Adjustments: The number of times traffic signals were adjusted based on congestion.
The program will also display the average vehicle travel time, total travel time, and number of congestion events that occurred during the simulation.

Functions Overview:
1. NetworkOfRoad
Loads the road network from the CSV file.
2. DataOfVehicle
Loads the vehicle data from the CSV file and initializes the vehicles.
3. DataOfTrafficSignals
Loads traffic signal timings from the CSV file.
4. DataOfAccidents
Loads accident or road closure data from the CSV file.
5. SignalsAdjustment
Adjusts traffic signals based on congestion levels using a priority queue (min-heap).
6. Monitoring
Monitors the congestion levels and reports congested roads.
7. DisplayNetworkOfRoad
Displays the road network and connected intersections.
8. DisplayPositionsOfVehicle
Displays the positions and routes of all vehicles.
9. DisplayStatusOfTrafficSignals
Displays the current status (Green or Red) of all traffic signals.
10. Dijkstra
Implements Dijkstra’s algorithm to find the shortest path for regular vehicles.
11. ASearchAlgo
Implements the A* algorithm for emergency vehicles to find the shortest path.
12. CongestionDetection
Uses BFS to detect congestion zones and find congested roads.
13. InaccessiblePathDetect
Uses DFS to detect inaccessible paths due to accidents or closures.
14. RunTraffic
Simulates the traffic flow and manages vehicle movement, signal adjustments, and congestion detection.

Sample Output:

Running Traffic Simulation:
Road Network:
Intersection1 connected to: Intersection2
Intersection2 connected to:
A connected to: B C
B connected to: C D
C connected to: D
D connected to: E
E connected to:

Position of Vehicles and Routes:
Vehicle V1 from A to E
Vehicle V2 from B to D
Vehicle V3 from A to C

Status of Traffic Signals:
Intersection A: Green
Intersection B: Red
Intersection C: Red
Intersection D: Red
Intersection E: Green

Vehicle V1 from A to E
Shortest path distance (Dijkstra): 10 units.

Vehicle V2 from B to D
Shortest path distance (Dijkstra): 9 units.

Emergency Vehicle EV1 from A to D
Shortest path distance with A* Algo: 12 units.

Congestion Detection:
Congestion zone starting from intersection A: B C
Inaccessible paths starting from intersection A: E

Adjustment of signals is: 2 adjustments made.
Average travelling time of vehicles is : 10 units.
Total time taken for travelling: 30 units.
No of conjested events occured: 2
Future Enhancements:
Graphical Visualization: Display the road network, vehicles, and traffic signals using a graphical interface.
More Sophisticated Traffic Models: Implement more complex traffic flow models considering more factors like pedestrian traffic, vehicle types, etc.
Real-Time Data Integration: Integrate the simulation with real-time data sources for live traffic monitoring and routing.
License:
This project is licensed under the MIT License.
