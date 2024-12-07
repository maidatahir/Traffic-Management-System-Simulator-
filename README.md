# Smart Traffic Management System Simulator

This repository contains a simulator for a **Smart Traffic Management System**. The system is designed to model real-world urban traffic flow, manage congestion, route emergency vehicles efficiently, and simulate traffic signal adjustments based on congestion levels. It uses graph-based algorithms to find optimal paths for vehicles, dynamically adjusts traffic signal timings, and handles road closures or accidents.

### Features:
- **Dynamic Traffic Routing**: Vehicles are routed using Dijkstra's algorithm, adjusting to congestion levels in real-time.
- **Emergency Vehicle Routing**: Emergency vehicles are given priority, and their paths are cleared dynamically.
- **Traffic Signal Adjustment**: Traffic signals at intersections are adjusted based on road congestion to optimize flow.
- **Congestion Monitoring**: Roads are monitored for congestion levels, and vehicles are rerouted if necessary.
- **Real-Time Dashboard**: A live dashboard that displays traffic conditions, congestion levels, and vehicle movements.
- **Simulation Logging**: Logs all actions, including vehicle movements, congestion events, and signal changes, for analysis.
