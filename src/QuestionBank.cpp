#include "QuestionBank.h"

#include <algorithm>
#include <cctype>

// ==================== FIND HELPERS ====================

int QuestionBank::findCourseIndexById(int courseId) const
{
    for (int i = 0; i < static_cast<int>(courses.size()); ++i)
    {
        if (courses[i].getCourseId() == courseId)
        {
            return i;
        }
    }

    return -1;
}

int QuestionBank::findTopicIndexById(int topicId) const
{
    for (int i = 0; i < static_cast<int>(topics.size()); ++i)
    {
        if (topics[i].getTopicId() == topicId)
        {
            return i;
        }
    }

    return -1;
}

int QuestionBank::findQuestionIndexById(int questionId) const
{
    for (int i = 0; i < static_cast<int>(questions.size()); ++i)
    {
        if (questions[i].getQuestionId() == questionId)
        {
            return i;
        }
    }

    return -1;
}

int QuestionBank::findAnswerOptionIndexById(int optionId) const
{
    for (int i = 0; i < static_cast<int>(answerOptions.size()); ++i)
    {
        if (answerOptions[i].getOptionId() == optionId)
        {
            return i;
        }
    }

    return -1;
}

// ==================== COURSE CRUD ====================

bool QuestionBank::addCourse(const Course& course)
{
    if (findCourseIndexById(course.getCourseId()) != -1)
    {
        return false;
    }

    courses.push_back(course);

    return true;
}

bool QuestionBank::updateCourse(int courseId,
                                const std::string& name,
                                const std::string& description)
{
    int index = findCourseIndexById(courseId);

    if (index == -1)
    {
        return false;
    }

    courses[index].setName(name);
    courses[index].setDescription(description);

    return true;
}

bool QuestionBank::deleteCourse(int courseId)
{
    int index = findCourseIndexById(courseId);

    if (index == -1)
    {
        return false;
    }

    // Delete all Topics belonging to this Course.
    // deleteTopic() will also delete Questions and AnswerOptions.
    for (int i = static_cast<int>(topics.size()) - 1; i >= 0; --i)
    {
        if (topics[i].getCourseId() == courseId)
        {
            int topicId = topics[i].getTopicId();
            deleteTopic(topicId);
        }
    }

    courses.erase(courses.begin() + index);

    return true;
}

const std::vector<Course>& QuestionBank::getCourses() const
{
    return courses;
}

// ==================== TOPIC CRUD ====================

bool QuestionBank::addTopic(const Topic& topic)
{
    // Topic ID must be unique
    if (findTopicIndexById(topic.getTopicId()) != -1)
    {
        return false;
    }

    // Parent Course must exist
    if (findCourseIndexById(topic.getCourseId()) == -1)
    {
        return false;
    }

    topics.push_back(topic);

    return true;
}

bool QuestionBank::updateTopic(int topicId,
                               int courseId,
                               const std::string& name,
                               const std::string& description)
{
    int index = findTopicIndexById(topicId);

    if (index == -1)
    {
        return false;
    }

    // New parent Course must exist
    if (findCourseIndexById(courseId) == -1)
    {
        return false;
    }

    topics[index].setCourseId(courseId);
    topics[index].setName(name);
    topics[index].setDescription(description);

    return true;
}

bool QuestionBank::deleteTopic(int topicId)
{
    int index = findTopicIndexById(topicId);

    if (index == -1)
    {
        return false;
    }

    // Delete Questions belonging to this Topic.
    // deleteQuestion() will also delete AnswerOptions.
    for (int i = static_cast<int>(questions.size()) - 1; i >= 0; --i)
    {
        if (questions[i].getTopicId() == topicId)
        {
            int questionId = questions[i].getQuestionId();
            deleteQuestion(questionId);
        }
    }

    topics.erase(topics.begin() + index);

    return true;
}

const std::vector<Topic>& QuestionBank::getTopics() const
{
    return topics;
}

// ==================== QUESTION CRUD ====================

bool QuestionBank::addQuestion(const Question& question)
{
    // Question ID must be unique
    if (findQuestionIndexById(question.getQuestionId()) != -1)
    {
        return false;
    }

    // Parent Topic must exist
    if (findTopicIndexById(question.getTopicId()) == -1)
    {
        return false;
    }

    questions.push_back(question);

    return true;
}

bool QuestionBank::updateQuestion(int questionId,
                                  int topicId,
                                  const std::string& content,
                                  DifficultyLevel difficultyLevel)
{
    int index = findQuestionIndexById(questionId);

    if (index == -1)
    {
        return false;
    }

    // New parent Topic must exist
    if (findTopicIndexById(topicId) == -1)
    {
        return false;
    }

    questions[index].setTopicId(topicId);
    questions[index].setContent(content);
    questions[index].setDifficultyLevel(difficultyLevel);

    return true;
}

bool QuestionBank::deleteQuestion(int questionId)
{
    int index = findQuestionIndexById(questionId);

    if (index == -1)
    {
        return false;
    }

    // Delete AnswerOptions belonging to this Question
    for (int i = static_cast<int>(answerOptions.size()) - 1; i >= 0; --i)
    {
        if (answerOptions[i].getQuestionId() == questionId)
        {
            answerOptions.erase(answerOptions.begin() + i);
        }
    }

    questions.erase(questions.begin() + index);

    return true;
}

const std::vector<Question>& QuestionBank::getQuestions() const
{
    return questions;
}

// ==================== ANSWER OPTION CRUD ====================

bool QuestionBank::addAnswerOption(const AnswerOption& answerOption)
{
    // AnswerOption ID must be unique
    if (findAnswerOptionIndexById(answerOption.getOptionId()) != -1)
    {
        return false;
    }

    // Parent Question must exist
    if (findQuestionIndexById(answerOption.getQuestionId()) == -1)
    {
        return false;
    }

    answerOptions.push_back(answerOption);

    return true;
}

bool QuestionBank::updateAnswerOption(int optionId,
                                      int questionId,
                                      const std::string& content,
                                      bool isCorrect)
{
    int index = findAnswerOptionIndexById(optionId);

    if (index == -1)
    {
        return false;
    }

    // New parent Question must exist
    if (findQuestionIndexById(questionId) == -1)
    {
        return false;
    }

    answerOptions[index].setQuestionId(questionId);
    answerOptions[index].setContent(content);
    answerOptions[index].setIsCorrect(isCorrect);

    return true;
}

bool QuestionBank::deleteAnswerOption(int optionId)
{
    int index = findAnswerOptionIndexById(optionId);

    if (index == -1)
    {
        return false;
    }

    answerOptions.erase(answerOptions.begin() + index);

    return true;
}

const std::vector<AnswerOption>& QuestionBank::getAnswerOptions() const
{
    return answerOptions;
}

// ==================== SEARCH ====================

std::vector<Question> QuestionBank::searchQuestions(const std::string& keyword) const
{
    std::vector<Question> result;

    std::string lowerKeyword = keyword;

    std::transform(lowerKeyword.begin(),
                   lowerKeyword.end(),
                   lowerKeyword.begin(),
                   [](unsigned char c)
                   {
                       return static_cast<char>(std::tolower(c));
                   });

    for (const Question& question : questions)
    {
        std::string lowerContent = question.getContent();

        std::transform(lowerContent.begin(),
                       lowerContent.end(),
                       lowerContent.begin(),
                       [](unsigned char c)
                       {
                           return static_cast<char>(std::tolower(c));
                       });

        if (lowerContent.find(lowerKeyword) != std::string::npos)
        {
            result.push_back(question);
        }
    }

    return result;
}

// ==================== FILTER ====================

std::vector<Question> QuestionBank::filterQuestions(int courseId,
                                                    int topicId,
                                                    int difficulty) const
{
    std::vector<Question> result;

    for (const Question& question : questions)
    {
        // Filter by Topic
        if (topicId != -1 && question.getTopicId() != topicId)
        {
            continue;
        }

        // Filter by Course
        if (courseId != -1)
        {
            int topicIndex = findTopicIndexById(question.getTopicId());

            if (topicIndex == -1)
            {
                continue;
            }

            if (topics[topicIndex].getCourseId() != courseId)
            {
                continue;
            }
        }

        // Filter by Difficulty
        if (difficulty != -1)
        {
            if (static_cast<int>(question.getDifficultyLevel()) != difficulty)
            {
                continue;
            }
        }

        result.push_back(question);
    }

    return result;
}