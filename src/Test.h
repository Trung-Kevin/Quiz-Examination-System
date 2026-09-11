#ifndef TEST_H
#define TEST_H

#include "Course.h"
#include "TestQuestion.h"
#include <string>
#include <vector>

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

    // Danh sách câu hỏi thuộc Test
    std::vector<TestQuestion> questions;

public:
    // Constructor
    Test();
    Test(int testId,
         const std::string& title,
         const Course& course,
         int timeLimit,
         const std::string& startTime,
         const std::string& endTime);

    // Test Management
    void addQuestion();
    void removeQuestion();
    void update();
    void publish();
    void deleteTest();

    // Getter
    int getTestId() const;
    std::string getTitle() const;
    int getTimeLimit() const;
    std::string getStartTime() const;
    std::string getEndTime() const;
    int getQuestionCount() const;
    TestStatus getStatus() const;
    const std::vector<TestQuestion>& getQuestions() const;
};

#endif
