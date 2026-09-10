#ifndef ANSWEROPTION_H
#define ANSWEROPTION_H

#include <string>

class AnswerOption
{
private:
    int optionId;
    std::string content;
    bool isCorrect;

public:
    void update();
};

#endif