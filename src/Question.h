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
    int topicId;
    std::string content;
    DifficultyLevel difficultyLevel;

public:
    Question(int id = 0,
             int topicId = 0,
             const std::string &content = "",
             DifficultyLevel difficulty = DifficultyLevel::Easy);

    Question(int id,
             const std::string &content,
             DifficultyLevel difficulty);

    void update();
    void deleteQuestion();
    std::vector<Question> search();

    int getQuestionId() const;
    int getTopicId() const;
    std::string getContent() const;
    DifficultyLevel getDifficultyLevel() const;

    void setQuestionId(int id);
    void setTopicId(int id);
    void setContent(const std::string &value);
    void setDifficultyLevel(DifficultyLevel value);
};

#endif