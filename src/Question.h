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
    Question(int id = 0,
             const std::string &content = "",
             DifficultyLevel difficulty = DifficultyLevel::Easy);

    void update();
    void deleteQuestion();
    std::vector<Question> search();

    int getQuestionId() const;
    std::string getContent() const;
    DifficultyLevel getDifficultyLevel() const;

    void setQuestionId(int id);
    void setContent(const std::string &value);
    void setDifficultyLevel(DifficultyLevel value);
};

#endif