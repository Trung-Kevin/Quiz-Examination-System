#ifndef TOPIC_H
#define TOPIC_H

#include <string>

class Topic
{
private:
    int topicId;
    std::string name;
    std::string description;

public:
    Topic(int id = 0,
          const std::string &name = "",
          const std::string &description = "");

    void update();
    void deleteTopic();

    int getTopicId() const;
    std::string getName() const;
    std::string getDescription() const;

    void setTopicId(int id);
    void setName(const std::string &value);
    void setDescription(const std::string &value);
};

#endif