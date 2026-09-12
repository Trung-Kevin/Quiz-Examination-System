#include "AnswerOption.h"

AnswerOption::AnswerOption(int id,
                           int questionId,
                           const std::string &content,
                           bool correct)
    : optionId(id),
      questionId(questionId),
      content(content),
      isCorrect(correct)
{
}

AnswerOption::AnswerOption(int id,
                           const std::string &content,
                           bool correct)
    : optionId(id),
      questionId(0),
      content(content),
      isCorrect(correct)
{
}

void AnswerOption::update()
{
    // Actual update is handled by QuestionBank / GUI.
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

void AnswerOption::setOptionId(int id)
{
    optionId = id;
}

void AnswerOption::setQuestionId(int id)
{
    questionId = id;
}

void AnswerOption::setContent(const std::string &value)
{
    content = value;
}

void AnswerOption::setIsCorrect(bool value)
{
    isCorrect = value;
}