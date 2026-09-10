#ifndef TOPIC_H
#define TOPIC_H

#include <string>

using namespace std;

class Topic
{
private:
    int topicId;
    string name;
    string description;

public:
    void update();
    void deleteTopic();
};

#endif