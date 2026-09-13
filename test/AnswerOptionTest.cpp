#include <gtest/gtest.h>
#include "../src/AnswerOption.h"

TEST(AnswerOptionTest, CreateAnswerOption)
{
    AnswerOption option(
        1,
        10,
        "Paris",
        true
    );

    EXPECT_EQ(option.getOptionId(), 1);
    EXPECT_EQ(option.getQuestionId(), 10);
    EXPECT_EQ(option.getContent(), "Paris");
    EXPECT_TRUE(option.getIsCorrect());
}

TEST(AnswerOptionTest, SetAnswerOptionInformation)
{
    AnswerOption option;

    option.setOptionId(5);
    option.setQuestionId(20);
    option.setContent("London");
    option.setIsCorrect(false);

    EXPECT_EQ(option.getOptionId(), 5);
    EXPECT_EQ(option.getQuestionId(), 20);
    EXPECT_EQ(option.getContent(), "London");
    EXPECT_FALSE(option.getIsCorrect());
}


