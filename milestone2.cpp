#include <direct.h> // For _mkdir
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>


using namespace std;

const int MAX_COL = 10;
const int MAX_ROWS = 100; // Maximum number of rows that can be stored

// Helper to strip quotes from a string
string stripQuotes(string input) {
    if (input.length() >= 2 && input.front() == '"' && input.back() == '"') {
        return input.substr(1, input.length() - 2);
    }
    return input;
}

// Function to create a folder (Database)
void createDatabase(string& termName) {
    cout << "Create School Term (Database)\n";
    cout << "-------------------------------------------\n";
    cout << "Enter term name: ";
    getline(cin, termName);

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
    ifstream inFile(filename);
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
            if (!token.empty() && token.back() == '\r')
                token.pop_back();
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
            if (!token.empty() && token.back() == '\r')
                token.pop_back();
            if (colIndex < MAX_COL) {
                attendanceData[rowCount][colIndex++] = stripQuotes(token);
            }
        }
        rowCount++;
    }

    inFile.close();
    cout << "Successfully loaded: " << filename << "\n\n";
    return true;
}

// Setup sheet (kept for structure but unused in this specific test)
void setupSheet(string& sheetName, int& numCols, string columnNames[]) {

    /* ================= HEADER ================= */

    cout << "===========================================\n";
    cout << "   STUDENT ATTENDANCE TRACKER - MILESTONE 2\n";
    cout << "===========================================\n\n";

    /* ================= SHEET NAME ================= */

    bool onlySpaces;

    do {

        onlySpaces = true;

        cout << "Enter attendance sheet name: ";
        getline(cin, sheetName);

        for (int i = 0; i < sheetName.length(); i++) {
            if (sheetName[i] != ' ') {
                onlySpaces = false;
                break;
            }
        }

        if (sheetName.empty() || onlySpaces)
            cout << "Error: Sheet name is empty. Please enter a name." << endl;

    } while (sheetName.empty() || onlySpaces);

    cout << "\nAttendance sheet \"" << sheetName
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
        bool onlySpaces;

        do {
            cout << "Enter column " << i + 1 << " name: ";
            getline(cin, input);

            // check if input is empty or only spaces
            onlySpaces = true;
            for (int j = 0; j < input.length(); j++) {
                if (input[j] != ' ') {
                    onlySpaces = false;
                    break;
                }
            }

            if (input.empty() || onlySpaces)
                cout << "Error: Column name cannot be empty or just spaces. Please "
                "enter again.\n";

        } while (input.empty() || onlySpaces);

        columnNames[i] = input; // store valid column name
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

// Function to insert attendance rows into the sheet

bool isValidInt(const string& input) {
    if (input.empty())
        return false;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] < '0' || input[i] > '9')
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
    ofstream outFile(path);

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

    outFile.close();
    // cout << "Attendance data saved to " << path << " successfully." << endl;
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

    /* ================= CREATE DATABASE (TERM) ================= */
    createDatabase(termName);

    /* ================= LOAD CSV ================= */
    cout << "Enter CSV filename to load (e.g. "
        "Trimester2530_Week1_Attendance.csv): ";
    getline(cin, csvFilename);

    if (loadFromCSV(csvFilename, attendanceData, rowCount, columnNames,
        numCols)) {

        /* ================= DISPLAY LOADED DATA ================= */
        cout << "-------------------------------------------\n";
        cout << "Current Attendance Sheet\n";
        cout << "-------------------------------------------\n";

        // Simple display as per screenshot (Comma separated)
        for (int i = 0; i < numCols; i++) {
            cout << columnNames[i];
            if (i < numCols - 1)
                cout << ", ";
        }
        cout << endl;

        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < numCols; j++) {
                cout << attendanceData[i][j];
                if (j < numCols - 1)
                    cout << ", ";
            }
            cout << endl;
        }
        cout << endl;

        /* ================= SAVE TO DATABASE FOLDER ================= */
        // Save a copy into the term folder
        string savePath = termName + "/" + csvFilename;
        saveToCSV(savePath, attendanceData, rowCount, columnNames, numCols);
        cout << "Data saved to database folder: " << savePath << endl;

    }
    else {
        cout << "Failed to load data. Exiting.\n";
    }

    return 0;
}
