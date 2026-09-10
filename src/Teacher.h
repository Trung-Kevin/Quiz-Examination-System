#ifndef TEACHER_H
#define TEACHER_H

#include <string>
#include "User.h"

using namespace std;

class Teacher : public User
{
private:
    string teacherID;

public:
    void manageCourse();
    void manageQuestions();
    void manageTests();
    void viewResults();
};

#endif