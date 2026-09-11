#include "TestQuestion.h"

TestQuestion::TestQuestion()
    : testId(0),
      questionId(0),
      questionOrder(0),
      score(0.0)
{
}

TestQuestion::TestQuestion(int testId,
                           int questionId,
                           int questionOrder,
                           double score)
    : testId(testId),
      questionId(questionId),
      questionOrder(questionOrder),
      score(score)
{
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

void TestQuestion::setTestId(int testId)
{
    this->testId = testId;
}

void TestQuestion::setQuestionId(int questionId)
{
    this->questionId = questionId;
}

void TestQuestion::setQuestionOrder(int questionOrder)
{
    this->questionOrder = questionOrder;
}

void TestQuestion::setScore(double score)
{
    this->score = score;
}
