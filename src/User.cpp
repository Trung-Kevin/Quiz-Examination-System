#include "User.h"
#include <iostream>

using namespace std;

// Constructor mặc định
User::User() {
    userID = "";
    username = "";
    password = "";
    email = "";
    fullName = "";
    dateOfBirth = "";
    status = "";
}

// Constructor có tham số
User::User(string userID,
           string username,
           string password,
           string email,
           string fullName,
           string dateOfBirth,
           string status) {

    this->userID = userID;
    this->username = username;
    this->password = password;
    this->email = email;
    this->fullName = fullName;
    this->dateOfBirth = dateOfBirth;
    this->status = status;
}

// UC01 - Login
bool User::login(string username, string password) {
    return this->username == username &&
           this->password == password;
}

// UC03 - Update Information
void User::updateProfile(string email,
                         string fullName,
                         string dateOfBirth) {

    this->email = email;
    this->fullName = fullName;
    this->dateOfBirth = dateOfBirth;
}

// UC02 - Recover Password
void User::recoverPassword() {
    cout << "Password recovery requested for: "
         << email << endl;
}

// Change password
void User::changePassword(string newPassword) {
    this->password = newPassword;
}

// Getter
string User::getUserID() {
    return userID;
}

string User::getUsername() {
    return username;
}

string User::getPassword() {
    return password;
}

string User::getEmail() {
    return email;
}

string User::getFullName() {
    return fullName;
}

string User::getDateOfBirth() {
    return dateOfBirth;
}

string User::getStatus() {
    return status;
}

// Setter
void User::setEmail(string email) {
    this->email = email;
}

void User::setFullName(string fullName) {
    this->fullName = fullName;
}

void User::setDateOfBirth(string dateOfBirth) {
    this->dateOfBirth = dateOfBirth;
}

void User::setPassword(string password) {
    this->password = password;
}