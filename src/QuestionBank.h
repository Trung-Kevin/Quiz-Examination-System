#ifndef QUESTIONBANK_H
#define QUESTIONBANK_H

#include "Course.h"
#include "Topic.h"
#include "Question.h"
#include "AnswerOption.h"

#include <string>
#include <vector>

class QuestionBank
{
private:
    std::vector<Course> courses;
    std::vector<Topic> topics;
    std::vector<Question> questions;
    std::vector<AnswerOption> answerOptions;

    int findCourseIndexById(int courseId) const;
    int findTopicIndexById(int topicId) const;
    int findQuestionIndexById(int questionId) const;
    int findAnswerOptionIndexById(int optionId) const;

public:
    // ==================== COURSE CRUD ====================

    bool addCourse(const Course& course);

    bool updateCourse(int courseId,
                      const std::string& name,
                      const std::string& description);

    bool deleteCourse(int courseId);

    const std::vector<Course>& getCourses() const;

    // ==================== TOPIC CRUD ====================

    bool addTopic(const Topic& topic);

    bool updateTopic(int topicId,
                     int courseId,
                     const std::string& name,
                     const std::string& description);

    bool deleteTopic(int topicId);

    const std::vector<Topic>& getTopics() const;

    // ==================== QUESTION CRUD ====================

    bool addQuestion(const Question& question);

    bool updateQuestion(int questionId,
                        int topicId,
                        const std::string& content,
                        DifficultyLevel difficultyLevel);

    bool deleteQuestion(int questionId);

    const std::vector<Question>& getQuestions() const;

    // ==================== ANSWER OPTION CRUD ====================

    bool addAnswerOption(const AnswerOption& answerOption);

    bool updateAnswerOption(int optionId,
                            int questionId,
                            const std::string& content,
                            bool isCorrect);

    bool deleteAnswerOption(int optionId);

    const std::vector<AnswerOption>& getAnswerOptions() const;

    // ==================== SEARCH ====================

    std::vector<Question> searchQuestions(const std::string& keyword) const;

    // ==================== FILTER ====================

    // courseId = -1    -> all courses
    // topicId = -1     -> all topics
    // difficulty = -1  -> all difficulties
    // difficulty = 0   -> Easy
    // difficulty = 1   -> Medium
    // difficulty = 2   -> Hard
    std::vector<Question> filterQuestions(int courseId,
                                          int topicId,
                                          int difficulty) const;
};

#endif