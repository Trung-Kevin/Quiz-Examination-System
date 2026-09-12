#ifndef RESULT_H
#define RESULT_H

#include <string>
#include <vector>
#include "StudentAnswer.h"

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
    Result(int resultId, int attemptId);

    void calculate();

    void calculate(const std::vector<StudentAnswer>& answers,
                   double maximumScore,
                   int secondsTaken,
                   const std::string& submittedTime);

    double getTotalScore() const;
    double getMaxScore() const;
    double getPercentage() const;

    int getCorrectCount() const;
    int getWrongCount() const;
    int getUnansweredCount() const;

    int getTimeTaken() const;
    std::string getSubmittedAt() const;

    std::string getGrade() const;
};

#endif