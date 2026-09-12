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
    Topic(int id = 0,
          int courseId = 0,
          const std::string &name = "",
          const std::string &description = "");

    Topic(int id,
          const std::string &name,
          const std::string &description);

    void update();
    void deleteTopic();

    int getTopicId() const;
    int getCourseId() const;
    std::string getName() const;
    std::string getDescription() const;

    void setTopicId(int id);
    void setCourseId(int id);
    void setName(const std::string &value);
    void setDescription(const std::string &value);
};

#endif