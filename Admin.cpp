#include "Admin.h"
#include <iostream>
#include <fstream>

using namespace std;

Admin::Admin() 
    : User(), adminID("ADMIN_000") {}

Admin::Admin(string id, int uId, string uname, string pwd, string mail, string name)
    : User(uId, uname, pwd, mail, name, "Admin"), adminID(id) 
{
    // Tap du lieu gia dinh ban dau de test
    userList.push_back(User(101, "teacher1", "123", "teacher@school.edu.vn", "Nguyen Van Thay", "Teacher"));
    userList.push_back(User(102, "student1", "123", "student@school.edu.vn", "Tran Van Tro", "Student"));
}

// ================= 1. QUAN LY NGUOI DUNG =================
void Admin::manageUsers() {
    int choice = -1;
    do {
        cout << "\n==========================================" << endl;
        cout << "   1. QUAN LY NGUOI DUNG (USER MANAGEMENT)" << endl;
        cout << "==========================================" << endl;
        cout << "1. Xem danh sach nguoi dung" << endl;
        cout << "2. Them nguoi dung moi" << endl;
        cout << "3. Cap nhat thong tin nguoi dung" << endl;
        cout << "4. Xoa nguoi dung" << endl;
        cout << "0. Quay lai Menu Admin" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Nhap lua chon cua ban (0-4): ";
        cin >> choice;

        switch (choice) {
            case 1:
                listAllUsers();
                break;

            case 2: {
                int id;
                string uname, pwd, mail, name, role;
                cout << "\n--- THEM NGUOI DUNG MOI ---" << endl;
                cout << "Nhap ID: "; cin >> id;
                cout << "Nhap Username: "; cin >> uname;
                cout << "Nhap Password: "; cin >> pwd;
                cout << "Nhap Email: "; cin >> mail;
                cin.ignore();
                cout << "Nhap Ho va Ten: "; getline(cin, name);
                cout << "Nhap Vai tro (Student/Teacher/Admin): "; cin >> role;
                
                addUser(User(id, uname, pwd, mail, name, role));
                break;
            }

            case 3: {
                int id;
                cout << "\n--- CAP NHAT THONG TIN ---" << endl;
                cout << "Nhap ID nguoi dung can sua: "; cin >> id;
                updateUser(id);
                break;
            }

            case 4: {
                int id;
                cout << "\n--- XOA NGUOI DUNG ---" << endl;
                cout << "Nhap ID nguoi dung can xoa: "; cin >> id;
                deleteUser(id);
                break;
            }

            case 0:
                cout << "=> Da quay lai Menu Admin." << endl;
                break;

            default:
                cout << "=> Lua chon khong hop le! Vui long nhap lai." << endl;
                break;
        }
    } while (choice != 0);
}

// ================= 2. QUAN LY PHAN QUYEN =================
void Admin::managePermissions() {
    int choice = -1;
    do {
        cout << "\n==========================================" << endl;
        cout << " 2. QUAN LY PHAN QUYEN (PERMISSION MANAGEMENT)" << endl;
        cout << "==========================================" << endl;
        cout << "1. Xem danh sach va vai tro hien tai" << endl;
        cout << "2. Cap / Doi vai tro (Role) cho nguoi dung" << endl;
        cout << "3. Thu hoi vai tro (Reset ve Student)" << endl;
        cout << "0. Quay lai Menu Admin" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Nhap lua chon cua ban (0-3): ";
        cin >> choice;

        switch (choice) {
            case 1:
                listAllUsers();
                break;

            case 2: {
                int id;
                string role;
                cout << "\nNhap User ID: "; cin >> id;
                cout << "Nhap Vai tro moi (Admin/Teacher/Student): "; cin >> role;
                assignRole(id, role);
                break;
            }

            case 3: {
                int id;
                cout << "\nNhap User ID can thu hoi quyen: "; cin >> id;
                revokeRole(id);
                break;
            }

            case 0:
                cout << "=> Da quay lai Menu Admin." << endl;
                break;

            default:
                cout << "=> Lua chon khong hop le!" << endl;
                break;
        }
    } while (choice != 0);
}

// ================= 3. QUAN LY DU LIEU =================
void Admin::manageData() {
    int choice = -1;
    string filename = "backup_data.dat";
    do {
        cout << "\n==========================================" << endl;
        cout << " 3. QUAN LY DU LIEU (DATA MANAGEMENT)" << endl;
        cout << "==========================================" << endl;
        cout << "1. Sao luu du lieu nguoi dung ra file (Backup)" << endl;
        cout << "2. Phuc hoi du lieu tu file (Restore)" << endl;
        cout << "3. Xoa nhap nhat nhat ky he thong (Clear Logs)" << endl;
        cout << "0. Quay lai Menu Admin" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Nhap lua chon cua ban (0-3): ";
        cin >> choice;

        switch (choice) {
            case 1:
                backupData(filename);
                break;

            case 2:
                restoreData(filename);
                break;

            case 3:
                clearSystemLogs();
                break;

            case 0:
                cout << "=> Da quay lai Menu Admin." << endl;
                break;

            default:
                cout << "=> Lua chon khong hop le!" << endl;
                break;
        }
    } while (choice != 0);
}

// ================= 4. QUAN LY DE THI (VIEW) =================
void Admin::manageTests() {
    int choice = -1;
    do {
        cout << "\n==========================================" << endl;
        cout << "    4. QUAN LY DE THI (TEST MANAGEMENT)" << endl;
        cout << "==========================================" << endl;
        cout << "1. Xem danh sach tat ca bai thi trong he thong" << endl;
        cout << "2. Khoa / Xoa bai thi vi pham" << endl;
        cout << "0. Quay lai Menu Admin" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Nhap lua chon cua ban (0-2): ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "=> Danh sach bai thi hien co: [TEST01 - C++], [TEST02 - CSDL]" << endl;
                break;
            case 2:
                cout << "=> Da xoa/khoa bai thi vi pham thanh cong!" << endl;
                break;
            case 0:
                cout << "=> Da quay lai Menu Admin." << endl;
                break;
            default:
                cout << "=> Lua chon khong hop le!" << endl;
                break;
        }
    } while (choice != 0);
}

// ================= LOGIC XU LY DU LIEU =================
void Admin::addUser(const User& newUser) {
    userList.push_back(newUser);
    cout << "=> Da them nguoi dung [" << newUser.getUsername() << "] thanh cong!" << endl;
}

void Admin::updateUser(int uId) {
    for (auto& user : userList) {
        if (user.getUserId() == uId) {
            string name, email;
            cin.ignore();
            cout << "Nhap Ho ten moi: "; getline(cin, name);
            cout << "Nhap Email moi: "; cin >> email;
            
            user.setFullName(name);
            user.setEmail(email);
            cout << "=> Cap nhat thong tin thanh cong!" << endl;
            return;
        }
    }
    cout << "=> Loi: Khong tim thay User ID: " << uId << endl;
}

void Admin::deleteUser(int uId) {
    for (auto it = userList.begin(); it != userList.end(); ++it) {
        if (it->getUserId() == uId) {
            cout << "=> Da xoa nguoi dung [" << it->getUsername() << "] khoi he thong." << endl;
            userList.erase(it);
            return;
        }
    }
    cout << "=> Loi: Khong tim thay User ID: " << uId << endl;
}

void Admin::listAllUsers() const {
    cout << "\n--- DANH SACH NGUOI DUNG (" << userList.size() << ") ---" << endl;
    if (userList.empty()) {
        cout << "(Danh sach trong)" << endl;
        return;
    }
    for (const auto& user : userList) {
        user.displayInfo();
    }
}

void Admin::assignRole(int uId, const string& role) {
    for (auto& user : userList) {
        if (user.getUserId() == uId) {
            user.setRole(role);
            cout << "=> Da phan quyen thanh '" << role << "' cho User ID: " << uId << endl;
            return;
        }
    }
    cout << "=> Loi: Khong tim thay User ID: " << uId << endl;
}

void Admin::revokeRole(int uId) {
    assignRole(uId, "Student");
}

bool Admin::backupData(const string& backupPath) {
    ofstream outFile(backupPath);
    if (!outFile.is_open()) {
        cout << "=> Loi: Khong the tao file sao luu!" << endl;
        return false;
    }
    for (const auto& user : userList) {
        outFile << user.getUserId() << ","
                << user.getUsername() << ","
                << user.getFullName() << ","
                << user.getRole() << "\n";
    }
    outFile.close();
    cout << "=> Sao luu thanh cong " << userList.size() << " nguoi dung vao file '" << backupPath << "'!" << endl;
    return true;
}

bool Admin::restoreData(const string& backupPath) {
    ifstream inFile(backupPath);
    if (!inFile.is_open()) {
        cout << "=> Loi: File sao luu khong ton tai!" << endl;
        return false;
    }
    userList.clear();
    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        size_t p3 = line.find(',', p2 + 1);

        int id = stoi(line.substr(0, p1));
        string uname = line.substr(p1 + 1, p2 - p1 - 1);
        string name = line.substr(p2 + 1, p3 - p2 - 1);
        string role = line.substr(p3 + 1);

        userList.push_back(User(id, uname, "123", uname + "@school.edu.vn", name, role));
    }
    inFile.close();
    cout << "=> Phuc hoi du lieu thanh cong! Tong cong: " << userList.size() << " nguoi dung." << endl;
    return true;
}

void Admin::clearSystemLogs() {
    cout << "=> Da xoa toan bo nhat ky hoat dong cua he thong." << endl;
}