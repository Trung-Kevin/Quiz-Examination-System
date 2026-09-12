#include "Question.h"

Question::Question(int id,
                   int topicId,
                   const std::string &content,
                   DifficultyLevel difficulty)
    : questionId(id),
      topicId(topicId),
      content(content),
      difficultyLevel(difficulty)
{
}

Question::Question(int id,
                   const std::string &content,
                   DifficultyLevel difficulty)
    : questionId(id),
      topicId(0),
      content(content),
      difficultyLevel(difficulty)
{
}

void Question::update()
{
    // Actual update is handled by QuestionBank / GUI.
}

void Question::deleteQuestion()
{
    // Actual deletion is handled by QuestionBank / GUI.
}

std::vector<Question> Question::search()
{
    // Actual search is handled by QuestionBank.
    return {};
}

int Question::getQuestionId() const
{
    return questionId;
}

int Question::getTopicId() const
{
    return topicId;
}

std::string Question::getContent() const
{
    return content;
}

DifficultyLevel Question::getDifficultyLevel() const
{
    return difficultyLevel;
}

void Question::setQuestionId(int id)
{
    questionId = id;
}

void Question::setTopicId(int id)
{
    topicId = id;
}

void Question::setContent(const std::string &value)
{
    content = value;
}

void Question::setDifficultyLevel(DifficultyLevel value)
{
    difficultyLevel = value;
}