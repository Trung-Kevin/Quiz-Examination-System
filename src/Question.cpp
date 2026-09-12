#include "Question.h"

#include <fstream>
#include <sstream>

namespace
{
    DifficultyLevel stringToDifficulty(const std::string &value)
    {
        if (value == "Medium")
            return DifficultyLevel::Medium;

        if (value == "Hard")
            return DifficultyLevel::Hard;

        return DifficultyLevel::Easy;
    }
}

Question::Question(int id,
                   const std::string &content,
                   DifficultyLevel difficulty)
    : questionId(id),
      content(content),
      difficultyLevel(difficulty)
{
}

void Question::update()
{
    // Actual update is handled by QuestionGui.
}

void Question::deleteQuestion()
{
    // Actual deletion is handled by QuestionGui.
}

std::vector<Question> Question::search()
{
    std::vector<Question> result;

    std::ifstream file("data/questions.txt");

    if (!file.is_open())
        return result;

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);

        std::string idText;
        std::string topicIdText;
        std::string difficultyText;
        std::string questionText;

        std::getline(ss, idText, '|');
        std::getline(ss, topicIdText, '|');
        std::getline(ss, difficultyText, '|');
        std::getline(ss, questionText);

        try
        {
            int id = std::stoi(idText);

            result.emplace_back(
                id,
                questionText,
                stringToDifficulty(difficultyText));
        }
        catch (...)
        {
        }
    }

    return result;
}

int Question::getQuestionId() const
{
    return questionId;
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

void Question::setContent(const std::string &value)
{
    content = value;
}

void Question::setDifficultyLevel(DifficultyLevel value)
{
    difficultyLevel = value;
}