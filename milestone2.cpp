#include <direct.h> // For _mkdir
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <cctype>   // For isdigit
#include <cstdlib>  // For exit

using namespace std;

const int MAX_COL = 10;
const int MAX_ROWS = 100; // Maximum number of rows that can be stored

// Helper to strip quotes from a string
string stripQuotes(string input) {
    if (input.length() >= 2 &&
		input[0] == '"' &&
		input[input.length() - 1] == '"') {

		return input.substr(1, input.length() - 2);
    }
    return input;
}

bool isValidFilename(const string& name) {
    if (name.empty()) return false;

    for (char c : name) {
        // Allow letters, numbers, and spaces. Block symbols like / \ : * ?
        if (!isalnum(c) && c != ' ' && c != '_' && c != '-') {
            return false;
        }
    }
    return true;
}

// Function to create a folder (Database)
void createDatabase(string& termName) {
    cout << "Create School Term (Database)\n";
    cout << "-------------------------------------------\n";

    do {
        cout << "Enter term name: ";
        getline(cin, termName);

        if (!isValidFilename(termName)) {
            cout << "Error: Name contains illegal characters. Try again.\n";
            termName = ""; // Force loop
        }
    } while (termName.empty());

    if (_mkdir(termName.c_str()) == 0) {
        cout << "Database \"" << termName << "\" created and loaded.\n\n";
    }
    else {
        cout << "Database \"" << termName
            << "\" created and loaded (or already exists).\n\n";
    }
}

// Function to load attendance data from a CSV file
bool loadFromCSV(string filename, string attendanceData[][MAX_COL],
    int& rowCount, string columnNames[], int& numCols) {
    ifstream inFile(filename.c_str());
    if (!inFile) {
        cout << "Error: Could not open file " << filename << endl;
        return false;
    }

    string line, token;
    rowCount = 0;
    numCols = 0;

    cout << "Reading attendance data from file...\n";

    // Read Header
    if (getline(inFile, line)) {
        stringstream ss(line);
        while (getline(ss, token, ',')) {
            if (!token.empty() && token[token.length() - 1] == '\r')
                token.erase(token.length() - 1);
            columnNames[numCols++] = stripQuotes(token);
            if (numCols >= MAX_COL)
                break;
        }
    }

    // Read Data
    while (getline(inFile, line) && rowCount < MAX_ROWS) {
        if (line.empty())
            continue;
        stringstream ss(line);
        int colIndex = 0;
        while (getline(ss, token, ',')) {
            if (!token.empty() && token[token.length() - 1] == '\r')
                token.erase(token.length() - 1);
            if (colIndex < MAX_COL) {
                attendanceData[rowCount][colIndex++] = stripQuotes(token);
            }
        }
        rowCount++;
    }

    if (!inFile.eof()&& inFile.fail()){
        cout << "Warning: File reading interrupted. Data may be incomplete.\n";
    }

    inFile.close();
    cout << "Successfully loaded: " << filename << "\n\n";
    return true;
}


// Setup sheet (kept for structure but unused in this specific test)
void setupSheet(string& sheetName, int& numCols, string columnNames[]) {

    /* ================= HEADER ================= */

    cout << "===========================================\n";
    cout << "   STUDENT ATTENDANCE TRACKER - MILESTONE 1\n";
    cout << "===========================================\n\n";

    /* ================= SHEET NAME ================= */

    bool onlySpaces;

    do {
        onlySpaces = true;

        cout << "Enter attendance sheet name: ";
        getline(cin, sheetName);

        // 1. Your existing check (ensure it's not just spaces)
        for (int i = 0; i < sheetName.length(); i++) {
            if (sheetName[i] != ' ') {
                onlySpaces = false;
                break;
            }
        }

        if (sheetName.empty() || onlySpaces) {
            cout << "Error: Sheet name is empty. Please enter a name." << endl;
        }
        // 2. NEW CHECK: Is the filename valid? (Chapter 10)
        else if (!isValidFilename(sheetName)) {
            cout << "Error: Name contains illegal characters. Use letters and numbers only.\n";
            // Clear the name to force the loop to repeat
            sheetName = "";
        }

    } while (sheetName.empty() || onlySpaces);

    cout << "\nAttendance sheet \"" << sheetName << ".csv"
        << "\" created successfully.\n\n";

    /* ================= COLUMN SETUP ================= */

    do {
        cout << "Define number of columns (max 10): ";
        cin >> numCols;

        if (cin.fail()) {
            cout << "Error: Please enter a number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        else if (numCols < 1 || numCols > 10) {
            cout << "Error: Number must be between 1 and 10.\n";
        }
        else {
            cin.ignore(1000, '\n');
            break;
        }

    } while (true);

    for (int i = 0; i < numCols; i++) {
        string input;
        bool isDuplicate;     // Flag for error detection
        bool onlySpaces;      // Flag for empty input

        do {
            isDuplicate = false;
            onlySpaces = true;

            cout << "Enter column " << i + 1 << " name: ";
            getline(cin, input);

            // 1. Check for empty or spaces only
            for (int j = 0; j < input.length(); j++) {
                if (input[j] != ' ') {
                    onlySpaces = false;
                    break;
                }
            }

            if (input.empty() || onlySpaces) {
                cout << "Error: Column name cannot be empty. Please enter again.\n";
            }
            // 2. NEW CHECK: Linear Search for duplicates (Chapter 8)
            else {
                for (int k = 0; k < i; k++) {
                    if (columnNames[k] == input) {
                        cout << "Error: Column name \"" << input << "\" already exists.\n";
                        isDuplicate = true;
                        break;
                    }
                }
            }

        } while (input.empty() || onlySpaces || isDuplicate); // Repeat if any error

        columnNames[i] = input; // Store valid column name
    }

    /* ================= DISPLAY SHEET STRUCTURE ================= */

    cout << "\nSheet \"" << sheetName << "\" structure:\n";
    cout << "-------------------------------------------\n";

    for (int i = 0; i < numCols; i++) {
        cout << columnNames[i];
        if (i < numCols - 1)
            cout << " | ";
    }

    cout << "\n-------------------------------------------\n\n";
    cout << "Sheet setup completed successfully.\n\n";
}

// Function to insert attendance rows into the sheet (milestone 1)

bool isValidInt(const string& input) {
    if (input.empty())
        return false;

    for (char c : input) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

bool isIntColumn(const string& columnName) {
    if (columnName.length() < 3)
        return false;

    for (int i = 0; i <= columnName.length() - 3; i++) {
        if (columnName[i] == 'I' && columnName[i + 1] == 'N' &&
            columnName[i + 2] == 'T')
            return true;
    }
    return false;
}

void insertAttendanceRows(string attendanceData[][MAX_COL], int& rowCount,
    string columnNames[], int numCols) {

    char continueInsert = 'y';

    while (continueInsert == 'y' || continueInsert == 'Y') {
        if (rowCount >= MAX_ROWS) {
            cout << "Maximum number of rows (" << MAX_ROWS << ") reached.\n";
            break;
        }

        cout << "Insert New Attendance Row\n";

        // Get data for each column with validation
        for (int i = 0; i < numCols; i++) {
            string input;

            do {
                cout << "Enter " << columnNames[i] << ": ";
                getline(cin, input);

                if (input.empty()) {
                    cout << "Error: Input cannot be empty. Please enter again.\n";
                }
                else if (isIntColumn(columnNames[i]) && !isValidInt(input)) {
                    cout << "Error: Invalid INT value. Please enter a number.\n";
                }
                else {
                    break; // valid input
                }

            } while (true);

            attendanceData[rowCount][i] = input; // store input
        }

        rowCount++;
        cout << "Row inserted successfully.\n\n";

        do {
            // Ask if user wants to insert another row
            cout << "Do you want to insert another row? (y/n): ";
            cin >> continueInsert;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (continueInsert != 'y' && continueInsert != 'Y' &&
                continueInsert != 'n' && continueInsert != 'N') {
                cout << "Error: Please enter y or n only.\n";
            }
        } while (continueInsert != 'y' && continueInsert != 'Y' &&
            continueInsert != 'n' && continueInsert != 'N');

        cout << "\n";
    }

    cout << "Total rows inserted: " << rowCount << "\n\n";
}

// Function to view attendance sheet in CSV mode
void displayCSV(string attendanceData[][MAX_COL], int& rowCount,
    string columnNames[], int numCols) {

    cout << "--------------------------------------------------------------\n";
    cout << "              View Attendance Sheet (CSV Mode)\n";
    cout << "--------------------------------------------------------------\n";

    // Print CSV header (Column)
    for (int i = 0; i < numCols; i++) {
        cout << left << setw(20) << columnNames[i];
        if (i < numCols - 1)
            cout << ", ";
    }
    cout << endl;

    // Print CSV rows
    for (int i = 0; i < rowCount; i++) // rowCount now = number of rows insert
    {
        for (int j = 0; j < numCols; j++) {
            cout << left << setw(20) << attendanceData[i][j];
            if (j < numCols - 1)
                cout << ", ";
        }
        cout << endl;
    }

    cout << endl;
}

// Function to save attendance data to a CSV file (modified support path)
void saveToCSV(string path, string attendanceData[][MAX_COL], int rowCount,
    string columnNames[], int numCols) {

    ofstream outFile(path.c_str());

    if (!outFile) {
        cout << "Error: Could not create file " << path << endl;
        return;
    }

    // Write header
    for (int i = 0; i < numCols; i++) {
        outFile << "\"" << columnNames[i] << "\"";
        if (i < numCols - 1)
            outFile << ",";
    }
    outFile << endl;

    // Write data
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < numCols; j++) {
            outFile << "\"" << attendanceData[i][j] << "\"";
            if (j < numCols - 1)
                outFile << ",";
        }
        outFile << endl;
    }

    // CRITICAL CHECK
    if (outFile.fail()){
        cout << "Error: Critical file write failure. Data may be lost.\n";
        exit(EXIT_FAILURE); // Emergency Exit
    }

    outFile.close();
    // cout << "Attendance data saved to " << path << " successfully." << endl;
}

// Function to update attendance rows in the database
void updateAttendanceRow(string attendanceData[][MAX_COL], int rowCount,
    string columnNames[], int numCols) {

    if (rowCount == 0) {
        cout << "No data to update.\n";
        return;
    }

    displayCSV(attendanceData, rowCount, columnNames, numCols);

    int rowChoice;

    while (true) {
        cout << "Enter row number to update (1 to " << rowCount << "): ";
        cin >> rowChoice;

        // Check if user entered text
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Invalid integer. Try again.\n\n";
        }
        // Check if number is out of range
        else if (rowChoice < 1 || rowChoice > rowCount) {
            cout << "Error: Row number " << rowChoice << " does not exist. Try again.\n\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        // Input is good
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }

    int rowIndex = rowChoice - 1;

    cout << "\nUpdating Row " << rowChoice << "\n";

    for (int i = 0; i < numCols; i++) {
        string input;

        do {
            cout << "Enter new " << columnNames[i]
                 << " (current: " << attendanceData[rowIndex][i] << "): ";

            getline(cin, input);

            if (input.empty()) {
                cout << "Cannot be empty.\n";
            }
            else if (isIntColumn(columnNames[i]) && !isValidInt(input)) {
                cout << "Invalid INT value.\n";
            }
            else break;

        } while (true);

        attendanceData[rowIndex][i] = input;
    }

    cout << "\nRow updated successfully.\n\n";
}

// Function to delete an attendance row
void deleteAttendanceRow(string attendanceData[][MAX_COL],
                         int& rowCount,
                         string columnNames[],
                         int numCols)
{
    if (rowCount == 0) {
        cout << "No data to delete.\n\n";
        return;
    }

    // Display data with row numbers
    cout << "--------------------------------------------------------------\n";
    cout << "                 Delete Attendance Row\n";
    cout << "--------------------------------------------------------------\n";

    cout << setw(5) << "No.";
    for (int i = 0; i < numCols; i++) {
        cout << left << setw(20) << columnNames[i];
    }
    cout << endl;

    for (int i = 0; i < rowCount; i++) {
        cout << setw(5) << i + 1;
        for (int j = 0; j < numCols; j++) {
            cout << left << setw(20) << attendanceData[i][j];
        }
        cout << endl;
    }

int rowChoice;

    while (true) {
        cout << "Enter row number to delete (1 to " << rowCount << "): ";
        cin >> rowChoice;

        // Check if user entered text like "Tom"
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Invalid integer. Try again.\n\n";
        }
        // Check if number is out of range
        else if (rowChoice < 1 || rowChoice > rowCount) {
            cout << "Error: Row number " << rowChoice << " does not exist. Try again.\n\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        // Input is good
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }

    int index = rowChoice - 1;

    // Shift rows up
    for (int i = index; i < rowCount - 1; i++) {
        for (int j = 0; j < numCols; j++) {
            attendanceData[i][j] = attendanceData[i + 1][j];
        }
    }

    rowCount--;

    cout << "Row deleted successfully.\n\n";
}

// Main Function
int main() {
    string sheetName;
    int numCols;
    string columnNames[MAX_COL];
    string attendanceData[MAX_ROWS][MAX_COL]; // array to store row data
    int rowCount = 0; // Counter for number of rows inserted
    string termName;
    string csvFilename;
    int choice;

    do {
        // Menu

        cout << "===========================================\n";
        cout << "   STUDENT ATTENDANCE TRACKER - MAIN MENU\n";
        cout << "===========================================\n";
        cout << "1. Create Attendance Sheet (CSV file)\n";
        cout << "2. Create School Term (Database)\n";
        cout << "3. Update Attendance Row\n";
        cout << "4. Delete Attendance Row\n";
        cout << "0. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            choice = -1; // Force "default" case
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard newline
        }
        cout << endl;;

        switch (choice) {
        case 1:
            /* ================= MILESTONE 1 ================= */
            rowCount = 0; // Reset row count for new sheet
            setupSheet(sheetName, numCols, columnNames);
            insertAttendanceRows(attendanceData, rowCount, columnNames, numCols);
            displayCSV(attendanceData, rowCount, columnNames, numCols);
            {
                string filename = sheetName + ".csv";
                saveToCSV(filename, attendanceData, rowCount, columnNames, numCols);
            }
            break;

        case 2:
            /* ================= MILESTONE 2 ================= */
            createDatabase(termName);

            { // Scope block for local variables
                bool fileLoaded = false;

                // Loop until a valid file is actually loaded
                while (!fileLoaded) {
                cout << "Enter CSV filename to load (example: filename.csv, must include .csv): ";
                getline(cin, csvFilename);

                    // 2. Try to open the file
                    if (loadFromCSV(csvFilename, attendanceData, rowCount, columnNames, numCols)) {

                        fileLoaded = true; // SUCCESS: Exit the loop

                        cout << "--------------------------------------------------------------\n";
                        cout << "                  Current Attendance Sheet\n";
                        cout << "--------------------------------------------------------------\n";

                        // Display Header
                        for (int i = 0; i < numCols; i++) {
                            cout << left << setw(20) << columnNames[i];
                            if (i < numCols - 1) cout << ", ";
                        }
                        cout << endl;

                        // Display Rows
                        for (int i = 0; i < rowCount; i++) {
                            for (int j = 0; j < numCols; j++) {
                                cout << left << setw(20) << attendanceData[i][j];
                                if (j < numCols - 1) cout << ", ";
                            }
                            cout << endl;
                        }
                        cout << endl;

                        // Save to DB
                        string savePath = termName + "/" + csvFilename;
                        saveToCSV(savePath, attendanceData, rowCount, columnNames, numCols);
                        cout << "Data saved to database folder: " << savePath << endl;

                    } else {
                        // FAILURE: File didn't open. Loop repeats.
                        cout << "File could not be opened. Please try again.\n\n";
                    }
                }
            }
            break;
       // Update attendance row
        case 3:
            {
                bool fileLoaded = false;
                string dbFolder;
                string csvFile;

                // Loop until we successfully open a valid file
                while (!fileLoaded) {

                    // 1. Get Database Folder Name
                    do {
                        cout << "Enter database folder name (if not created yet, go to option 2): ";
                        getline(cin, dbFolder);

                        if (!isValidFilename(dbFolder)) {
                            cout << "Error: Illegal characters. Try again.\n";
                            dbFolder = "";
                        }
                    } while (dbFolder.empty());

                    // 2. Get CSV Filename
                    cout << "Enter CSV filename to update (example: filename.csv, must include .csv): ";
                    getline(cin, csvFile);

                    string fullPath = dbFolder + "/" + csvFile;

                    // 3. Try to Load
                    if (loadFromCSV(fullPath, attendanceData, rowCount, columnNames, numCols)) {
                        fileLoaded = true; // SUCCESS: Exit the loop

                        // Perform the Update
                        updateAttendanceRow(attendanceData, rowCount, columnNames, numCols);
                        saveToCSV(fullPath, attendanceData, rowCount, columnNames, numCols);
                        cout << "Updated data saved to: " << fullPath << endl;
                        displayCSV(attendanceData, rowCount, columnNames, numCols);

                    } else {
                        // FAILURE: Could not open file
                        cout << "File not found in folder \"" << dbFolder << "\". Please try again.\n";
                        cout << "Tip: Make sure you created the database in Option 2 first.\n\n";
                    }
                }
            }
            break;
        case 4:
            {
                bool fileLoaded = false;
                string dbFolder;
                string csvFile;

                // Loop until we successfully open a valid file
                while (!fileLoaded) {

                    // 1. Get Database Folder Name
                    do {
                        cout << "Enter database folder name (if not created yet, go to option 2): ";
                        getline(cin, dbFolder);

                        if (!isValidFilename(dbFolder)) {
                            cout << "Error: Illegal characters. Try again.\n";
                            dbFolder = "";
                        }
                    } while (dbFolder.empty());

                    // 2. Get CSV Filename
                    cout << "Enter CSV filename to delete (example: filename.csv, must include .csv): ";
                    getline(cin, csvFile);

                    string fullPath = dbFolder + "/" + csvFile;

                    // 3. Try to Load
                    if (loadFromCSV(fullPath, attendanceData, rowCount, columnNames, numCols)) {
                        fileLoaded = true; // SUCCESS: Exit the loop

                        // Perform the Update
                        deleteAttendanceRow(attendanceData, rowCount, columnNames, numCols);
                        saveToCSV(fullPath, attendanceData, rowCount, columnNames, numCols);
                        cout << "Updated data saved to: " << fullPath << endl;
                        displayCSV(attendanceData, rowCount, columnNames, numCols);

                    } else {
                        // FAILURE: Could not open file
                        cout << "File not found in folder \"" << dbFolder << "\". Please try again.\n";
                        cout << "Tip: Make sure you created the database in Option 2 first.\n\n";
                    }
                }
            }
            break;
        case 0:
            cout << "Exiting program.\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }

        cout << "\n";
    } while (choice != 0);

    return 0;
}
