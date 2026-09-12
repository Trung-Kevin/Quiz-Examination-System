#include "TestQuestion.h"

TestQuestion::TestQuestion()
{
    testId = 0;
    questionId = 0;
    questionOrder = 0;
    score = 0.0;
}

TestQuestion::TestQuestion(int testId,
                           int questionId,
                           int questionOrder,
                           double score)
{
    this->testId = testId;
    this->questionId = questionId;
    this->questionOrder = questionOrder;
    this->score = score;
}

int TestQuestion::getTestId() const
{
    return testId;
}

int TestQuestion::getQuestionId() const
{
    return questionId;
}

int TestQuestion::getQuestionOrder() const
{
    return questionOrder;
}

double TestQuestion::getScore() const
{
    return score;
}

void TestQuestion::setQuestionOrder(int questionOrder)
{
    if (questionOrder > 0)
    {
        this->questionOrder = questionOrder;
    }
}

void TestQuestion::setScore(double score)
{
    if (score >= 0)
    {
        this->score = score;
    }
}