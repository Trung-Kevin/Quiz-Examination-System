#ifndef QUESTION_H
#define QUESTION_H

#include <string>

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
    Question();
    Question(int questionId,
             int topicId,
             const std::string& content,
             DifficultyLevel difficultyLevel);

    int getQuestionId() const;
    int getTopicId() const;
    std::string getContent() const;
    DifficultyLevel getDifficultyLevel() const;

    void setQuestionId(int questionId);
    void setTopicId(int topicId);
    void setContent(const std::string& content);
    void setDifficultyLevel(DifficultyLevel difficultyLevel);
};

#endif