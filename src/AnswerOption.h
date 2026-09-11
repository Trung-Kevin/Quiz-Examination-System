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
    AnswerOption();
    AnswerOption(int optionId,
                 int questionId,
                 const std::string& content,
                 bool isCorrect);

    int getOptionId() const;
    int getQuestionId() const;
    std::string getContent() const;
    bool getIsCorrect() const;

    void setOptionId(int optionId);
    void setQuestionId(int questionId);
    void setContent(const std::string& content);
    void setIsCorrect(bool isCorrect);
};

#endif