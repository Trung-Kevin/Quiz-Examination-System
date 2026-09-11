#include "AnswerOption.h"

AnswerOption::AnswerOption()
    : optionId(0),
      questionId(0),
      content(""),
      isCorrect(false)
{
}

AnswerOption::AnswerOption(int optionId,
                           int questionId,
                           const std::string& content,
                           bool isCorrect)
    : optionId(optionId),
      questionId(questionId),
      content(content),
      isCorrect(isCorrect)
{
}

int AnswerOption::getOptionId() const
{
    return optionId;
}

int AnswerOption::getQuestionId() const
{
    return questionId;
}

std::string AnswerOption::getContent() const
{
    return content;
}

bool AnswerOption::getIsCorrect() const
{
    return isCorrect;
}

void AnswerOption::setOptionId(int optionId)
{
    this->optionId = optionId;
}

void AnswerOption::setQuestionId(int questionId)
{
    this->questionId = questionId;
}

void AnswerOption::setContent(const std::string& content)
{
    this->content = content;
}

void AnswerOption::setIsCorrect(bool isCorrect)
{
    this->isCorrect = isCorrect;
}