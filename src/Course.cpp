#include "Course.h"

Course::Course(int id,
               const std::string &name,
               const std::string &description)
    : courseId(id), name(name), description(description)
{
}

void Course::addTopic()
{
    // Topic-course relationship is handled by the GUI/data layer.
}

void Course::update()
{
    // Actual saving is handled by QuestionGui data manager.
}

void Course::deleteCourse()
{
    // Actual deletion is handled by QuestionGui data manager.
}

int Course::getCourseId() const
{
    return courseId;
}

std::string Course::getName() const
{
    return name;
}

std::string Course::getDescription() const
{
    return description;
}

void Course::setCourseId(int id)
{
    courseId = id;
}

void Course::setName(const std::string &value)
{
    name = value;
}

void Course::setDescription(const std::string &value)
{
    description = value;
}