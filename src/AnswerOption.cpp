#include "AnswerOption.h"

AnswerOption::AnswerOption(int id,
                           const std::string &content,
                           bool correct)
    : optionId(id),
      content(content),
      isCorrect(correct)
{
}

void AnswerOption::update()
{
    // Actual update is handled by QuestionGui.
}

int AnswerOption::getOptionId() const
{
    return optionId;
}

std::string AnswerOption::getContent() const
{
    return content;
}

bool AnswerOption::getIsCorrect() const
{
    return isCorrect;
}

void AnswerOption::setOptionId(int id)
{
    optionId = id;
}

void AnswerOption::setContent(const std::string &value)
{
    content = value;
}

void AnswerOption::setIsCorrect(bool value)
{
    isCorrect = value;
}