#include "Student.h"
#include <iostream>

using namespace std;

// Constructor mặc định
Student::Student() : User() {
    studentID = "";
}

// Constructor có tham số
Student::Student(string userID,
                 string username,
                 string password,
                 string email,
                 string fullName,
                 string dateOfBirth,
                 string status,
                 string studentID)
    : User(userID, username, password, email,
           fullName, dateOfBirth, status) {

    this->studentID = studentID;
}

// Register
void Student::registerAccount() {
    cout << "Student registration." << endl;
}

// View Subject
void Student::viewSubject() {
    cout << "Viewing subjects." << endl;
}

// View Tests
void Student::viewTests() {
    cout << "Viewing available tests." << endl;
}

// Take Test
void Student::takeTest() {
    cout << "Taking test." << endl;
}

// View Results
void Student::viewResults() {
    cout << "Viewing results." << endl;
}

// Getter
string Student::getStudentID() {
    return studentID;
}