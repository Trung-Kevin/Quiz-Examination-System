#ifndef COURSE_H
#define COURSE_H

#include <string>

using namespace std;

class Course
{
private:
    int courseId;
    string name;
    string description;

public:
    void addTopic();
    void update();
    void deleteCourse();
};

#endif