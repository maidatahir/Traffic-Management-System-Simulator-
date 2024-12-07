#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <list>
#include <vector>
#include <queue>
#include <limits>
#include <iomanip>
#include <thread>
#include <chrono>
using namespace std;

class TrafficNetwork {
private:
    unordered_map<string, list<pair<string, int>>> graph; // Adjacency list
    unordered_map<string, int> congestionLevels;         // Road congestion levels
    unordered_map<string, int> signalDurations;          // Signal durations at intersections
    vector<pair<string, string>> vehicles;               // Regular vehicles
    vector<pair<string, string>> emergencyVehicles;      // Emergency vehicles
    int totalTravelTime = 0;                             // Total travel time for metrics
    int congestionEvents = 0;                            // Count of congestion events
    vector<string> logEntries;                           // Stores log entries for reporting

    void logEvent(const string& event) {
        logEntries.push_back(event);
    }

    void saveLogReport() {
        ofstream logFile("simulation_log.txt", ios::out);
        if (logFile.is_open()) {
            for (const string& entry : logEntries) {
                logFile << entry << endl;
            }
            logFile.close();
            cout << "Simulation log report saved to simulation_log.txt\n";
        } else {
            cerr << "Failed to save log report.\n";
        }
    }

    void adjustTrafficSignals() {
        cout << "Adjusting traffic signals based on congestion levels...\n";
        for (const auto& [road, level] : congestionLevels) {
            string intersection = road.substr(0, road.find("->"));
            signalDurations[intersection] = max(10, level * 2); // Example logic: increase duration with congestion
        }

        for (const auto& [intersection, duration] : signalDurations) {
            cout << "Signal at " << intersection << ": " << duration << " seconds\n";
            logEvent("Signal adjusted at " + intersection + ": " + to_string(duration) + " seconds");
        }
    }

    void displayMonitoringDashboard() {
        cout << "\nReal-Time Monitoring Dashboard\n";
        cout << "---------------------------------\n";
        for (const auto& [node, neighbors] : graph) {
            cout << "Intersection: " << node << " -> Roads: ";
            for (const auto& [neighbor, weight] : neighbors) {
                string road = node + "->" + neighbor;
                string congestion = congestionLevels[road] > 5 ? "High" : "Low";
                cout << neighbor << " (W: " << weight << ", Congestion: " << congestion << ")  ";
            }
            cout << endl;
        }
        cout << "---------------------------------\n";
    }

    vector<string> findPath(const string& start, const string& end) {
        unordered_map<string, int> distances;
        unordered_map<string, string> previous;
        for (const auto& [node, _] : graph) {
            distances[node] = numeric_limits<int>::max();
        }
        distances[start] = 0;

        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
        pq.emplace(0, start);

        while (!pq.empty()) {
            auto [currentDist, currentNode] = pq.top();
            pq.pop();

            if (currentDist > distances[currentNode]) continue;

            for (const auto& [neighbor, weight] : graph[currentNode]) {
                int newDist = currentDist + weight + congestionLevels[currentNode + "->" + neighbor];
                if (newDist < distances[neighbor]) {
                    distances[neighbor] = newDist;
                    previous[neighbor] = currentNode;
                    pq.emplace(newDist, neighbor);
                }
            }
        }

        vector<string> path;
        if (distances[end] != numeric_limits<int>::max()) {
            string temp = end;
            while (temp != start) {
                path.push_back(temp);
                temp = previous[temp];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
        }
        return path;
    }

    void moveVehicle(const vector<string>& path, const string& vehicleType) {
        for (size_t i = 0; i < path.size() - 1; ++i) {
            string road = path[i] + "->" + path[i + 1];
            cout << vehicleType << " on road: " << road
                 << " (Congestion: " << congestionLevels[road] << ")\n";
            logEvent(vehicleType + " traveled on road: " + road);

            if (vehicleType == "Regular vehicle" && congestionLevels[road] > 7) {
                cout << "High congestion detected! Recalculating route...\n";
                logEvent("High congestion detected on road: " + road + ". Route recalculated.");
                break; // Trigger route recalculation
            }
            this_thread::sleep_for(chrono::seconds(1)); // Simulate movement
        }
        cout << vehicleType << " completed journey.\n";
        logEvent(vehicleType + " completed journey.");
    }

public:
    void addIntersection(const string& name) {
        if (graph.find(name) == graph.end()) {
            graph[name] = {};
            signalDurations[name] = 30; // Default signal duration
        }
    }

    void addRoad(const string& from, const string& to, int weight) {
        graph[from].emplace_back(to, weight);
        congestionLevels[from + "->" + to] = rand() % 10; // Simulate random congestion
    }

    void addVehicle(const string& start, const string& end, bool isEmergency = false) {
        if (isEmergency) {
            emergencyVehicles.emplace_back(start, end);
            cout << "Emergency vehicle added from " << start << " to " << end << ".\n";
            logEvent("Emergency vehicle added: " + start + " to " + end);
        } else {
            vehicles.emplace_back(start, end);
            cout << "Vehicle added from " << start << " to " << end << ".\n";
            logEvent("Vehicle added: " + start + " to " + end);
        }
    }

    void runSimulation() {
        displayMonitoringDashboard();
        adjustTrafficSignals();

        for (const auto& [start, end] : vehicles) {
            vector<string> path = findPath(start, end);
            if (!path.empty()) {
                moveVehicle(path, "Regular vehicle");
            } else {
                cout << "No path found for vehicle from " << start << " to " << end << ".\n";
                logEvent("No path found for vehicle from " + start + " to " + end);
            }
        }

        for (const auto& [start, end] : emergencyVehicles) {
            vector<string> path = findPath(start, end);
            if (!path.empty()) {
                moveVehicle(path, "Emergency vehicle");
            } else {
                cout << "No path found for emergency vehicle from " << start << " to " << end << ".\n";
                logEvent("No path found for emergency vehicle from " + start + " to " + end);
            }
        }

        saveLogReport();
    }
};

int main() {
    TrafficNetwork network;

    // Add intersections and roads
    network.addIntersection("A");
    network.addIntersection("B");
    network.addIntersection("C");
    network.addIntersection("D");

    network.addRoad("A", "B", 5);
    network.addRoad("B", "C", 3);
    network.addRoad("C", "D", 2);
    network.addRoad("A", "C", 10);

    // Add vehicles
    network.addVehicle("A", "D");
    network.addVehicle("B", "C", true); // Emergency vehicle

    // Run simulation
    network.runSimulation();

    return 0;
}

