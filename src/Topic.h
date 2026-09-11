#ifndef TOPIC_H
#define TOPIC_H

#include <string>

class Topic
{
private:
    int topicId;
    int courseId;
    std::string name;
    std::string description;

public:
    Topic();
    Topic(int topicId, int courseId, const std::string& name, const std::string& description);

    int getTopicId() const;
    int getCourseId() const;
    std::string getName() const;
    std::string getDescription() const;

    void setTopicId(int topicId);
    void setCourseId(int courseId);
    void setName(const std::string& name);
    void setDescription(const std::string& description);
};

#endif