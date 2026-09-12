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
    TestAttempt(int attemptId, int testId, int studentId);

    void submit();

    int getAttemptId() const;
    int getTestId() const;
    int getStudentId() const;

    std::string getStartTime() const;
    std::string getSubmitTime() const;

    AttemptStatus getStatus() const;
    bool isSubmitted() const;
};

#endif