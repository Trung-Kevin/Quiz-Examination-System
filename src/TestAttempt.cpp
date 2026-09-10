#include "TestAttempt.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

static std::string getCurrentTime()
{
    auto now = std::chrono::system_clock::now();

    std::time_t currentTime =
        std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    std::ostringstream oss;

    oss << std::put_time(
        &localTime,
        "%Y-%m-%d %H:%M:%S"
    );

    return oss.str();
}

TestAttempt::TestAttempt(
    int attemptId,
    int testId,
    int studentId
)
    : attemptId(attemptId),
      testId(testId),
      studentId(studentId),
      startTime(getCurrentTime()),
      submitTime(""),
      status(AttemptStatus::InProgress)
{
}

void TestAttempt::submit()
{
    if (status == AttemptStatus::Submitted)
        return;

    submitTime = getCurrentTime();

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