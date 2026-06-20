#include <iostream>
#include <string>
#include "Vehicle.h"
#include "ChargingStation.h"
#include "NetworkManager.h"
#include "Logger.h"

using namespace std;

void printMenu() {
    cout << "\n========== SMART EV NETWORK SIMULATOR ==========" << endl;
    cout << "  1.  Show Network Status" << endl;
    cout << "  2.  Smart Route a New Vehicle" << endl;
    cout << "  3.  Simulate Time (Tick)" << endl;
    cout << "  4.  Simulate Station Failure" << endl;
    cout << "  5.  Restore Station" << endl;
    cout << "  6.  View Analytics Report" << endl;
    cout << "  7.  Exit" << endl;
    cout << "=================================================" << endl;
    cout << "Enter choice: ";
}

int main() {
    cout << "========================================" << endl;
    cout << "   Smart EV Charging Network Simulator  " << endl;
    cout << "   Version 2.0 - Upgraded Edition       " << endl;
    cout << "========================================" << endl;

    // Initialize logger
    Logger logger("ev_simulation.log");

    // Setup stations (id, slots, maxPower kW, logger)
    ChargingStation s1(1, 2, 100.0, &logger);
    ChargingStation s2(2, 2, 80.0,  &logger);
    ChargingStation s3(3, 3, 120.0, &logger);

    // Initial vehicles
    Vehicle v1(101, "Private",     20);
    Vehicle v2(102, "Commercial",  40);
    Vehicle v3(103, "Emergency",   60);
    Vehicle v4(104, "Private",     10); // critical battery!
    Vehicle v5(105, "Emergency",   25);
    Vehicle v6(106, "Commercial",  55);
    Vehicle v7(107, "Private",     80);

    cout << "\n[Setup] Assigning initial vehicles..." << endl;
    s1.assignVehicle(v1);
    s1.assignVehicle(v2);
    s1.assignVehicle(v3); // goes to priority queue

    s2.assignVehicle(v4);
    s2.assignVehicle(v5);

    s3.assignVehicle(v6);
    s3.assignVehicle(v7);

    // Build network
    NetworkManager network(&logger);
    network.addStation(s1);
    network.addStation(s2);
    network.addStation(s3);

    network.displayNetwork();

    // Interactive CLI
    int choice;
    do {
        printMenu();
        cin >> choice;

        if (choice == 1) {
            network.displayNetwork();

        } else if (choice == 2) {
            int id, battery;
            string type;
            cout << "Enter Vehicle ID: ";
            cin >> id;
            cout << "Enter Type (Private / Commercial / Emergency): ";
            cin >> type;
            cout << "Enter Battery Level (0-100): ";
            cin >> battery;
            Vehicle newV(id, type, battery);
            newV.displayVehicle();
            network.smartRoute(newV);

        } else if (choice == 3) {
            int mins;
            cout << "Advance by how many minutes? ";
            cin >> mins;
            network.tickAll(mins);
            network.displayNetwork();

        } else if (choice == 4) {
            int sid;
            cout << "Enter Station ID to simulate failure: ";
            cin >> sid;
            network.simulateStationFailure(sid);
            network.displayNetwork();

        } else if (choice == 5) {
            int sid;
            cout << "Enter Station ID to restore: ";
            cin >> sid;
            network.restoreStation(sid);
            cout << "  Station " << sid << " restored successfully!" << endl;

        } else if (choice == 6) {
            network.displayNetworkAnalytics();

        } else if (choice == 7) {
            cout << "\nExiting simulator." << endl;
            cout << "Log saved to: ev_simulation.log" << endl;
            network.displayNetworkAnalytics();

        } else {
            cout << "Invalid choice. Try again." << endl;
        }

    } while (choice != 7);

    return 0;
}
