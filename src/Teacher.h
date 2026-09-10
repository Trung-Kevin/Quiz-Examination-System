#ifndef TEACHER_H
#define TEACHER_H

#include "User.h"
#include <string>

class Teacher : public User
{
private:
    std::string teacherID;

public:
    void manageCourse();
    void manageQuestions();
    void manageTests();
    void viewResults();
};

#endif