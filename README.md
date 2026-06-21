# ⚡ Smart EV Charging Network Simulator

> A C++ simulation of a real-world Smart Electric Vehicle Charging Network — featuring priority-based scheduling, intelligent routing, fault tolerance, dynamic pricing, and energy load balancing.

---

## 📌 Problem Statement

As EV adoption grows rapidly, charging infrastructure faces serious operational challenges:

- Long waiting times at charging stations
- Uneven distribution of vehicles across stations
- Difficulty prioritizing emergency vehicles
- Power constraints during peak demand
- Service disruptions caused by station failures

This project simulates the **intelligence layer** of a real EV charging network — the scheduling, routing, pricing, and fault management logic that makes it work.

---

## 🚀 Features

### ⚡ Priority-Based Vehicle Scheduling
Vehicles are categorized by urgency and served accordingly:

| Vehicle Type | Weight |
|-------------|--------|
| Emergency (Ambulances, Fire Trucks, Police) | 50 |
| Commercial (Delivery trucks, Ola/Uber cabs) | 30 |
| Private (Personal cars) | 20 |

**Priority Score Formula:**
```
priorityScore = typeWeight + (100 - batteryLevel) + criticalBoost
criticalBoost = +20 if batteryLevel < 20%, else 0
```

Uses STL `priority_queue` — highest priority vehicle always served first.

---

### 🗺️ Smart Vehicle Routing
When a vehicle enters the network, the system evaluates all online stations and routes it to the one with the **lowest waiting time** — a greedy algorithm with O(n) time complexity.

---

### 🔧 Fault Tolerance & Automatic Rerouting
If a station goes offline:
- All charging sessions are interrupted
- Vehicles are evacuated from both slots and waiting queue
- Each vehicle is automatically rerouted to the next best station
- Offline stations are excluded from all future routing decisions

---

### 💰 Dynamic Surge Pricing
Rates adjust automatically based on real-time demand — similar to Uber surge pricing:

| Station Condition | Charging Rate |
|------------------|--------------|
| Slots available | Rs. 10 / min |
| All slots occupied | Rs. 15 / min |
| Queue present | Rs. 20 / min |

Higher prices during peak demand naturally incentivizes vehicles to choose less busy stations — balancing network load automatically.

---

### ⚡ Energy Load Balancing
Each station has a fixed power capacity shared equally among active vehicles:
```
powerPerSlot = maxCapacity / activeVehicles
chargingTime = baseTime / (powerPerSlot / maxCapacity)
```
More vehicles = less power each = longer charging time — mirrors real-world electrical constraints.

---

### 📝 Event Logging
All major events are recorded in `ev_simulation.log` with simulation timestamps:
- Vehicle assignments and completions
- Station failures and recoveries
- Vehicle rerouting events
- Pricing changes

---

### 📊 Analytics Report
Network-wide performance statistics including total vehicles served, revenue per station, average wait time, and overall network summary.

---
<img width="506" height="148" alt="Image" src="https://github.com/user-attachments/assets/507a66a4-5f2a-447b-8bba-b85796c5f58c" />
<img width="494" height="166" alt="Image" src="https://github.com/user-attachments/assets/83b8d871-b85f-4c21-b68c-c4777f976cb1" />

## 🏗️ System Architecture

```
Vehicle → ChargingSlot → ChargingStation → NetworkManager → main.cpp
                               ↑                   ↑
                            Logger ────────────────┘
```

| Component | Responsibility |
|-----------|---------------|
| `Vehicle` | Vehicle info, priority calculation, charging requirements |
| `ChargingSlot` | Individual slot management, time simulation, power tracking |
| `ChargingStation` | Queue management, pricing, load balancing |
| `NetworkManager` | Smart routing, fault handling, analytics |
| `Logger` | Event logging with timestamps |
| `main.cpp` | Program entry point and interactive CLI |

---

## 🛠️ Tech Stack

| | |
|--|--|
| **Language** | C++ (C++17) |
| **Data Structures** | STL `priority_queue`, `vector` |
| **Algorithm** | Greedy Algorithm (Smart Routing) |
| **File I/O** | `ofstream` (Event Logging) |
| **Paradigm** | Object-Oriented Programming |
| **Build Tool** | G++ Compiler |

---

## 🧠 CS Concepts Demonstrated

- Object-Oriented Programming — encapsulation, operator overloading
- STL Containers — `priority_queue`, `vector`
- Greedy Algorithms — O(n) smart routing
- File I/O — event logging with `ofstream`
- Resource Management — energy load balancing
- Fault-Tolerant Design — automatic failure detection and rerouting
- Modular Software Architecture — single responsibility per class

---

## 📦 How to Compile and Run

### Requirements
- C++17 compatible compiler (g++)

### Compile
```bash
g++ -std=c++17 -o ev_simulator main.cpp Vehicle.cpp ChargingSlot.cpp ChargingStation.cpp NetworkManager.cpp Logger.cpp
```

### Run
```bash
# Windows
.\ev_simulator.exe

# Linux / macOS
./ev_simulator
```

---

## 🖥️ Menu Options

| Option | Action |
|--------|--------|
| 1 | Show Network Status |
| 2 | Smart Route a New Vehicle |
| 3 | Simulate Time (Tick) |
| 4 | Simulate Station Failure |
| 5 | Restore Station |
| 6 | View Analytics Report |
| 7 | Exit |

---

## ⚠️ Known Limitations & Future Improvements

- **Distance-based routing** not implemented — future versions will incorporate both waiting time and physical distance using a weighted formula
- **Billing is estimated upfront** — a production system would calculate charges based on actual energy consumed per session
- **Single-threaded** — real-world networks would process multiple stations concurrently using multithreading
- **No data persistence** — integrating a database would allow permanent storage of vehicles, stations, and sessions

---

## 👩‍💻 Author

**Vaishnavi Reddy**
- GitHub: [github.com/VaishnaviReddy1120](https://github.com/VaishnaviReddy1120)
