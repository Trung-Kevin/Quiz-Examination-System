#ifndef TEACHER_H
#define TEACHER_H

#include "User.h"

class Teacher : public User {
private:
    string teacherID;

public:
    Teacher();

    Teacher(string userID,
            string username,
            string password,
            string email,
            string fullName,
            string dateOfBirth,
            string status,
            string teacherID);

    void manageCourse();
    void manageQuestions();
    void manageTests();
    void viewResults();

    string getTeacherID();
};

#endif