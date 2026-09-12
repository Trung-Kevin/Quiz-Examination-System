#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include <string>
#include <vector>

using namespace std;

class Admin : public User
{
private:
    string adminID;
    vector<User> userList;

public:
    Admin();
    Admin(string id, int uId, string uname, string pwd, string mail, string name);

    // Cac menu chuc nang chinh theo giao dien phan cong
    void manageUsers();
    void managePermissions();
    void manageData();
    void manageTests();

    // Thao tac du lieu chi tiet
    void addUser(const User& newUser);
    void updateUser(int uId);
    void deleteUser(int uId);
    void listAllUsers() const;

    void assignRole(int uId, const string& role);
    void revokeRole(int uId);

    bool backupData(const string& backupPath);
    bool restoreData(const string& backupPath);
    void clearSystemLogs();
};

#endif