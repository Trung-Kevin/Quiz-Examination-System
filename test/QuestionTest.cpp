#include <gtest/gtest.h>
#include "../src/Question.h"

TEST(QuestionTest, CreateQuestion)
{
    Question q(
        1,
        10,
        "What is C++?",
        DifficultyLevel::Easy);

    EXPECT_EQ(q.getQuestionId(), 1);
    EXPECT_EQ(q.getTopicId(), 10);
    EXPECT_EQ(q.getContent(), "What is C++?");
    EXPECT_EQ(q.getDifficultyLevel(), DifficultyLevel::Easy);
}
TEST(QuestionTest, SetQuestionInformation)
{
    Question q;

    q.setQuestionId(5);
    q.setTopicId(20);
    q.setContent("What is OOP?");
    q.setDifficultyLevel(DifficultyLevel::Hard);

    EXPECT_EQ(q.getQuestionId(), 5);
    EXPECT_EQ(q.getTopicId(), 20);
    EXPECT_EQ(q.getContent(), "What is OOP?");
    EXPECT_EQ(q.getDifficultyLevel(), DifficultyLevel::Hard);
}