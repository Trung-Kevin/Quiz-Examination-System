#ifndef STUDENTANSWER_H
#define STUDENTANSWER_H

#include <string>

class StudentAnswer
{
private:
    int answerId;
    int attemptId;
    int questionId;
    int selectedOptionId;
    std::string answerText;
    bool isCorrect;
    double score;

public:
    StudentAnswer(int answerId, int attemptId, int questionId);

    void setSelectedOption(int optionId);
    void setAnswerText(const std::string& text);

    void evaluate(int correctOptionId, double questionScore);
    void evaluateWritten(const std::string& correctAnswer,
                         double questionScore);

    void clearAnswer();

    int getAnswerId() const;
    int getAttemptId() const;
    int getQuestionId() const;
    int getSelectedOptionId() const;
    std::string getAnswerText() const;
    bool getIsCorrect() const;
    double getScore() const;
};

#endif