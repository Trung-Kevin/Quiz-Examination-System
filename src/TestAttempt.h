#ifndef TESTATTEMPT_H
#define TESTATTEMPT_H

#include <string>

enum class AttemptStatus
{
    InProgress,
    Submitted
};

class TestAttempt
{
private:
    int attemptId;
    int testId;
    int studentId;
    std::string startTime;
    std::string submitTime;
    AttemptStatus status;

public:
    void submit();
};

#endif