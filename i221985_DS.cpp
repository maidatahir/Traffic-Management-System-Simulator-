#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <queue>
#include <stack>

using namespace std;

// Constants
const int intersectMax = 100, roadMaxxx = 200, infoo = 1e6;  // Substitute for INT_MAX

// Global variables
struct road {
    int to = -1;            // destination index
    int travelTime = infoo;   // time taken to travel on this road
};

struct intersact {
    string name = "";          // name of the intersection
    int green = 0;         // green signal duration
    bool isGreen = false;      // whether the signal is green or not
    road roads[roadMaxxx];     // roads leading from this intersection
    int roadCount = 0;         // total number of connected roads
};

struct vahicle {
    string id = " ";           // vehicle ID
    int IndexSt = -1, indexEnd = -1;          // start and end intersection index
    bool isemergencyy = false; // is this an emergency vehicle?
};

// Declare global arrays properly
intersact intersections[intersectMax];
vahicle vehicles[roadMaxxx];
int vehicleCount = 0, CountInter = 0, TravellingTotal = 0, EventsConjested = 0, SignalAdj = 0, TVehicles = 0;  // total number of vehicles for average calculation
int congestions[intersectMax][intersectMax] = { {0} }; // 2D array for congestion levels between intersections
int roadVehicleCounts[intersectMax][intersectMax] = { {0} };  // 2D array to track vehicle counts on roads

// utility function: get intersection index by name
int getIntersectInd(const string& name) {
    for (int i = 0; i < CountInter; ++i) {
        if (intersections[i].name == name) return i;
    }
    return -1;  // return -1 if intersection not found
}

// load the road network from file
void NetworkOfRoad(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string from, to;
        int travelTime;

        getline(ss, from, ',');
        getline(ss, to, ',');
        ss >> travelTime;

        int indexFrom = getIntersectInd(from);
        if (indexFrom == -1) {
            indexFrom = CountInter++;
            intersections[indexFrom].name = from;
            intersections[indexFrom].roadCount = 0;
        }

        int indexTo = getIntersectInd(to);
        if (indexTo == -1) {
            indexTo = CountInter++;
            intersections[indexTo].name = to;
            intersections[indexTo].roadCount = 0;
        }

        intersections[indexFrom].roads[intersections[indexFrom].roadCount++] = { indexTo, travelTime };
        congestions[indexFrom][indexTo] = 0;
    }
    file.close();
}

// load the vehicle data from file
void DataOfVehicle(const string& fileName, bool isEmergency) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, from, to;

        getline(ss, id, ',');
        getline(ss, from, ',');
        getline(ss, to, ',');

        int fromIdx = getIntersectInd(from);
        int toIdx = getIntersectInd(to);

        if (fromIdx != -1 && toIdx != -1) {
            vehicles[vehicleCount++] = { id, fromIdx, toIdx, isEmergency };
            TVehicles++;
        }
    }
    file.close();
}

// load the traffic signal data from file
void DataOfTrafficSignals(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string intersection;
        int greenTime;

        getline(ss, intersection, ',');
        ss >> greenTime;

        int indec = getIntersectInd(intersection);
        if (indec != -1) {
            intersections[indec].green = greenTime;
        }
    }
    file.close();
}

// load accident data from file
void DataOfAccidents(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string from, to;

        getline(ss, from, ',');
        getline(ss, to, ',');

        int indexFrom = getIntersectInd(from);
        int indexTo = getIntersectInd(to);

        if (indexFrom != -1 && indexTo != -1) {
            congestions[indexFrom][indexTo] = infoo; // block the road
        }
    }
    file.close();
}

// adjust traffic signals dynamically based on congestion using a Priority Queue (Min-Heap)
void SignalsAdjustment() {
    priority_queue<pair<int, int>> congestionQueue; // Priority Queue with (congestion, road index)

    for (int i = 0; i < CountInter; ++i) {
        int totalCongestion = 0;

        for (int j = 0; j < intersections[i].roadCount; ++j) {
            totalCongestion += congestions[i][intersections[i].roads[j].to];
        }

        congestionQueue.push({ totalCongestion, i });
    }

    while (!congestionQueue.empty()) {
        auto top = congestionQueue.top();
        int intersectionIdx = top.second;
        int totalCongestion = top.first;
        congestionQueue.pop();

        intersections[intersectionIdx].green = (totalCongestion > 10) ? 60 : 30;
        intersections[intersectionIdx].isGreen = totalCongestion <= 10;

        if (totalCongestion > 10) {
            SignalAdj++;  // track signal adjustments
        }
    }
}

// monitor and resolve congestion events
void monitoring() {
    for (int i = 0; i < CountInter; ++i) {
        for (int j = 0; j < intersections[i].roadCount; ++j) {
            if (congestions[i][intersections[i].roads[j].to] > 5) {  // Example threshold for congestion
                EventsConjested++;
                cout << "Road is congested from "
                    << intersections[i].name
                    << " to " << intersections[intersections[i].roads[j].to].name << endl;
            }
        }
    }
}

// print the road network (visualization)
void DisplayNetworkOfRoad() {
    cout << "Road Network:\n";
    for (int i = 0; i < CountInter; ++i) {
        cout << intersections[i].name << " connected to: ";

        for (int j = 0; j < intersections[i].roadCount; ++j) {
            cout << intersections[intersections[i].roads[j].to].name << " ";
        }

        cout << endl;
    }
}

// print the vehicle positions and routes
void DisplayPositionsOfVehicle() {
    cout << endl;
    cout << "Position of Vehicles and Routes: " << endl;
    for (int i = 0; i < vehicleCount; ++i) {
        cout << "Vehicle " << vehicles[i].id << " from "
            << intersections[vehicles[i].IndexSt].name << " to "
            << intersections[vehicles[i].indexEnd].name << endl;
    }
}

// print the traffic signal status
void DisplayStatusOfTrafficSignals() {
    cout << endl;
    cout << "Status of Traffic Signals: " << endl;
    for (int i = 0; i < CountInter; ++i) {
        cout << "Intersection " << intersections[i].name
            << ": " << (intersections[i].isGreen ? "Green" : "Red") << endl;
    }
}

// dijkstra's algorithm for regular vehicles
void dijkstra(int startIdx, int endIdx, bool isEmergency) {
    int distances[intersectMax];
    int previous[intersectMax];
    bool visited[intersectMax] = { false };

    for (int i = 0; i < CountInter; ++i) {
        distances[i] = infoo;
        previous[i] = -1;
    }
    distances[startIdx] = 0;

    for (int i = 0; i < CountInter; ++i) {
        int current = -1;

        for (int j = 0; j < CountInter; ++j) {
            if (!visited[j] && (current == -1 || distances[j] < distances[current])) {
                current = j;
            }
        }

        if (distances[current] == infoo) break;

        visited[current] = true;

        for (int j = 0; j < intersections[current].roadCount; ++j) {
            road& road = intersections[current].roads[j];
            int newDist = distances[current] + road.travelTime;
            if (!isEmergency) newDist += congestions[current][road.to];
            if (newDist < distances[road.to]) {
                distances[road.to] = newDist;
                previous[road.to] = current;
            }
        }
    }

    if (distances[endIdx] != infoo) {
        TravellingTotal += distances[endIdx];  // Accumulate the travel time
        cout << "Shortest path distance (Dijkstra): " << distances[endIdx] << " units.\n";
    }
    else {
        cout << "No path found between the selected intersections.\n";
    }
}

// A* algorithm for emergency vehicles
void ASearchAlgo(int startIdx, int endIdx) {
    int distances[intersectMax];
    int previous[intersectMax];
    bool visited[intersectMax] = { false };

    for (int i = 0; i < CountInter; ++i) {
        distances[i] = infoo;
        previous[i] = -1;
    }
    distances[startIdx] = 0;

    while (true) {
        int Index = -1;
        int cost = infoo;

        for (int i = 0; i < CountInter; ++i) {
            if (!visited[i] && distances[i] + abs(i - endIdx) < cost) {
                cost = distances[i] + abs(i - endIdx);
                Index = i;
            }
        }

        if (Index == -1 || Index == endIdx) break;

        visited[Index] = true;

        for (int i = 0; i < intersections[Index].roadCount; ++i) {
            road& road = intersections[Index].roads[i];
            int newDist = distances[Index] + road.travelTime;
            if (newDist < distances[road.to]) {
                distances[road.to] = newDist;
                previous[road.to] = Index;
            }
        }
    }

    if (distances[endIdx] != infoo) {
        TravellingTotal += distances[endIdx];  // Accumulate the travel time
        cout << "Shortest path distance with A Search Algo: " << distances[endIdx] << " units." << endl;
    }
    else {
        cout << "No path found between the selected intersections." << endl;
    }
}

// BFS to detect congestion zones
void congestionDetection(int startIdx) {
    bool visited[intersectMax] = { false };
    queue<int> q;
    visited[startIdx] = true;
    q.push(startIdx);

    cout << "Congestion zone starting from intersection " << intersections[startIdx].name << ": ";

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int i = 0; i < intersections[current].roadCount; ++i) {
            int next = intersections[current].roads[i].to;
            if (!visited[next] && congestions[current][next] > 5) { // Example threshold for congestion
                cout << intersections[next].name << " ";
                visited[next] = true;
                q.push(next);
            }
        }
    }

    cout << endl;
}

// DFS to detect inaccessible paths (due to accidents/closures)
void inaccessiblePathDetect(int startIdx) {
    bool visited[intersectMax] = { false };
    stack<int> s;
    visited[startIdx] = true;
    s.push(startIdx);

    cout << "Inaccessible paths starting from intersection " << intersections[startIdx].name << ": ";

    while (!s.empty()) {
        int current = s.top();
        s.pop();

        for (int i = 0; i < intersections[current].roadCount; ++i) {
            int next = intersections[current].roads[i].to;
            if (!visited[next] && congestions[current][next] != infoo) { // If road is not blocked
                cout << intersections[next].name << " ";
                visited[next] = true;
                s.push(next);
            }
        }
    }

    cout << endl;
}

// Display the vehicle counts on roads
void displayVehicleCounts() {
    cout << "\nVehicle counts on each road segment:\n";
    for (int i = 0; i < CountInter; ++i) {
        for (int j = 0; j < intersections[i].roadCount; ++j) {
            cout << "From " << intersections[i].name << " to "
                << intersections[intersections[i].roads[j].to].name
                << " has " << roadVehicleCounts[i][intersections[i].roads[j].to] << " vehicles.\n";
        }
    }
}

// Simulate the traffic flow
void RunTraffic() {
    cout << "Running Traffic Simulation: " << endl;
    DisplayNetworkOfRoad();
    DisplayPositionsOfVehicle();
    DisplayStatusOfTrafficSignals();

    for (int i = 0; i < vehicleCount; ++i) {
        cout << (vehicles[i].isemergencyy ? "Emergency Vehicle: " : "Vehicle: ") << vehicles[i].id << endl;

        // Update vehicle count as the vehicle moves
        roadVehicleCounts[vehicles[i].IndexSt][vehicles[i].indexEnd]++;

        if (vehicles[i].isemergencyy) {
            ASearchAlgo(vehicles[i].IndexSt, vehicles[i].indexEnd);
        }
        else {
            dijkstra(vehicles[i].IndexSt, vehicles[i].indexEnd, vehicles[i].isemergencyy);
        }
    }

    SignalsAdjustment();  // Adjust traffic signals dynamically
    monitoring();     // Monitor and report congestion

    // Display real-time vehicle counts and congestion
    displayVehicleCounts();

    // Calculate and display the average vehicle travel time
    double AvgTravelling = static_cast<double>(TravellingTotal) / TVehicles;
    cout << "Average travelling time of vehicles is : " << AvgTravelling << " units." << endl;

    cout << "Adjustment of signals is: " << SignalAdj << " adjustments made." << endl;
    cout << "Total time taken for travelling: " << TravellingTotal << " units." << endl;
    cout << "No of conjested events occured: " << EventsConjested << endl;
}

// Main function
int main() {
    // Load data from files
    NetworkOfRoad("road_network.csv");
    DataOfVehicle("vehicles.csv", false);  // Regular vehicles
    DataOfVehicle("emergency_vehicles.csv", true);  // Emergency vehicles
    DataOfTrafficSignals("traffic_signal_timings.csv");
    DataOfAccidents("accidents_or_closures.csv");

    // Run simulation
    RunTraffic();

    // Run BFS and DFS for congestion zones and inaccessible paths
    congestionDetection(0); // Starting from intersection 0
    inaccessiblePathDetect(0); // Starting from intersection 0

    return 0;
}
