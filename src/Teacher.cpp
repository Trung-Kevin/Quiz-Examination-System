#include "Teacher.h"
#include <iostream>

using namespace std;

// Constructor mặc định
Teacher::Teacher() : User() {
    teacherID = "";
}

// Constructor có tham số
Teacher::Teacher(string userID,
                 string username,
                 string password,
                 string email,
                 string fullName,
                 string dateOfBirth,
                 string status,
                 string teacherID)
    : User(userID, username, password, email,
           fullName, dateOfBirth, status) {

    this->teacherID = teacherID;
}

// Manage Course
void Teacher::manageCourse() {
    cout << "Managing courses." << endl;
}

// Manage Questions
void Teacher::manageQuestions() {
    cout << "Managing questions." << endl;
}

// Manage Tests
void Teacher::manageTests() {
    cout << "Managing tests." << endl;
}

// View Results
void Teacher::viewResults() {
    cout << "Viewing student results." << endl;
}

// Getter
string Teacher::getTeacherID() {
    return teacherID;
}