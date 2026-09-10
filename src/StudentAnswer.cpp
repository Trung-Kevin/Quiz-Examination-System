#include "StudentAnswer.h"
#include <algorithm>
#include <cctype>

static std::string normalizeAnswer(std::string text)
{
    std::transform(text.begin(), text.end(), text.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });

    size_t start = text.find_first_not_of(" \t\n\r");
    size_t end = text.find_last_not_of(" \t\n\r");

    if (start == std::string::npos)
        return "";

    return text.substr(start, end - start + 1);
}

StudentAnswer::StudentAnswer(int answerId, int attemptId, int questionId)
    : answerId(answerId),
      attemptId(attemptId),
      questionId(questionId),
      selectedOptionId(0),
      answerText(""),
      isCorrect(false),
      score(0.0)
{
}

void StudentAnswer::setSelectedOption(int optionId)
{
    selectedOptionId = optionId;
}

void StudentAnswer::setAnswerText(const std::string& text)
{
    answerText = text;
}

void StudentAnswer::evaluate(int correctOptionId, double questionScore)
{
    if (selectedOptionId == 0)
    {
        isCorrect = false;
        score = 0.0;
        return;
    }

    isCorrect = (selectedOptionId == correctOptionId);
    score = isCorrect ? questionScore : 0.0;
}

void StudentAnswer::evaluateWritten(const std::string& correctAnswer,
                                    double questionScore)
{
    if (normalizeAnswer(answerText) == normalizeAnswer(correctAnswer)
        && !normalizeAnswer(answerText).empty())
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
    selectedOptionId = 0;
    answerText.clear();
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