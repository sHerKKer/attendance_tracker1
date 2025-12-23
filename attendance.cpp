#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

const int MAX_COL = 10;
const int MAX_ROW = 100;

// Function prototype
bool isValidInt(const string& input);

int main() {

    string sheetName;
    int numCols;
    string columnNames[MAX_COL];

    int rowCount = 0;
    string attendanceData[MAX_ROW][MAX_COL];

    /* ================= HEADER ================= */

    cout << "===========================================\n";
    cout << "   STUDENT ATTENDANCE TRACKER - MILESTONE 1\n";
    cout << "===========================================\n\n";

    /* ================= SHEET NAME ================= */

    cout << "Enter attendance sheet name: ";
    getline(cin, sheetName);

    cout << "Attendance sheet \"" << sheetName
         << "\" created successfully.\n\n";

    /* ================= COLUMN SETUP ================= */

    cout << "Define number of columns (max 10): ";
    cin >> numCols;
    cin.ignore();

    if (numCols <= 0 || numCols > MAX_COL) {
        cout << "Error: Invalid number of columns.\n";
        return 0;
    }

    for (int i = 0; i < numCols; i++) {
        cout << "Enter column " << i + 1 << " name: ";
        getline(cin, columnNames[i]);
    }

    cout << "\nSheet structure created successfully.\n\n";

    /* ================= INSERT ATTENDANCE ROWS ================= */

string fileName = sheetName + ".txt";
ofstream outFile(fileName.c_str(), ios::app);
string input;

// -------- First Row --------
cout << "-------------------------------------------\n";
cout << "Insert New Attendance Row\n";
cout << "-------------------------------------------\n";

for (int col = 0; col < numCols; col++) {

    cout << "Enter " << columnNames[col] << ": ";

    // Assume FIRST column is INT (StudentID)
    if (col == 0) {
        cin >> input;

        if (!isValidInt(input)) {
            cout << "Error: Invalid INT value. Please enter a number.\n";
            col--;              // retry same column
            continue;
        }
        attendanceData[rowCount][col] = input;
        outFile << input;
        cin.ignore();
    }
    else {
        getline(cin, input);
        attendanceData[rowCount][col] = input;
        outFile << input;
    }

    if (col < numCols - 1)
        outFile << ",";
}

outFile << endl;
rowCount++;

cout << "Row inserted successfully.\n\n";

// -------- Second Row --------
cout << "-------------------------------------------\n";
cout << "Insert New Attendance Row\n";
cout << "-------------------------------------------\n";

for (int col = 0; col < numCols; col++) {

    cout << "Enter " << columnNames[col] << ": ";

    if (col == 0) {
        cin >> input;

        if (!isValidInt(input)) {
            cout << "Error: Invalid INT value. Please enter a number.\n";
            col--;
            continue;
        }
        attendanceData[rowCount][col] = input;
        outFile << input;
        cin.ignore();
    }
    else {
        getline(cin, input);
        attendanceData[rowCount][col] = input;
        outFile << input;
    }

    if (col < numCols - 1)
        outFile << ",";
}

outFile << endl;
rowCount++;

cout << "Row inserted successfully.\n\n";

outFile.close();


    return 0;
}

/* ================= VALIDATE INT ================= */

bool isValidInt(const string& input) {
    stringstream ss(input);
    int num;
    ss >> num;
    return !ss.fail() && ss.eof();
}


