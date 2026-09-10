#ifndef TEST_H
#define TEST_H

#include "Course.h"
#include <string>

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
    std::string title;
    Course course;
    int timeLimit;
    std::string startTime;
    std::string endTime;
    int questionCount;
    TestStatus status;

public:
    void addQuestion();
    void removeQuestion();
    void update();
    void publish();
    void deleteTest();
};

#endif