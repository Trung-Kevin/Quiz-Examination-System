#include <iostream>
#include "Admin.h"

using namespace std;

int main() {
    Admin adminSystem("ADM100", 1, "admin", "admin123", "admin@system.com", "Quan Tri Vien");

    int choice = -1;

    do {
        cout << "\n==========================================" << endl;
        cout << "     HE THONG QUAN LY ADMIN (PHAN 5)      " << endl;
        cout << "==========================================" << endl;
        cout << "1. Quan ly Nguoi dung (User Management)" << endl;
        cout << "2. Quan ly Phan quyen (Permission Management)" << endl;
        cout << "3. Quan ly Du lieu (Data Backup & Restore)" << endl;
        cout << "4. Quan ly De thi (Test Management)" << endl;
        cout << "0. Thoat chuong trinh" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Nhap lua chon cua ban (0-4): ";
        cin >> choice;

        switch (choice) {
            case 1:
                adminSystem.manageUsers();
                break;

            case 2:
                adminSystem.managePermissions();
                break;

            case 3:
                adminSystem.manageData();
                break;

            case 4:
                adminSystem.manageTests();
                break;

            case 0:
                cout << "\n=> Da thoat chuong trinh. Tam biet!" << endl;
                break;

            default:
                cout << "\n=> Lua chon khong hop le! Vui long nhap tu 0 den 4." << endl;
                break;
        }

    } while (choice != 0);

    return 0;
}