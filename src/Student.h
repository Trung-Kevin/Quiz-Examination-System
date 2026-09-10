#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include <vector>
#include <string>

class Course;
class Test;
class TestAttempt;
class Result;

class Student : public User
{
private:
    std::string studentID;

public:
    bool registerAccount();
    std::vector<Course> viewSubject();
    std::vector<Test> viewTests();
    TestAttempt takeTest();
    std::vector<Result> viewResults();
};

#endif