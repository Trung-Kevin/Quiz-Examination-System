#ifndef COURSE_H
#define COURSE_H

#include <string>

class Course
{
private:
    int courseId;
    std::string name;
    std::string description;

public:
    void addTopic();
    void update();
    void deleteCourse();
};

#endif