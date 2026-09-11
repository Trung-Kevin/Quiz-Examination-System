#include "Topic.h"

Topic::Topic()
    : topicId(0), courseId(0), name(""), description("")
{
}

Topic::Topic(int topicId, int courseId, const std::string& name, const std::string& description)
    : topicId(topicId), courseId(courseId), name(name), description(description)
{
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

void Topic::setTopicId(int topicId)
{
    this->topicId = topicId;
}

void Topic::setCourseId(int courseId)
{
    this->courseId = courseId;
}

void Topic::setName(const std::string& name)
{
    this->name = name;
}

void Topic::setDescription(const std::string& description)
{
    this->description = description;
}