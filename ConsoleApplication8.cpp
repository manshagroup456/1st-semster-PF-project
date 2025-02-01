#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <ctime>

using namespace std;

// Constants
const int MAX_SEATS = 10;

// Train details
struct Train {
    string name;
    string source;
    string destination;
    vector<string> coaches; // List of coaches
    vector<vector<bool>> seatAvailability; // Seat availability for each coach
    vector<int> coachSeats; // Number of seats per coach
    map<string, int> ticketPrices; // Ticket prices for each coach
};

vector<Train> trains = {
    {"Green Express", "Karachi", "Lahore", {"Business", "Economy"}, {{false, false, false, false, false, false, false, false, false, false}, {false, false, false, false, false, false, false, false, false, false}}, {10, 10}, {{"Business", 2000}, {"Economy", 1000}}},
    {"Blue Line", "Islamabad", "Karachi", {"Business", "Economy"}, {{false, false, false, false, false, false, false, false, false, false}, {false, false, false, false, false, false, false, false, false, false}}, {10, 10}, {{"Business", 2200}, {"Economy", 1200}}},
    {"Red Arrow", "Quetta", "Peshawar", {"Business", "Economy"}, {{false, false, false, false, false, false, false, false, false, false}, {false, false, false, false, false, false, false, false, false, false}}, {10, 10}, {{"Business", 2500}, {"Economy", 1500}}}
};

// Passenger details
struct Passenger {
    string name;
    string cnic;
    int age;
    string gender;
    string seatClass;
    int seatNumber;
    string trainName;
    string coachName;
    int ticketPrice;
    time_t bookingTime;
};

int ticketID = 0;  // Global variable for ticket ID
vector<Passenger> passengerDetails;
map<string, bool> trainDelays; // Train delay notifications
vector<string> transactionLogs; // Transaction tracking

// Function to display available trains
void displayTrains() {
    cout << "\nAvailable Trains:\n";
    cout << left << setw(10) << "Train No." << setw(20) << "Train Name" << setw(15) << "Source" << setw(20) << "Destination" << "Classes (Seats)\n";
    cout << "---------------------------------------------------------------\n";
    for (size_t i = 0; i < trains.size(); ++i) {
        cout << left << setw(10) << i + 1
            << setw(20) << trains[i].name
            << setw(15) << trains[i].source
            << setw(20) << trains[i].destination;
        for (size_t j = 0; j < trains[i].coaches.size(); ++j) {
            int availableSeats = count(trains[i].seatAvailability[j].begin(), trains[i].seatAvailability[j].end(), false);
            cout << trains[i].coaches[j] << " (" << availableSeats << " seats)";
            if (j != trains[i].coaches.size() - 1) cout << ", ";
        }
        cout << "\n";
    }
}

// Function to log transactions
void logTransaction(const string& log) {
    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back(); // Remove trailing newline
    transactionLogs.push_back(timestamp + " - " + log);
}

// Function to notify train delays
void notifyTrainDelays() {
    cout << "\nTrain Delay Notifications:\n";
    if (trainDelays.empty()) {
        cout << "No delay notifications at the moment.\n";
        return;
    }
    for (const auto& delay : trainDelays) {
        cout << "Train: " << delay.first << " - " << (delay.second ? "Delayed" : "On Time") << "\n";
    }
}

// Function to update train delays
void updateTrainDelays() {
    string trainName;
    cout << "Enter Train Name to Update Delay Status: ";
    cin.ignore();
    getline(cin, trainName);

    bool delayStatus;
    cout << "Is the train delayed? (1 for Yes, 0 for No): ";
    cin >> delayStatus;

    trainDelays[trainName] = delayStatus;
    logTransaction("Updated delay status for train: " + trainName + " to " + (delayStatus ? "Delayed" : "On Time"));
    cout << "Train delay status updated successfully.\n";
}

// Function to generate revenue report
void generateRevenueReport() {
    double totalRevenue = 0;
    cout << "\nRevenue Report:\n";
    cout << left << setw(20) << "Train Name" << setw(15) << "Coach" << "Revenue\n";
    cout << "------------------------------------------\n";
    for (const auto& train : trains) {
        for (size_t i = 0; i < train.coaches.size(); ++i) {
            int bookedSeats = count(train.seatAvailability[i].begin(), train.seatAvailability[i].end(), true);
            double revenue = bookedSeats * train.ticketPrices.at(train.coaches[i]);
            totalRevenue += revenue;
            cout << left << setw(20) << train.name << setw(15) << train.coaches[i] << revenue << "\n";
        }
    }
    cout << "------------------------------------------\n";
    cout << "Total Revenue: " << totalRevenue << "\n";
}

// Function to view transaction logs
void viewTransactionLogs() {
    cout << "\nTransaction Logs:\n";
    if (transactionLogs.empty()) {
        cout << "No transactions recorded yet.\n";
        return;
    }
    for (const auto& log : transactionLogs) {
        cout << log << "\n";
    }
}

// Updated admin portal menu
void adminPortal() {
    if (!adminLogin()) {
        cout << "Invalid credentials. Access denied.\n";
        return;
    }

    int adminChoice;
    do {
        cout << "\nAdmin Portal\n";
        cout << "1. View All Trains\n";
        cout << "2. View Booked Tickets\n";
        cout << "3. Update Train Source/Destination\n";
        cout << "4. Add New Train\n";
        cout << "5. Update or Add Coaches/Classes\n";
        cout << "6. Update or Add Seats in Coaches/Trains\n";
        cout << "7. Notify Train Delays\n";
        cout << "8. Generate Revenue Report\n";
        cout << "9. View Transaction Logs\n";
        cout << "10. Logout\n";
        cout << "Enter your choice: ";
        cin >> adminChoice;

        switch (adminChoice) {
        case 1:
            displayTrains();
            break;
        case 2:
            viewBookedTickets();
            break;
        case 3:
            updateTrainSourceDestination();
            break;
        case 4:
            addNewTrain();
            break;
        case 5:
            updateAddCoachesClasses();
            break;
        case 6:
            updateAddSeats();
            break;
        case 7:
            updateTrainDelays();
            break;
        case 8:
            generateRevenueReport();
            break;
        case 9:
            viewTransactionLogs();
            break;
        case 10:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (adminChoice != 10);
}

// Main menu
int main() {
    int choice;
    do {
        cout << "\nRailway Reservation System\n";
        cout << "1. View Available Trains\n";
        cout << "2. Book Ticket\n";
        cout << "3. View Booked Tickets\n";
        cout << "4. Cancel Ticket\n";
        cout << "5. Admin Portal\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            displayTrains();
            break;
        case 2:
            bookTicket();
            break;
        case 3:
            viewBookedTickets();
            break;
        case 4:
            cancelTicket();
            break;
        case 5:
            adminPortal();
            break;
        case 6:
            cout << "Exiting the system...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 6);

    return 0;
}
