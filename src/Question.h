#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

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
    std::string content;
    DifficultyLevel difficultyLevel;

public:
    void update();
    void deleteQuestion();
    std::vector<Question> search();
};

#endif