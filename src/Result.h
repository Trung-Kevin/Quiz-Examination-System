#ifndef RESULT_H
#define RESULT_H

#include <string>

class Result
{
private:
    int resultId;
    int attemptId;
    double totalScore;
    double maxScore;
    int correctCount;
    int wrongCount;
    int unansweredCount;
    int timeTaken;
    std::string submittedAt;

public:
    void calculate();
};

#endif