#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

const int MAX_COL = 10;
const int MAX_ROWS = 100;  // Maximum number of rows that can be stored


// Function to setup the sheet
void setupSheet(string &sheetName, int &numCols, string columnNames[]) {

    /* ================= HEADER ================= */

    cout << "===========================================\n";
    cout << "   STUDENT ATTENDANCE TRACKER - MILESTONE 1\n";
    cout << "===========================================\n\n";

    /* ================= SHEET NAME ================= */

    cout << "Enter attendance sheet name: ";
    getline(cin, sheetName);

    cout << "\nAttendance sheet \"" << sheetName
         << "\" created successfully.\n\n";

    /* ================= COLUMN SETUP ================= */

    cout << "Define number of columns (max 10): ";
    cin >> numCols;
    cin.ignore();

    if (numCols > MAX_COL)
        numCols = MAX_COL;

    for (int i = 0; i < numCols; i++) {
        cout << "Enter column " << i + 1 << " name: ";
        getline(cin, columnNames[i]);
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
void insertAttendanceRows(string attendanceData[][MAX_COL], int& rowCount, string columnNames[], int numCols) {

    char continueInsert = 'y';

    while (continueInsert == 'y' || continueInsert == 'Y') {
        if (rowCount >= MAX_ROWS) {
            cout << "Maximum number of rows (" << MAX_ROWS << ") reached.\n";
            break;
        }

        cout << "Insert New Attendance Row\n";

        // Get data for each column
        for (int i = 0; i < numCols; i++) {
            string input;
            cout << "Enter " << columnNames[i] << ": ";
            getline(cin, input);
            attendanceData[rowCount][i] = input;
        }

        rowCount++;
        cout << "Row inserted successfully.\n\n";

        // Ask if user wants to insert another row
        cout << "Do you want to insert another row? (y/n): ";
        cin >> continueInsert;
        cin.ignore();  // Clear the newline character
        cout << "\n";
    }

    cout << "Total rows inserted: " << rowCount << "\n\n";
}

// Function to view attendance sheet in CSV mode
void displayCSV(string attendanceData[][MAX_COL], int& rowCount, string columnNames[], int numCols)
{

    cout << "--------------------------------------------------------------\n";
    cout << "              View Attendance Sheet (CSV Mode)\n";
    cout << "--------------------------------------------------------------\n";

    // Print CSV header (Column)
    for (int i = 0; i < numCols; i++)
    {
        cout << left << setw(20) << columnNames[i];
        if (i < numCols - 1)
            cout << ", ";
    }
    cout << endl;

    // Print CSV rows
    for (int i = 0; i < rowCount; i++) //rowCount now = number of rows insert
    {
        for (int j = 0; j < numCols; j++)
        {
            cout << left << setw(20) << attendanceData[i][j];
            if (j < numCols - 1)
                cout << ", ";
        }
        cout << endl;
    }

    cout << endl;
}


// Main Function
int main() {

    string sheetName;
    int numCols;
    string columnNames[MAX_COL];
    string attendanceData[MAX_ROWS][MAX_COL];  // array to store row data
    int rowCount = 0;  // Counter for number of rows inserted

    /* ================= SETUP SHEET ================= */
    setupSheet(sheetName, numCols, columnNames);

    /* ================= INSERT ATTENDANCE ROWS ================= */
    insertAttendanceRows(attendanceData, rowCount, columnNames, numCols);

    /* ================= CSV MODE ================= */
    displayCSV(attendanceData, rowCount, columnNames, numCols);

    return 0;
}
