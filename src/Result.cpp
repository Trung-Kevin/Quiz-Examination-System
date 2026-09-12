#include "Result.h"

Result::Result(int resultId, int attemptId)
    : resultId(resultId),
      attemptId(attemptId),
      totalScore(0.0),
      maxScore(0.0),
      correctCount(0),
      wrongCount(0),
      unansweredCount(0),
      timeTaken(0),
      submittedAt("")
{
}

void Result::calculate()
{
    // Calculation is performed by the overloaded calculate() method.
}

void Result::calculate(const std::vector<StudentAnswer> &answers,
                       double maximumScore,
                       int secondsTaken,
                       const std::string &submittedTime)
{
    totalScore = 0.0;
    maxScore = maximumScore;
    correctCount = 0;
    wrongCount = 0;
    unansweredCount = 0;
    timeTaken = secondsTaken;
    submittedAt = submittedTime;

    for (const StudentAnswer &answer : answers)
    {
        // Chưa trả lời
        if (answer.getSelectedOptionId() == -1 &&
            answer.getAnswerText().empty())
        {
            unansweredCount++;
        }
        // Trả lời đúng
        else if (answer.getIsCorrect())
        {
            correctCount++;
            totalScore += answer.getScore();
        }
        // Trả lời sai
        else
        {
            wrongCount++;
        }
    }
}

double Result::getTotalScore() const
{
    return totalScore;
}

double Result::getMaxScore() const
{
    return maxScore;
}

double Result::getPercentage() const
{
    if (maxScore <= 0.0)
        return 0.0;

    return (totalScore / maxScore) * 100.0;
}

int Result::getCorrectCount() const
{
    return correctCount;
}

int Result::getWrongCount() const
{
    return wrongCount;
}

int Result::getUnansweredCount() const
{
    return unansweredCount;
}

int Result::getTimeTaken() const
{
    return timeTaken;
}

std::string Result::getSubmittedAt() const
{
    return submittedAt;
}

std::string Result::getGrade() const
{
    double percentage = getPercentage();

    if (percentage >= 90.0)
        return "A";
    if (percentage >= 80.0)
        return "B";
    if (percentage >= 70.0)
        return "C";
    if (percentage >= 60.0)
        return "D";

    return "F";
}