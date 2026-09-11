#include "Test.h"
#include <iostream>
#include <algorithm>

using namespace std;

Test::Test()
    : testId(0),
      title(""),
      timeLimit(0),
      startTime(""),
      endTime(""),
      questionCount(0),
      status(TestStatus::Draft)
{
}

Test::Test(int testId,
           const string& title,
           const Course& course,
           int timeLimit,
           const string& startTime,
           const string& endTime)
    : testId(testId),
      title(title),
      course(course),
      timeLimit(timeLimit),
      startTime(startTime),
      endTime(endTime),
      questionCount(0),
      status(TestStatus::Draft)
{
}

void Test::addQuestion()
{
    cout << "=== Add Question to Test ===" << endl;

    int questionId;
    int questionOrder;
    double score;

    cout << "Question ID: ";
    cin >> questionId;

    cout << "Question order: ";
    cin >> questionOrder;

    cout << "Score: ";
    cin >> score;

    TestQuestion question(testId, questionId, questionOrder, score);
    questions.push_back(question);

    questionCount = static_cast<int>(questions.size());

    cout << "Question added successfully." << endl;
}

void Test::removeQuestion()
{
    cout << "=== Remove Question from Test ===" << endl;

    int questionId;
    cout << "Question ID to remove: ";
    cin >> questionId;

    auto it = remove_if(
        questions.begin(),
        questions.end(),
        [questionId](const TestQuestion& question)
        {
            return question.getQuestionId() == questionId;
        });

    if (it == questions.end())
    {
        cout << "Question not found." << endl;
        return;
    }

    questions.erase(it, questions.end());

    // Cap nhat lai thu tu cau hoi
    for (size_t i = 0; i < questions.size(); ++i)
    {
        questions[i].setQuestionOrder(static_cast<int>(i + 1));
    }

    questionCount = static_cast<int>(questions.size());

    cout << "Question removed successfully." << endl;
}

void Test::update()
{
    cout << "=== Update Test ===" << endl;

    cin.ignore();

    cout << "New title: ";
    getline(cin, title);

    cout << "New time limit (minutes): ";
    cin >> timeLimit;

    cin.ignore();

    cout << "New start time: ";
    getline(cin, startTime);

    cout << "New end time: ";
    getline(cin, endTime);

    cout << "Test updated successfully." << endl;
}

void Test::publish()
{
    cout << "=== Publish Test ===" << endl;

    if (status == TestStatus::Published)
    {
        cout << "Test is already published." << endl;
        return;
    }

    if (status == TestStatus::Closed)
    {
        cout << "Cannot publish a closed test." << endl;
        return;
    }

    status = TestStatus::Published;

    cout << "Test published successfully." << endl;
}

void Test::deleteTest()
{
    cout << "=== Delete Test ===" << endl;

    status = TestStatus::Closed;

    cout << "Test ID " << testId
         << " has been closed/deleted." << endl;
}

int Test::getTestId() const
{
    return testId;
}

string Test::getTitle() const
{
    return title;
}

int Test::getTimeLimit() const
{
    return timeLimit;
}

string Test::getStartTime() const
{
    return startTime;
}

string Test::getEndTime() const
{
    return endTime;
}

int Test::getQuestionCount() const
{
    return questionCount;
}

TestStatus Test::getStatus() const
{
    return status;
}

const vector<TestQuestion>& Test::getQuestions() const
{
    return questions;
}
