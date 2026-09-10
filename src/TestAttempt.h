#ifndef TESTATTEMPT_H
#define TESTATTEMPT_H

#include <string>

using namespace std;

enum class AttemptStatus
{
    InProgress,
    Submitted,
    TimeOut
};

class TestAttempt
{
private:
    int attemptId;
    int testId;
    int studentId;
    string startTime;
    string submitTime;
    AttemptStatus status;

public:
    void submit();
};

#endif