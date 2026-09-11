#include "Question.h"

Question::Question()
    : questionId(0),
      topicId(0),
      content(""),
      difficultyLevel(DifficultyLevel::Easy)
{
}

Question::Question(int questionId,
                   int topicId,
                   const std::string& content,
                   DifficultyLevel difficultyLevel)
    : questionId(questionId),
      topicId(topicId),
      content(content),
      difficultyLevel(difficultyLevel)
{
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

void Question::setQuestionId(int questionId)
{
    this->questionId = questionId;
}

void Question::setTopicId(int topicId)
{
    this->topicId = topicId;
}

void Question::setContent(const std::string& content)
{
    this->content = content;
}

void Question::setDifficultyLevel(DifficultyLevel difficultyLevel)
{
    this->difficultyLevel = difficultyLevel;
}