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
    void update();
    void deleteTopic();
};

#endif