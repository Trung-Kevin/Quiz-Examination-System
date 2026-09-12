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
    AnswerOption(int id = 0,
                 const std::string &content = "",
                 bool correct = false);

    void update();

    int getOptionId() const;
    std::string getContent() const;
    bool getIsCorrect() const;

    void setOptionId(int id);
    void setContent(const std::string &value);
    void setIsCorrect(bool value);
};

#endif