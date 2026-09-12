#include "TestAttempt.h"
#include <ctime>
#include <iomanip>
#include <sstream>

static std::string getCurrentDateTime()
{
    std::time_t now = std::time(nullptr);
    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

TestAttempt::TestAttempt(int attemptId, int testId, int studentId)
{
    this->attemptId = attemptId;
    this->testId = testId;
    this->studentId = studentId;

    this->startTime = getCurrentDateTime();
    this->submitTime = "";

    this->status = AttemptStatus::InProgress;
}

void TestAttempt::submit()
{
    if (status == AttemptStatus::Submitted)
        return;

    submitTime = getCurrentDateTime();
    status = AttemptStatus::Submitted;
}

int TestAttempt::getAttemptId() const
{
    return attemptId;
}

int TestAttempt::getTestId() const
{
    return testId;
}

int TestAttempt::getStudentId() const
{
    return studentId;
}

std::string TestAttempt::getStartTime() const
{
    return startTime;
}

std::string TestAttempt::getSubmitTime() const
{
    return submitTime;
}

AttemptStatus TestAttempt::getStatus() const
{
    return status;
}

bool TestAttempt::isSubmitted() const
{
    return status == AttemptStatus::Submitted;
}