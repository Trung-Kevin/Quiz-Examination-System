#ifndef TESTQUESTION_H
#define TESTQUESTION_H

class TestQuestion
{
private:
    int testId;
    int questionId;
    int questionOrder;
    double score;

public:
    TestQuestion();
    TestQuestion(int testId,
                 int questionId,
                 int questionOrder,
                 double score);

    int getTestId() const;
    int getQuestionId() const;
    int getQuestionOrder() const;
    double getScore() const;

    void setTestId(int testId);
    void setQuestionId(int questionId);
    void setQuestionOrder(int questionOrder);
    void setScore(double score);
};

#endif
