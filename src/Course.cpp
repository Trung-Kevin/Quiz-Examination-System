#include "Course.h"

Course::Course()
    : courseId(0), name(""), description("")
{
}

Course::Course(int courseId, const std::string& name, const std::string& description)
    : courseId(courseId), name(name), description(description)
{
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

void Course::setCourseId(int courseId)
{
    this->courseId = courseId;
}

void Course::setName(const std::string& name)
{
    this->name = name;
}

void Course::setDescription(const std::string& description)
{
    this->description = description;
}