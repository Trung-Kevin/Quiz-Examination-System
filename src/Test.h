#ifndef TEST_H
#define TEST_H

#include <string>
#include <vector>
#include "Course.h"
#include "TestQuestion.h"

using namespace std;

enum class TestStatus
{
    Draft,
    Published,
    Closed
};

class Test
{
private:
    int testId;
    string title;
    Course course;
    int timeLimit;
    string startTime;
    string endTime;
    int questionCount;
    TestStatus status;
    vector<TestQuestion> testQuestions;

public:
    void addQuestion();
    void removeQuestion();
    void update();
    void publish();
    void deleteTest();
};

#endif