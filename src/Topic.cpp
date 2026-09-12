#include "Topic.h"

Topic::Topic(int id,
             int courseId,
             const std::string &name,
             const std::string &description)
    : topicId(id),
      courseId(courseId),
      name(name),
      description(description)
{
}

Topic::Topic(int id,
             const std::string &name,
             const std::string &description)
    : topicId(id),
      courseId(0),
      name(name),
      description(description)
{
}

void Topic::update()
{
    // Actual update is handled by QuestionBank / GUI.
}

void Topic::deleteTopic()
{
    // Actual deletion is handled by QuestionBank / GUI.
}

int Topic::getTopicId() const
{
    return topicId;
}

int Topic::getCourseId() const
{
    return courseId;
}

std::string Topic::getName() const
{
    return name;
}

std::string Topic::getDescription() const
{
    return description;
}

void Topic::setTopicId(int id)
{
    topicId = id;
}

void Topic::setCourseId(int id)
{
    courseId = id;
}

void Topic::setName(const std::string &value)
{
    name = value;
}

void Topic::setDescription(const std::string &value)
{
    description = value;
}