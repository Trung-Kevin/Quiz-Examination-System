#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>
#include "AnswerOption.h"

using namespace std;

enum class DifficultyLevel
{
    Easy,
    Medium,
    Hard
};

class Question
{
private:
    int questionId;
    string content;
    DifficultyLevel difficultyLevel;
    vector<AnswerOption> answerOptions;

public:
    void update();
    void deleteQuestion();
    vector<Question> search();
};

#endif