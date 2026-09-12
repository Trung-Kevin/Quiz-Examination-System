#include "Test.h"

Test::Test()
{
    testId = 0;
    title = "";
    timeLimit = 0;
    startTime = "";
    endTime = "";
    questionCount = 0;
    status = TestStatus::Draft;
}

Test::Test(int testId,
           const std::string &title,
           const Course &course,
           int timeLimit,
           const std::string &startTime,
           const std::string &endTime,
           int questionCount,
           TestStatus status)
{
    this->testId = testId;
    this->title = title;
    this->course = course;
    this->timeLimit = timeLimit;
    this->startTime = startTime;
    this->endTime = endTime;
    this->questionCount = questionCount;
    this->status = status;
}

int Test::getTestId() const
{
    return testId;
}

std::string Test::getTitle() const
{
    return title;
}

Course Test::getCourse() const
{
    return course;
}

int Test::getTimeLimit() const
{
    return timeLimit;
}

std::string Test::getStartTime() const
{
    return startTime;
}

std::string Test::getEndTime() const
{
    return endTime;
}

int Test::getQuestionCount() const
{
    return questionCount;
}

TestStatus Test::getStatus() const
{
    return status;
}

void Test::setTitle(const std::string &title)
{
    this->title = title;
}

void Test::setCourse(const Course &course)
{
    this->course = course;
}

void Test::setTimeLimit(int timeLimit)
{
    if (timeLimit > 0)
    {
        this->timeLimit = timeLimit;
    }
}

void Test::setStartTime(const std::string &startTime)
{
    this->startTime = startTime;
}

void Test::setEndTime(const std::string &endTime)
{
    this->endTime = endTime;
}

void Test::setQuestionCount(int questionCount)
{
    if (questionCount >= 0)
    {
        this->questionCount = questionCount;
    }
}

void Test::setStatus(TestStatus status)
{
    this->status = status;
}

void Test::addQuestion()
{
    questionCount++;
}

void Test::removeQuestion()
{
    if (questionCount > 0)
    {
        questionCount--;
    }
}

void Test::update()
{
    // Test information is updated through setter methods.
}

void Test::publish()
{
    status = TestStatus::Published;
}

void Test::unpublish()
{
    if (status == TestStatus::Published)
    {
        status = TestStatus::Draft;
    }
}

void Test::deleteTest()
{
    // Removing the object from the test collection is handled by the GUI.
}