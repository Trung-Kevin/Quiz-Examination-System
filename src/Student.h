#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"

class Student : public User {
private:
    string studentID;

public:
    Student();

    Student(string userID,
            string username,
            string password,
            string email,
            string fullName,
            string dateOfBirth,
            string status,
            string studentID);

    void registerAccount();
    void viewSubject();
    void viewTests();
    void takeTest();
    void viewResults();

    string getStudentID();
};

#endif