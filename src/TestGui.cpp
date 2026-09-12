#include "TestGui.h"
#include "Test.h"
#include "TestQuestion.h"

#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

// ============================================================
// DATA
// ============================================================

static std::vector<Test> testList;
static std::vector<TestQuestion> testQuestionList;

static int nextTestId = 1;

static void selectTest();

// ============================================================
// CONTROL HANDLES
// ============================================================

// Test information
static HWND hTitle = NULL;
static HWND hCourse = NULL;
static HWND hTimeLimit = NULL;
static HWND hStartTime = NULL;
static HWND hEndTime = NULL;
static HWND hQuestionCount = NULL;

// Question information
static HWND hQuestionId = NULL;
static HWND hQuestionOrder = NULL;
static HWND hQuestionScore = NULL;

// List boxes
static HWND hTestList = NULL;
static HWND hQuestionList = NULL;

// Section labels
static HWND lblTestInformation = NULL;
static HWND lblQuestionInformation = NULL;

// Test labels
static HWND lblTestTitle = NULL;
static HWND lblCourse = NULL;
static HWND lblTimeLimit = NULL;
static HWND lblStartTime = NULL;
static HWND lblEndTime = NULL;
static HWND lblQuestionCount = NULL;

// Question labels
static HWND lblQuestionId = NULL;
static HWND lblQuestionOrder = NULL;
static HWND lblQuestionScore = NULL;

// List labels
static HWND lblTestList = NULL;
static HWND lblQuestionList = NULL;

// ============================================================
// GET TEXT
// ============================================================

static std::string getText(HWND hwnd)
{
    if (hwnd == NULL)
        return "";

    int length = GetWindowTextLengthA(hwnd);

    if (length <= 0)
        return "";

    std::string text(length + 1, '\0');

    GetWindowTextA(
        hwnd,
        &text[0],
        length + 1);

    text.resize(length);

    return text;
}

// ============================================================
// SET TEXT
// ============================================================

static void setText(HWND hwnd, const std::string &text)
{
    if (hwnd == NULL)
        return;

    SetWindowTextA(
        hwnd,
        text.c_str());
}

// ============================================================
// TEST STATUS TO STRING
// ============================================================

static std::string statusToString(TestStatus status)
{
    switch (status)
    {
    case TestStatus::Draft:
        return "Draft";

    case TestStatus::Published:
        return "Published";

    case TestStatus::Closed:
        return "Closed";
    }

    return "Draft";
}

// ============================================================
// MOVE CONTROL
// ============================================================

static void moveControl(
    HWND hwnd,
    int x,
    int y,
    int width,
    int height)
{
    if (hwnd == NULL)
        return;

    if (width < 1)
        width = 1;

    if (height < 1)
        height = 1;

    MoveWindow(
        hwnd,
        x,
        y,
        width,
        height,
        TRUE);
}

// ============================================================
// REFRESH TEST LIST
// ============================================================

static void refreshTestList()
{
    if (hTestList == NULL)
        return;

    SendMessageA(
        hTestList,
        LB_RESETCONTENT,
        0,
        0);

    for (const Test &test : testList)
    {
        std::ostringstream text;

        text
            << "ID: "
            << test.getTestId()
            << " | "
            << test.getTitle()
            << " | Course: "
            << test.getCourse().getName()
            << " | Time: "
            << test.getTimeLimit()
            << " min"
            << " | Questions: "
            << test.getQuestionCount()
            << " | Status: "
            << statusToString(test.getStatus());

        std::string line = text.str();

        SendMessageA(
            hTestList,
            LB_ADDSTRING,
            0,
            (LPARAM)line.c_str());
    }
}

// ============================================================
// REFRESH QUESTION LIST
// ============================================================

static void refreshQuestionList()
{
    if (hQuestionList == NULL)
        return;

    SendMessageA(
        hQuestionList,
        LB_RESETCONTENT,
        0,
        0);

    if (hTestList == NULL)
        return;

    int selected =
        (int)SendMessageA(
            hTestList,
            LB_GETCURSEL,
            0,
            0);

    if (selected == LB_ERR)
        return;

    if (selected < 0 ||
        selected >= (int)testList.size())
    {
        return;
    }

    int testId =
        testList[selected].getTestId();

    std::vector<TestQuestion> questions;

    for (const TestQuestion &tq : testQuestionList)
    {
        if (tq.getTestId() == testId)
        {
            questions.push_back(tq);
        }
    }

    std::sort(
        questions.begin(),
        questions.end(),
        [](const TestQuestion &a,
           const TestQuestion &b)
        {
            return a.getQuestionOrder() <
                   b.getQuestionOrder();
        });

    for (const TestQuestion &tq : questions)
    {
        std::ostringstream text;

        text
            << "Question ID: "
            << tq.getQuestionId()
            << " | Order: "
            << tq.getQuestionOrder()
            << " | Score: "
            << tq.getScore();

        std::string line = text.str();

        SendMessageA(
            hQuestionList,
            LB_ADDSTRING,
            0,
            (LPARAM)line.c_str());
    }
}

// ============================================================
// CLEAR FORM
// ============================================================

static void clearForm()
{
    setText(hTitle, "");
    setText(hCourse, "");
    setText(hTimeLimit, "");
    setText(hStartTime, "");
    setText(hEndTime, "");
    setText(hQuestionCount, "0");

    setText(hQuestionId, "");
    setText(hQuestionOrder, "");
    setText(hQuestionScore, "");

    if (hTestList != NULL)
    {
        SendMessageA(
            hTestList,
            LB_SETCURSEL,
            (WPARAM)-1,
            0);
    }

    if (hQuestionList != NULL)
    {
        SendMessageA(
            hQuestionList,
            LB_RESETCONTENT,
            0,
            0);
    }
}

// ============================================================
// ADD TEST
// ============================================================

static void addTest()
{
    std::string title =
        getText(hTitle);

    std::string courseName =
        getText(hCourse);

    std::string timeText =
        getText(hTimeLimit);

    std::string startTime =
        getText(hStartTime);

    std::string endTime =
        getText(hEndTime);

    if (title.empty())
    {
        MessageBoxA(
            NULL,
            "Test title cannot be empty.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (courseName.empty())
    {
        MessageBoxA(
            NULL,
            "Course cannot be empty.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (timeText.empty())
    {
        MessageBoxA(
            NULL,
            "Time limit cannot be empty.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    int timeLimit = 0;

    try
    {
        timeLimit = std::stoi(timeText);
    }
    catch (...)
    {
        MessageBoxA(
            NULL,
            "Time limit must be a number.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (timeLimit <= 0)
    {
        MessageBoxA(
            NULL,
            "Time limit must be greater than 0.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    Course course(
        0,
        courseName,
        "");

    Test newTest(
        nextTestId,
        title,
        course,
        timeLimit,
        startTime,
        endTime,
        0,
        TestStatus::Draft);

    testList.push_back(newTest);

    nextTestId++;

    refreshTestList();
    clearForm();

    MessageBoxA(
        NULL,
        "Test created successfully.",
        "Success",
        MB_OK | MB_ICONINFORMATION);
}

// ============================================================
// UPDATE TEST
// ============================================================

static void updateTest()
{
    int selected =
        (int)SendMessageA(
            hTestList,
            LB_GETCURSEL,
            0,
            0);

    if (selected == LB_ERR)
    {
        MessageBoxA(
            NULL,
            "Please select a test first.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (selected < 0 ||
        selected >= (int)testList.size())
    {
        return;
    }

    std::string title =
        getText(hTitle);

    std::string courseName =
        getText(hCourse);

    std::string timeText =
        getText(hTimeLimit);

    std::string startTime =
        getText(hStartTime);

    std::string endTime =
        getText(hEndTime);

    if (title.empty() ||
        courseName.empty() ||
        timeText.empty())
    {
        MessageBoxA(
            NULL,
            "Please fill in all required test information.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    int timeLimit = 0;

    try
    {
        timeLimit = std::stoi(timeText);
    }
    catch (...)
    {
        MessageBoxA(
            NULL,
            "Time limit must be a number.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (timeLimit <= 0)
    {
        MessageBoxA(
            NULL,
            "Time limit must be greater than 0.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    Test &test =
        testList[selected];

    Course course(
        test.getCourse().getCourseId(),
        courseName,
        test.getCourse().getDescription());

    test.setTitle(title);
    test.setCourse(course);
    test.setTimeLimit(timeLimit);
    test.setStartTime(startTime);
    test.setEndTime(endTime);

    test.update();

    refreshTestList();

    SendMessageA(
        hTestList,
        LB_SETCURSEL,
        (WPARAM)selected,
        0);

    MessageBoxA(
        NULL,
        "Test updated successfully.",
        "Success",
        MB_OK | MB_ICONINFORMATION);
}

// ============================================================
// DELETE TEST
// ============================================================

static void deleteTest()
{
    int selected =
        (int)SendMessageA(
            hTestList,
            LB_GETCURSEL,
            0,
            0);

    if (selected == LB_ERR)
    {
        MessageBoxA(
            NULL,
            "Please select a test first.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (selected < 0 ||
        selected >= (int)testList.size())
    {
        return;
    }

    int testId =
        testList[selected].getTestId();

    int answer =
        MessageBoxA(
            NULL,
            "Are you sure you want to delete this test?",
            "Confirm",
            MB_YESNO | MB_ICONQUESTION);

    if (answer != IDYES)
        return;

    testList[selected].deleteTest();

    testList.erase(
        testList.begin() + selected);

    testQuestionList.erase(
        std::remove_if(
            testQuestionList.begin(),
            testQuestionList.end(),
            [testId](const TestQuestion &tq)
            {
                return tq.getTestId() == testId;
            }),
        testQuestionList.end());

    refreshTestList();

    if (hQuestionList != NULL)
    {
        SendMessageA(
            hQuestionList,
            LB_RESETCONTENT,
            0,
            0);
    }

    clearForm();

    MessageBoxA(
        NULL,
        "Test deleted successfully.",
        "Success",
        MB_OK | MB_ICONINFORMATION);
}

// ============================================================
// PUBLISH TEST
// ============================================================

static void publishTest()
{
    int selected =
        (int)SendMessageA(
            hTestList,
            LB_GETCURSEL,
            0,
            0);

    if (selected == LB_ERR)
    {
        MessageBoxA(
            NULL,
            "Please select a test first.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (selected < 0 ||
        selected >= (int)testList.size())
    {
        return;
    }

    if (testList[selected].getQuestionCount() <= 0)
    {
        MessageBoxA(
            NULL,
            "A test must contain at least one question before publishing.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (testList[selected].getStatus() ==
        TestStatus::Published)
    {
        MessageBoxA(
            NULL,
            "This test is already published.",
            "Information",
            MB_OK | MB_ICONINFORMATION);

        return;
    }

    if (testList[selected].getStatus() ==
        TestStatus::Closed)
    {
        MessageBoxA(
            NULL,
            "A closed test cannot be published.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    testList[selected].publish();

    refreshTestList();

    SendMessageA(
        hTestList,
        LB_SETCURSEL,
        (WPARAM)selected,
        0);

    selectTest();

    MessageBoxA(
        NULL,
        "Test published successfully.",
        "Success",
        MB_OK | MB_ICONINFORMATION);
}

// ============================================================
// UNPUBLISH TEST
// ============================================================

static void unpublishTest()
{
    int selected =
        (int)SendMessageA(
            hTestList,
            LB_GETCURSEL,
            0,
            0);

    if (selected == LB_ERR)
    {
        MessageBoxA(
            NULL,
            "Please select a test first.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (selected < 0 ||
        selected >= (int)testList.size())
    {
        return;
    }

    Test &test =
        testList[selected];

    if (test.getStatus() ==
        TestStatus::Draft)
    {
        MessageBoxA(
            NULL,
            "This test is already in Draft status.",
            "Information",
            MB_OK | MB_ICONINFORMATION);

        return;
    }

    if (test.getStatus() ==
        TestStatus::Closed)
    {
        MessageBoxA(
            NULL,
            "A closed test cannot be unpublished.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    int answer =
        MessageBoxA(
            NULL,
            "Are you sure you want to unpublish this test?",
            "Confirm",
            MB_YESNO | MB_ICONQUESTION);

    if (answer != IDYES)
        return;

    test.unpublish();

    refreshTestList();

    SendMessageA(
        hTestList,
        LB_SETCURSEL,
        (WPARAM)selected,
        0);

    selectTest();

    MessageBoxA(
        NULL,
        "Test unpublished successfully. The test is now in Draft status.",
        "Success",
        MB_OK | MB_ICONINFORMATION);
}

// ============================================================
// ADD QUESTION TO TEST
// ============================================================

static void addQuestionToTest()
{
    int selectedTest =
        (int)SendMessageA(
            hTestList,
            LB_GETCURSEL,
            0,
            0);

    if (selectedTest == LB_ERR)
    {
        MessageBoxA(
            NULL,
            "Please select a test first.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (selectedTest < 0 ||
        selectedTest >= (int)testList.size())
    {
        return;
    }

    Test &test =
        testList[selectedTest];

    if (test.getStatus() ==
        TestStatus::Published)
    {
        MessageBoxA(
            NULL,
            "Cannot add questions to a published test.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (test.getStatus() ==
        TestStatus::Closed)
    {
        MessageBoxA(
            NULL,
            "Cannot add questions to a closed test.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    std::string questionIdText =
        getText(hQuestionId);

    std::string orderText =
        getText(hQuestionOrder);

    std::string scoreText =
        getText(hQuestionScore);

    if (questionIdText.empty() ||
        orderText.empty() ||
        scoreText.empty())
    {
        MessageBoxA(
            NULL,
            "Please enter Question ID, Order and Score.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    int questionId = 0;
    int questionOrder = 0;
    double score = 0.0;

    try
    {
        questionId =
            std::stoi(questionIdText);

        questionOrder =
            std::stoi(orderText);

        score =
            std::stod(scoreText);
    }
    catch (...)
    {
        MessageBoxA(
            NULL,
            "Question ID, Order and Score must be valid numbers.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (questionId <= 0)
    {
        MessageBoxA(
            NULL,
            "Question ID must be greater than 0.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (questionOrder <= 0)
    {
        MessageBoxA(
            NULL,
            "Question Order must be greater than 0.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (score <= 0)
    {
        MessageBoxA(
            NULL,
            "Score must be greater than 0.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    int testId =
        test.getTestId();

    for (const TestQuestion &tq :
         testQuestionList)
    {
        if (tq.getTestId() == testId &&
            tq.getQuestionId() == questionId)
        {
            MessageBoxA(
                NULL,
                "This question already exists in the selected test.",
                "Error",
                MB_OK | MB_ICONERROR);

            return;
        }
    }

    for (const TestQuestion &tq :
         testQuestionList)
    {
        if (tq.getTestId() == testId &&
            tq.getQuestionOrder() == questionOrder)
        {
            MessageBoxA(
                NULL,
                "This question order already exists in the selected test.",
                "Error",
                MB_OK | MB_ICONERROR);

            return;
        }
    }

    TestQuestion newQuestion(
        testId,
        questionId,
        questionOrder,
        score);

    testQuestionList.push_back(
        newQuestion);

    test.setQuestionCount(
        test.getQuestionCount() + 1);

    refreshTestList();
    refreshQuestionList();

    setText(hQuestionId, "");
    setText(hQuestionOrder, "");
    setText(hQuestionScore, "");

    MessageBoxA(
        NULL,
        "Question added to test successfully.",
        "Success",
        MB_OK | MB_ICONINFORMATION);
}

// ============================================================
// REMOVE QUESTION
// ============================================================

static void removeQuestionFromTest()
{
    int selectedTest =
        (int)SendMessageA(
            hTestList,
            LB_GETCURSEL,
            0,
            0);

    if (selectedTest == LB_ERR)
    {
        MessageBoxA(
            NULL,
            "Please select a test first.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (selectedTest < 0 ||
        selectedTest >= (int)testList.size())
    {
        return;
    }

    Test &test =
        testList[selectedTest];

    if (test.getStatus() ==
        TestStatus::Published)
    {
        MessageBoxA(
            NULL,
            "Cannot remove questions from a published test.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    if (test.getStatus() ==
        TestStatus::Closed)
    {
        MessageBoxA(
            NULL,
            "Cannot remove questions from a closed test.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    int selectedQuestion =
        (int)SendMessageA(
            hQuestionList,
            LB_GETCURSEL,
            0,
            0);

    if (selectedQuestion == LB_ERR)
    {
        MessageBoxA(
            NULL,
            "Please select a question first.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    int testId =
        test.getTestId();

    std::vector<int> indexes;

    for (int i = 0;
         i < (int)testQuestionList.size();
         i++)
    {
        if (testQuestionList[i].getTestId() ==
            testId)
        {
            indexes.push_back(i);
        }
    }

    if (selectedQuestion < 0 ||
        selectedQuestion >= (int)indexes.size())
    {
        return;
    }

    int realIndex =
        indexes[selectedQuestion];

    testQuestionList.erase(
        testQuestionList.begin() + realIndex);

    if (test.getQuestionCount() > 0)
    {
        test.removeQuestion();
    }

    refreshTestList();
    refreshQuestionList();

    MessageBoxA(
        NULL,
        "Question removed from test.",
        "Success",
        MB_OK | MB_ICONINFORMATION);
}

// ============================================================
// SELECT TEST
// ============================================================

static void selectTest()
{
    int selected =
        (int)SendMessageA(
            hTestList,
            LB_GETCURSEL,
            0,
            0);

    if (selected == LB_ERR)
        return;

    if (selected < 0 ||
        selected >= (int)testList.size())
    {
        return;
    }

    Test &test =
        testList[selected];

    setText(
        hTitle,
        test.getTitle());

    setText(
        hCourse,
        test.getCourse().getName());

    setText(
        hTimeLimit,
        std::to_string(
            test.getTimeLimit()));

    setText(
        hStartTime,
        test.getStartTime());

    setText(
        hEndTime,
        test.getEndTime());

    setText(
        hQuestionCount,
        std::to_string(
            test.getQuestionCount()));

    setText(hQuestionId, "");
    setText(hQuestionOrder, "");
    setText(hQuestionScore, "");

    refreshQuestionList();
}

// ============================================================
// RESPONSIVE LAYOUT
// ============================================================

static void resizeTestGui(
    HWND hwnd,
    int width,
    int height)
{
    if (width <= 0 || height <= 0)
        return;

    const int margin = 20;
    const int gap = 15;

    // ========================================================
    // COLUMN LAYOUT
    // ========================================================

    int contentWidth =
        width - margin * 2;

    if (contentWidth < 600)
        contentWidth = 600;

    int columnWidth =
        (contentWidth - gap) / 2;

    int leftX = margin;

    int rightX =
        leftX + columnWidth + gap;

    // ========================================================
    // TOP SECTION TITLES
    // ========================================================

    moveControl(
        lblTestInformation,
        leftX,
        15,
        columnWidth,
        35);

    moveControl(
        lblQuestionInformation,
        rightX,
        15,
        columnWidth,
        35);

    // ========================================================
    // TEST INFORMATION
    // ========================================================

    const int labelWidth = 170;

    int inputX =
        leftX + labelWidth + 5;

    int inputWidth =
        columnWidth - labelWidth - 5;

    if (inputWidth < 150)
        inputWidth = 150;

    const int rowHeight = 32;
    const int rowGap = 11;

    int y = 65;

    moveControl(
        lblTestTitle,
        leftX,
        y,
        labelWidth,
        rowHeight);

    moveControl(
        hTitle,
        inputX,
        y,
        inputWidth,
        rowHeight);

    y += rowHeight + rowGap;

    moveControl(
        lblCourse,
        leftX,
        y,
        labelWidth,
        rowHeight);

    moveControl(
        hCourse,
        inputX,
        y,
        inputWidth,
        rowHeight);

    y += rowHeight + rowGap;

    moveControl(
        lblTimeLimit,
        leftX,
        y,
        labelWidth,
        rowHeight);

    moveControl(
        hTimeLimit,
        inputX,
        y,
        inputWidth,
        rowHeight);

    y += rowHeight + rowGap;

    moveControl(
        lblStartTime,
        leftX,
        y,
        labelWidth,
        rowHeight);

    moveControl(
        hStartTime,
        inputX,
        y,
        inputWidth,
        rowHeight);

    y += rowHeight + rowGap;

    moveControl(
        lblEndTime,
        leftX,
        y,
        labelWidth,
        rowHeight);

    moveControl(
        hEndTime,
        inputX,
        y,
        inputWidth,
        rowHeight);

    y += rowHeight + rowGap;

    moveControl(
        lblQuestionCount,
        leftX,
        y,
        labelWidth,
        rowHeight);

    moveControl(
        hQuestionCount,
        inputX,
        y,
        inputWidth,
        rowHeight);

    // ========================================================
    // QUESTION INFORMATION
    // ========================================================

    const int qLabelWidth = 170;

    int qInputX =
        rightX + qLabelWidth + 5;

    int qInputWidth =
        columnWidth - qLabelWidth - 5;

    if (qInputWidth < 150)
        qInputWidth = 150;

    y = 65;

    moveControl(
        lblQuestionId,
        rightX,
        y,
        qLabelWidth,
        rowHeight);

    moveControl(
        hQuestionId,
        qInputX,
        y,
        qInputWidth,
        rowHeight);

    y += rowHeight + rowGap;

    moveControl(
        lblQuestionOrder,
        rightX,
        y,
        qLabelWidth,
        rowHeight);

    moveControl(
        hQuestionOrder,
        qInputX,
        y,
        qInputWidth,
        rowHeight);

    y += rowHeight + rowGap;

    moveControl(
        lblQuestionScore,
        rightX,
        y,
        qLabelWidth,
        rowHeight);

    moveControl(
        hQuestionScore,
        qInputX,
        y,
        qInputWidth,
        rowHeight);

    // ========================================================
    // QUESTION BUTTONS
    // ========================================================

    int questionButtonY = 205;

    int questionButtonGap = 15;

    int questionButtonWidth =
        (columnWidth - questionButtonGap) / 2;

    moveControl(
        GetDlgItem(hwnd, 106),
        rightX,
        questionButtonY,
        questionButtonWidth,
        42);

    moveControl(
        GetDlgItem(hwnd, 107),
        rightX +
            questionButtonWidth +
            questionButtonGap,
        questionButtonY,
        questionButtonWidth,
        42);

    // ========================================================
    // MAIN BUTTONS
    // ========================================================

    int mainButtonY = 315;
    int mainButtonHeight = 42;

    int mainButtonGap = 10;

    // 6 buttons:
    // Add | Update | Delete | Publish | Unpublish | Clear

    int mainButtonWidth =
        (contentWidth -
         mainButtonGap * 5) /
        6;

    if (mainButtonWidth < 80)
        mainButtonWidth = 80;

    int buttonX = margin;

    // Add Test - 101
    moveControl(
        GetDlgItem(hwnd, 101),
        buttonX,
        mainButtonY,
        mainButtonWidth,
        mainButtonHeight);

    buttonX +=
        mainButtonWidth +
        mainButtonGap;

    // Update Test - 102
    moveControl(
        GetDlgItem(hwnd, 102),
        buttonX,
        mainButtonY,
        mainButtonWidth,
        mainButtonHeight);

    buttonX +=
        mainButtonWidth +
        mainButtonGap;

    // Delete Test - 103
    moveControl(
        GetDlgItem(hwnd, 103),
        buttonX,
        mainButtonY,
        mainButtonWidth,
        mainButtonHeight);

    buttonX +=
        mainButtonWidth +
        mainButtonGap;

    // Publish - 104
    moveControl(
        GetDlgItem(hwnd, 104),
        buttonX,
        mainButtonY,
        mainButtonWidth,
        mainButtonHeight);

    buttonX +=
        mainButtonWidth +
        mainButtonGap;

    // Unpublish - 109
    moveControl(
        GetDlgItem(hwnd, 109),
        buttonX,
        mainButtonY,
        mainButtonWidth,
        mainButtonHeight);

    buttonX +=
        mainButtonWidth +
        mainButtonGap;

    // Clear - 105
    moveControl(
        GetDlgItem(hwnd, 105),
        buttonX,
        mainButtonY,
        mainButtonWidth,
        mainButtonHeight);

    // ========================================================
    // TEST LIST AREA
    // ========================================================

    int testLabelY =
        mainButtonY +
        mainButtonHeight +
        15;

    int testListY =
        testLabelY +
        35;

    // ========================================================
    // CALCULATE AVAILABLE SPACE
    // ========================================================

    int bottomMargin = 15;

    int totalListArea =
        height -
        testListY -
        bottomMargin;

    if (totalListArea < 160)
        totalListArea = 160;

    // ========================================================
    // SPLIT SPACE BETWEEN TWO LISTS
    // ========================================================

    int testListHeight =
        (totalListArea * 42) / 100;

    if (testListHeight < 75)
        testListHeight = 75;

    int questionLabelY =
        testListY +
        testListHeight +
        12;

    int questionListY =
        questionLabelY +
        35;

    int questionListHeight =
        height -
        questionListY -
        bottomMargin;

    if (questionListHeight < 70)
    {
        int difference =
            70 - questionListHeight;

        testListHeight -= difference;

        if (testListHeight < 70)
            testListHeight = 70;

        questionLabelY =
            testListY +
            testListHeight +
            12;

        questionListY =
            questionLabelY +
            35;

        questionListHeight =
            height -
            questionListY -
            bottomMargin;
    }

    if (questionListHeight < 50)
        questionListHeight = 50;

    // ========================================================
    // TEST LIST LABEL
    // ========================================================

    moveControl(
        lblTestList,
        margin,
        testLabelY,
        contentWidth,
        30);

    // ========================================================
    // TEST LIST
    // ========================================================

    moveControl(
        hTestList,
        margin,
        testListY,
        contentWidth,
        testListHeight);

    // ========================================================
    // QUESTION LIST LABEL
    // ========================================================

    moveControl(
        lblQuestionList,
        margin,
        questionLabelY,
        contentWidth,
        30);

    // ========================================================
    // QUESTION LIST
    // ========================================================

    moveControl(
        hQuestionList,
        margin,
        questionListY,
        contentWidth,
        questionListHeight);
}

// ============================================================
// WINDOW PROCEDURE
// ============================================================

static LRESULT CALLBACK TestGuiProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE:
    {
        int width =
            LOWORD(lParam);

        int height =
            HIWORD(lParam);

        resizeTestGui(
            hwnd,
            width,
            height);

        return 0;
    }

    case WM_COMMAND:
    {
        int id =
            LOWORD(wParam);

        if (id == 101)
        {
            addTest();
        }
        else if (id == 102)
        {
            updateTest();
        }
        else if (id == 103)
        {
            deleteTest();
        }
        else if (id == 104)
        {
            publishTest();
        }
        else if (id == 109)
        {
            unpublishTest();
        }
        else if (id == 105)
        {
            clearForm();
        }
        else if (id == 106)
        {
            addQuestionToTest();
        }
        else if (id == 107)
        {
            removeQuestionFromTest();
        }
        else if (id == 108 &&
                 HIWORD(wParam) ==
                     LBN_SELCHANGE)
        {
            selectTest();
        }

        return 0;
    }

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    }

    return DefWindowProcA(
        hwnd,
        msg,
        wParam,
        lParam);
}

// ============================================================
// OPEN TEST GUI
// ============================================================

void openTestGui(HWND parent)
{
    const char CLASS_NAME[] =
        "QuizTestManagementGui";

    static bool registered = false;

    if (!registered)
    {
        WNDCLASSA wc = {};

        wc.lpfnWndProc =
            TestGuiProc;

        wc.hInstance =
            GetModuleHandleA(NULL);

        wc.lpszClassName =
            CLASS_NAME;

        wc.hbrBackground =
            (HBRUSH)(COLOR_WINDOW + 1);

        wc.hCursor =
            LoadCursor(
                NULL,
                IDC_ARROW);

        RegisterClassA(&wc);

        registered = true;
    }

    // ========================================================
    // CREATE WINDOW
    // ========================================================

    HWND hwnd =
        CreateWindowExA(
            0,
            CLASS_NAME,
            "Test Management",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            1100,
            850,
            parent,
            NULL,
            GetModuleHandleA(NULL),
            NULL);

    if (!hwnd)
        return;

    // ========================================================
    // TEST INFORMATION TITLE
    // ========================================================

    lblTestInformation =
        CreateWindowA(
            "STATIC",
            "TEST INFORMATION",
            WS_VISIBLE | WS_CHILD,
            20,
            15,
            500,
            35,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // QUESTION INFORMATION TITLE
    // ========================================================

    lblQuestionInformation =
        CreateWindowA(
            "STATIC",
            "QUESTION INFORMATION",
            WS_VISIBLE | WS_CHILD,
            550,
            15,
            500,
            35,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // TEST TITLE
    // ========================================================

    lblTestTitle =
        CreateWindowA(
            "STATIC",
            "Test Title:",
            WS_VISIBLE | WS_CHILD,
            20,
            65,
            170,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    hTitle =
        CreateWindowA(
            "EDIT",
            "",
            WS_VISIBLE |
                WS_CHILD |
                WS_BORDER,
            195,
            65,
            350,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // COURSE
    // ========================================================

    lblCourse =
        CreateWindowA(
            "STATIC",
            "Course:",
            WS_VISIBLE | WS_CHILD,
            20,
            108,
            170,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    hCourse =
        CreateWindowA(
            "EDIT",
            "",
            WS_VISIBLE |
                WS_CHILD |
                WS_BORDER,
            195,
            108,
            350,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // TIME LIMIT
    // ========================================================

    lblTimeLimit =
        CreateWindowA(
            "STATIC",
            "Time Limit (min):",
            WS_VISIBLE | WS_CHILD,
            20,
            151,
            170,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    hTimeLimit =
        CreateWindowA(
            "EDIT",
            "",
            WS_VISIBLE |
                WS_CHILD |
                WS_BORDER,
            195,
            151,
            350,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // START TIME
    // ========================================================

    lblStartTime =
        CreateWindowA(
            "STATIC",
            "Start Time:",
            WS_VISIBLE | WS_CHILD,
            20,
            194,
            170,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    hStartTime =
        CreateWindowA(
            "EDIT",
            "",
            WS_VISIBLE |
                WS_CHILD |
                WS_BORDER,
            195,
            194,
            350,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // END TIME
    // ========================================================

    lblEndTime =
        CreateWindowA(
            "STATIC",
            "End Time:",
            WS_VISIBLE | WS_CHILD,
            20,
            237,
            170,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    hEndTime =
        CreateWindowA(
            "EDIT",
            "",
            WS_VISIBLE |
                WS_CHILD |
                WS_BORDER,
            195,
            237,
            350,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // QUESTION COUNT
    // ========================================================

    lblQuestionCount =
        CreateWindowA(
            "STATIC",
            "Question Count:",
            WS_VISIBLE | WS_CHILD,
            20,
            280,
            170,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    hQuestionCount =
        CreateWindowA(
            "EDIT",
            "0",
            WS_VISIBLE |
                WS_CHILD |
                WS_BORDER |
                ES_READONLY,
            195,
            280,
            350,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // QUESTION ID
    // ========================================================

    lblQuestionId =
        CreateWindowA(
            "STATIC",
            "Question ID:",
            WS_VISIBLE | WS_CHILD,
            550,
            65,
            170,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    hQuestionId =
        CreateWindowA(
            "EDIT",
            "",
            WS_VISIBLE |
                WS_CHILD |
                WS_BORDER,
            725,
            65,
            350,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // QUESTION ORDER
    // ========================================================

    lblQuestionOrder =
        CreateWindowA(
            "STATIC",
            "Order:",
            WS_VISIBLE | WS_CHILD,
            550,
            108,
            170,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    hQuestionOrder =
        CreateWindowA(
            "EDIT",
            "",
            WS_VISIBLE |
                WS_CHILD |
                WS_BORDER,
            725,
            108,
            350,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // QUESTION SCORE
    // ========================================================

    lblQuestionScore =
        CreateWindowA(
            "STATIC",
            "Score:",
            WS_VISIBLE | WS_CHILD,
            550,
            151,
            170,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    hQuestionScore =
        CreateWindowA(
            "EDIT",
            "",
            WS_VISIBLE |
                WS_CHILD |
                WS_BORDER,
            725,
            151,
            350,
            32,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // ADD QUESTION
    // ========================================================

    CreateWindowA(
        "BUTTON",
        "Add Question",
        WS_VISIBLE | WS_CHILD,
        550,
        205,
        235,
        42,
        hwnd,
        (HMENU)106,
        NULL,
        NULL);

    // ========================================================
    // REMOVE QUESTION
    // ========================================================

    CreateWindowA(
        "BUTTON",
        "Remove Question",
        WS_VISIBLE | WS_CHILD,
        800,
        205,
        235,
        42,
        hwnd,
        (HMENU)107,
        NULL,
        NULL);

    // ========================================================
    // ADD TEST
    // ========================================================

    CreateWindowA(
        "BUTTON",
        "Add Test",
        WS_VISIBLE | WS_CHILD,
        20,
        315,
        180,
        42,
        hwnd,
        (HMENU)101,
        NULL,
        NULL);

    // ========================================================
    // UPDATE TEST
    // ========================================================

    CreateWindowA(
        "BUTTON",
        "Update Test",
        WS_VISIBLE | WS_CHILD,
        210,
        315,
        180,
        42,
        hwnd,
        (HMENU)102,
        NULL,
        NULL);

    // ========================================================
    // DELETE TEST
    // ========================================================

    CreateWindowA(
        "BUTTON",
        "Delete Test",
        WS_VISIBLE | WS_CHILD,
        400,
        315,
        180,
        42,
        hwnd,
        (HMENU)103,
        NULL,
        NULL);

    // ========================================================
    // PUBLISH
    // ========================================================

    CreateWindowA(
        "BUTTON",
        "Publish",
        WS_VISIBLE | WS_CHILD,
        590,
        315,
        180,
        42,
        hwnd,
        (HMENU)104,
        NULL,
        NULL);

    // ========================================================
    // UNPUBLISH
    // ========================================================

    CreateWindowA(
        "BUTTON",
        "Unpublish",
        WS_VISIBLE | WS_CHILD,
        780,
        315,
        180,
        42,
        hwnd,
        (HMENU)109,
        NULL,
        NULL);

    // ========================================================
    // CLEAR
    // ========================================================

    CreateWindowA(
        "BUTTON",
        "Clear",
        WS_VISIBLE | WS_CHILD,
        970,
        315,
        110,
        42,
        hwnd,
        (HMENU)105,
        NULL,
        NULL);

    // ========================================================
    // TEST LIST LABEL
    // ========================================================

    lblTestList =
        CreateWindowA(
            "STATIC",
            "TEST LIST",
            WS_VISIBLE | WS_CHILD,
            20,
            372,
            1000,
            30,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // TEST LIST
    // ========================================================

    hTestList =
        CreateWindowA(
            "LISTBOX",
            "",
            WS_VISIBLE |
                WS_CHILD |
                WS_BORDER |
                LBS_NOTIFY |
                WS_VSCROLL,
            20,
            407,
            1040,
            130,
            hwnd,
            (HMENU)108,
            NULL,
            NULL);

    // ========================================================
    // QUESTION LIST LABEL
    // ========================================================

    lblQuestionList =
        CreateWindowA(
            "STATIC",
            "QUESTIONS IN SELECTED TEST",
            WS_VISIBLE | WS_CHILD,
            20,
            550,
            1000,
            30,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // QUESTION LIST
    // ========================================================

    hQuestionList =
        CreateWindowA(
            "LISTBOX",
            "",
            WS_VISIBLE |
                WS_CHILD |
                WS_BORDER |
                LBS_NOTIFY |
                WS_VSCROLL,
            20,
            585,
            1040,
            180,
            hwnd,
            NULL,
            NULL,
            NULL);

    // ========================================================
    // INITIAL DATA
    // ========================================================

    refreshTestList();

    // ========================================================
    // SHOW WINDOW
    // ========================================================

    ShowWindow(
        hwnd,
        SW_SHOW);

    UpdateWindow(hwnd);

    // ========================================================
    // INITIAL LAYOUT
    // ========================================================

    RECT rect;

    GetClientRect(
        hwnd,
        &rect);

    resizeTestGui(
        hwnd,
        rect.right - rect.left,
        rect.bottom - rect.top);
}