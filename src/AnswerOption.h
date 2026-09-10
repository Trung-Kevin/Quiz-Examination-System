#ifndef ANSWEROPTION_H
#define ANSWEROPTION_H

#include <string>

using namespace std;

class AnswerOption
{
private:
    int optionId;
    string content;
    bool isCorrect;

public:
    void update();
};

#endif