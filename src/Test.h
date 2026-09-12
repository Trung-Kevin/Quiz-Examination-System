#ifndef TEST_H
#define TEST_H

#include <string>
#include <vector>
#include "Course.h"

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
    Test();

    Test(int testId,
         const std::string &title,
         const Course &course,
         int timeLimit,
         const std::string &startTime,
         const std::string &endTime,
         int questionCount,
         TestStatus status);

    int getTestId() const;
    std::string getTitle() const;
    Course getCourse() const;
    int getTimeLimit() const;
    std::string getStartTime() const;
    std::string getEndTime() const;
    int getQuestionCount() const;
    TestStatus getStatus() const;

    void setTitle(const std::string &title);
    void setCourse(const Course &course);
    void setTimeLimit(int timeLimit);
    void setStartTime(const std::string &startTime);
    void setEndTime(const std::string &endTime);
    void setQuestionCount(int questionCount);
    void setStatus(TestStatus status);

    void addQuestion();
    void removeQuestion();
    void update();
    void publish();
    void unpublish();
    void deleteTest();
};

#endif