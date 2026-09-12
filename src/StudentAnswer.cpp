#include "StudentAnswer.h"
#include <algorithm>
#include <cctype>

StudentAnswer::StudentAnswer(int answerId, int attemptId, int questionId)
{
    this->answerId = answerId;
    this->attemptId = attemptId;
    this->questionId = questionId;

    this->selectedOptionId = -1;
    this->answerText = "";

    this->isCorrect = false;
    this->score = 0.0;
}

void StudentAnswer::setSelectedOption(int optionId)
{
    selectedOptionId = optionId;
}

void StudentAnswer::setAnswerText(const std::string &text)
{
    answerText = text;
}

void StudentAnswer::evaluate(int correctOptionId, double questionScore)
{
    if (selectedOptionId == -1)
    {
        isCorrect = false;
        score = 0.0;
        return;
    }

    if (selectedOptionId == correctOptionId)
    {
        isCorrect = true;
        score = questionScore;
    }
    else
    {
        isCorrect = false;
        score = 0.0;
    }
}

void StudentAnswer::evaluateWritten(
    const std::string &correctAnswer,
    double questionScore)
{
    std::string student = answerText;
    std::string correct = correctAnswer;

    std::transform(student.begin(), student.end(), student.begin(),
                   [](unsigned char c)
                   {
                       return static_cast<char>(std::tolower(c));
                   });

    std::transform(correct.begin(), correct.end(), correct.begin(),
                   [](unsigned char c)
                   {
                       return static_cast<char>(std::tolower(c));
                   });

    // Remove spaces at beginning/end
    student.erase(0, student.find_first_not_of(" \t\r\n"));
    student.erase(student.find_last_not_of(" \t\r\n") + 1);

    correct.erase(0, correct.find_first_not_of(" \t\r\n"));
    correct.erase(correct.find_last_not_of(" \t\r\n") + 1);

    if (!student.empty() && student == correct)
    {
        isCorrect = true;
        score = questionScore;
    }
    else
    {
        isCorrect = false;
        score = 0.0;
    }
}

void StudentAnswer::clearAnswer()
{
    selectedOptionId = -1;
    answerText = "";
    isCorrect = false;
    score = 0.0;
}

int StudentAnswer::getAnswerId() const
{
    return answerId;
}

int StudentAnswer::getAttemptId() const
{
    return attemptId;
}

int StudentAnswer::getQuestionId() const
{
    return questionId;
}

int StudentAnswer::getSelectedOptionId() const
{
    return selectedOptionId;
}

std::string StudentAnswer::getAnswerText() const
{
    return answerText;
}

bool StudentAnswer::getIsCorrect() const
{
    return isCorrect;
}

double StudentAnswer::getScore() const
{
    return score;
}