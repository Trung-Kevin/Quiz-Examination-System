#ifndef RESULT_H
#define RESULT_H

#include <string>

using namespace std;

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
    string submittedAt;

public:
    void calculate();
};

#endif