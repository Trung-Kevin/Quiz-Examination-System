#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include "User.h"
#include "Course.h"
#include "Test.h"
#include "TestAttempt.h"
#include "Result.h"

using namespace std;

class Student : public User
{
private:
    string studentID;

public:
    bool registerAccount();
    vector<Course> viewSubject();
    vector<Test> viewTests();
    TestAttempt takeTest();
    vector<Result> viewResults();
};

#endif