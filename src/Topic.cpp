#include "Topic.h"

Topic::Topic(int id,
             const std::string &name,
             const std::string &description)
    : topicId(id), name(name), description(description)
{
}

void Topic::update()
{
    // Actual saving is handled by QuestionGui data manager.
}

void Topic::deleteTopic()
{
    // Actual deletion is handled by QuestionGui data manager.
}

int Topic::getTopicId() const
{
    return topicId;
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

void Topic::setName(const std::string &value)
{
    name = value;
}

void Topic::setDescription(const std::string &value)
{
    description = value;
}