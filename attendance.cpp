#include <iostream>
#include <string>

using namespace std;

const int MAX_COL = 10;;

int main() {

    string sheetName;
    int numCols;
    string columnNames[MAX_COL];

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

    cout << "\n-------------------------------------------\n";
    cout << "Sheet setup completed successfully.\n";

    return 0;
}
