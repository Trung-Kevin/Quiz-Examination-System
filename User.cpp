#include "User.h"
#include <iostream>

using namespace std;

User::User() 
    : userId(0), username(""), password(""), email(""), fullName(""), role("Student") {}

User::User(int uId, string uname, string pwd, string mail, string name, string r)
    : userId(uId), username(uname), password(pwd), email(mail), fullName(name), role(r) {}

int User::getUserId() const { return userId; }
string User::getUsername() const { return username; }
string User::getFullName() const { return fullName; }
string User::getRole() const { return role; }

void User::setRole(const string& newRole) { role = newRole; }
void User::setFullName(const string& newName) { fullName = newName; }
void User::setEmail(const string& newEmail) { email = newEmail; }

bool User::login() { return true; }
void User::logout() {}
void User::updateProfile() {}

void User::displayInfo() const {
    cout << "ID: " << userId 
         << " | Username: " << username 
         << " | Ho ten: " << fullName 
         << " | Email: " << email 
         << " | Role: " << role << endl;
}