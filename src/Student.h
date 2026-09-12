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

    bool registerAccount(
        const std::string &fullName,
        const std::string &email,
        const std::string &username,
        const std::string &password,
        const std::string &dateOfBirth);

    std::vector<Course> viewSubject();
    std::vector<Test> viewTests();
    TestAttempt takeTest();
    std::vector<Result> viewResults();
};

#endif