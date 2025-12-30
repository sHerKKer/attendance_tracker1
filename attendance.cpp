// *********************************************************
// Program: TC1L_Group04_main.cpp
// Course: CCP6114 Programming Fundamentals
// Lecture Class: TC1L
// Tutorial Class: TT2L
// Trimester: 2530
// Member_1: 252UC241V4 | MEGAT HAKIM BIN MEGAT ZUBAIRY           | MEGAT.HAKIM.MEGAT1@student.mmu.edu.my      | 019-2317372
// Member_2: 252UC241P4 | MUHAMMAD AZIM IRFAN BIN HILMI           | MUHAMMAD.AZIM.IRFAN1@student.mmu.edu.my    | 011-29722356
// Member_3: 252UC24328 | MUHAMMAD NAFIS BIN MOHAMED ASLAM        | MUHAMMAD.NAFIS.MOHAMED1@student.mmu.edu.my | 011-61671600
// Member_4: 252UC25344 | MUHAMMAD KHAIR ASYRAF BIN KHAIDIR KHANA | MUHAMMAD.KHAIR.ASYRAF@student.mmu.edu.my   | 012-4996208
// *********************************************************
// Task Distribution
// Member_1: Implemented the CSV mode display function [ displayCSV() ] as well as the flowchart/pseudocode for it.
// Member_2: Worked on insert rows into attendance sheet as well as the flowchart/pseudocode for it.
// Member_3: Worked on the basic error handling and input validation as well as the flowchart/pseudocode for it.
// Member_4: Implemented the attendance sheet structure section, prepared the corresponding flowchart and pseudocode for it.
// *********************************************************

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

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


    bool onlySpaces;

    do{

        onlySpaces = true;

        cout << "Enter attendance sheet name: ";
        getline(cin, sheetName);

        for(int i = 0; i < sheetName.length(); i++) {
            if (sheetName[i] != ' '){
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
                cout << "Error: Column name cannot be empty or just spaces. Please enter again.\n";

        } while (input.empty() || onlySpaces);

        columnNames[i] = input;  // store valid column name
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

bool isValidInt(const string &input) {
    if (input.empty()) return false;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] < '0' || input[i] > '9')
            return false;
    }
    return true;
}

bool isIntColumn(const string &columnName) {
    if (columnName.length() < 3)
        return false;

    for (int i = 0; i <= columnName.length() - 3; i++) {
        if (columnName[i] == 'I' &&
            columnName[i+1] == 'N' &&
            columnName[i+2] == 'T')
            return true;
    }
    return false;
}


void insertAttendanceRows(string attendanceData[][MAX_COL], int& rowCount, string columnNames[], int numCols) {

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
                break;  // valid input
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

        if (continueInsert != 'y' && continueInsert != 'Y' && continueInsert!= 'n' && continueInsert != 'N')
        {
            cout << "Error: Please enter y or n only.\n";
        }
    } while (continueInsert != 'y' && continueInsert != 'Y' && continueInsert != 'n' && continueInsert != 'N');

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
