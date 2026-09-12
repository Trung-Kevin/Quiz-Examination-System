#ifndef ANSWEROPTION_H
#define ANSWEROPTION_H

#include <string>

class AnswerOption
{
private:
    int optionId;
    int questionId;
    std::string content;
    bool isCorrect;

public:
    AnswerOption(int id = 0,
                 int questionId = 0,
                 const std::string &content = "",
                 bool correct = false);

    AnswerOption(int id,
                 const std::string &content,
                 bool correct);

    void update();

    int getOptionId() const;
    int getQuestionId() const;
    std::string getContent() const;
    bool getIsCorrect() const;

    void setOptionId(int id);
    void setQuestionId(int id);
    void setContent(const std::string &value);
    void setIsCorrect(bool value);
};

#endif