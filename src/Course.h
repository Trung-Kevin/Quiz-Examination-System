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
    // Constructor mặc định
    Course();

    // Constructor đầy đủ
    Course(int id,
           const std::string &name,
           const std::string &description);

    void addTopic();
    void update();
    void deleteCourse();

    // Getters
    int getCourseId() const;
    std::string getName() const;
    std::string getDescription() const;

    // Setters
    void setCourseId(int id);
    void setName(const std::string &value);
    void setDescription(const std::string &value);
};

#endif