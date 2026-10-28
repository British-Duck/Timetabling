#include "TimetableSystem.h"
#include <iostream>
#include <stdexcept>
using namespace std;

/**
 * This file sets up the system, loads initial data, runs the main loop,
 * and handles cleanup. It's the central coordinator for the application.
 */
int main() {
    try {
        // -------------- SYSTEM INITIALIZATION------------------//
        cout << "Initializing NTU Timetable System...\n";
        TimetableSystem system;

        // --------------DATA LOADING----------------------//
        // Attempt to load any existing data from storage
        cout << "Loading system data...\n";
        system.loadData();

        // -----------DEMO DATA SETUP (for first-run experience)----//
        // If no data exists, create some sample data
        if (system.getAllSessions().empty()) {
            cout << "No existing data found. Creating sample data...\n";
            system.createSampleData();
        }

        // ------------MAIN APPLICATION LOOP------------------//
        // This starts the interactive menu system
        cout << "\nWelcome to NTU Timetable System\n";
        system.run();

        //------------------ DATA PERSISTENCE-------------------//
        // Save all changes before exiting
        cout << "Saving system data...\n";
        system.saveData();

        cout << "Thank you for using NTU Timetable System. Goodbye!\n";
    }
    catch (const exception& e) {
        //--------------- ERROR HANDLING-----------------//
        // Catch any fatal errors that weren't handled elsewhere
        cerr << "\nFATAL ERROR: " << e.what() << endl;
        cerr << "The application will now terminate.\n";
        return 1; // Return non-zero to indicate error
    }

    return 0; // Successful execution
}

/** * Key Components:
 * 1. System Initialization - Creates the main TimetableSystem object
 * 2. Data Loading - Uses loadData() to read from persistent storage
 * 3. Demo Data - Provides first-run experience with sample data
 * 4. Main Loop - Triggers the interactive menu system via run()
 * 5. Data Saving - Persists changes to storage
 * 6. Error Handling - Catches any unhandled exceptions
 */
