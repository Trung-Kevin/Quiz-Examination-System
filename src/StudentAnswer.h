#ifndef STUDENTANSWER_H
#define STUDENTANSWER_H

class StudentAnswer
{
private:
    int answerId;
    int attemptId;
    int questionId;
    int selectedOptionId;
    bool isCorrect;
    double score;
};

#endif