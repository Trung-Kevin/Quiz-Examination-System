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
    Course();
    Course(int courseId, const std::string& name, const std::string& description);

    int getCourseId() const;
    std::string getName() const;
    std::string getDescription() const;

    void setCourseId(int courseId);
    void setName(const std::string& name);
    void setDescription(const std::string& description);
};

#endif