#include "StudentGUI.h"

#include "TestAttempt.h"
#include "StudentAnswer.h"
#include "Result.h"

#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <algorithm>

struct QuizQuestion
{
    int questionId;
    std::string content;
    std::vector<std::string> options;
    int correctOption;
    double score;
};

struct StudentGUIState
{
    HWND hwnd = nullptr;

    int studentId = 0;
    std::string studentName;
    std::string studentUsername;

    int testId = 1;
    std::string testTitle = "C++ Programming Fundamentals";

    TestAttempt *attempt = nullptr;
    Result *result = nullptr;

    std::vector<QuizQuestion> questions;
    std::vector<StudentAnswer> answers;

    std::vector<HWND> radioButtons;

    int currentQuestion = 0;

    int timeLimit = 300;
    int remainingSeconds = 300;

    bool testStarted = false;
    bool submitted = false;

    UINT_PTR timerId = 1;
};

static StudentGUIState g_state;

static const int ID_START_TEST = 1001;
static const int ID_SUBMIT_TEST = 1002;
static const int ID_NEXT = 1003;
static const int ID_PREVIOUS = 1004;
static const int ID_VIEW_RESULT = 1005;

static const int ID_RADIO_BASE = 2000;

// =====================================================
// ATTEMPT DATA
// =====================================================

static const std::string ATTEMPT_FILE =
    "data/attempts.txt";

static const std::string RESULT_FILE =
    "data/results.txt";

struct StoredQuestion
{
    int questionId;
    std::string content;
    std::vector<std::string> options;
    int correctOption;
};

struct StoredTestQuestion
{
    int questionId;
    int questionOrder;
    double score;
};

static bool isTestRegistered(const std::string &testTitle)
{
    std::ifstream file("data/registrations.txt");
    std::string line;

    while (file.is_open() && std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string username;
        std::string title;

        std::getline(ss, username, '|');
        std::getline(ss, title, '|');

        if (username == g_state.studentUsername &&
            title == testTitle)
            return true;
    }

    return false;
}

static bool loadPublishedQuiz()
{
    g_state.questions.clear();

    std::ifstream testFile("data/tests.txt");
    std::string line;
    bool foundTest = false;

    while (testFile.is_open() && std::getline(testFile, line))
    {
        std::stringstream ss(line);
        std::string idText;
        std::string title;
        std::string unused;
        std::string timeText;
        std::string status;

        std::getline(ss, idText, '|');
        std::getline(ss, title, '|');
        for (int i = 0; i < 3; ++i)
            std::getline(ss, unused, '|');
        std::getline(ss, timeText, '|');
        std::getline(ss, unused, '|');
        std::getline(ss, unused, '|');
        std::getline(ss, unused, '|');
        std::getline(ss, status);

        if (status == "Published" &&
            isTestRegistered(title))
        {
            try
            {
                g_state.testId = std::stoi(idText);
                g_state.testTitle = title;
                g_state.timeLimit = std::max(1, std::stoi(timeText)) * 60;
                foundTest = true;
                break;
            }
            catch (...)
            {
            }
        }
    }

    if (!foundTest)
        return false;

    std::vector<StoredTestQuestion> mappings;
    std::ifstream mappingFile("data/test_questions.txt");

    while (mappingFile.is_open() && std::getline(mappingFile, line))
    {
        std::stringstream ss(line);
        std::string testIdText;
        std::string questionIdText;
        std::string orderText;
        std::string scoreText;

        std::getline(ss, testIdText, '|');
        std::getline(ss, questionIdText, '|');
        std::getline(ss, orderText, '|');
        std::getline(ss, scoreText, '|');

        try
        {
            if (std::stoi(testIdText) == g_state.testId)
            {
                mappings.push_back(
                    {std::stoi(questionIdText),
                     std::stoi(orderText),
                     std::stod(scoreText)});
            }
        }
        catch (...)
        {
        }
    }

    std::vector<StoredQuestion> storedQuestions;
    std::ifstream questionFile("data/questions.txt");

    while (questionFile.is_open() && std::getline(questionFile, line))
    {
        std::stringstream ss(line);
        std::string idText;
        std::string unused;
        StoredQuestion question{};

        std::getline(ss, idText, '|');
        std::getline(ss, unused, '|');
        std::getline(ss, unused, '|');
        std::getline(ss, question.content);

        try
        {
            question.questionId = std::stoi(idText);
            question.correctOption = -1;
            storedQuestions.push_back(question);
        }
        catch (...)
        {
        }
    }

    std::ifstream optionFile("data/answer_options.txt");

    while (optionFile.is_open() && std::getline(optionFile, line))
    {
        std::stringstream ss(line);
        std::string questionIdText;
        std::string optionIdText;
        std::string correctText;
        std::string content;

        std::getline(ss, questionIdText, '|');
        std::getline(ss, optionIdText, '|');
        std::getline(ss, correctText, '|');
        std::getline(ss, content);

        try
        {
            int questionId = std::stoi(questionIdText);
            int optionId = std::stoi(optionIdText);

            for (StoredQuestion &question : storedQuestions)
            {
                if (question.questionId == questionId)
                {
                    question.options.push_back(content);

                    if (correctText == "1")
                        question.correctOption = optionId;

                    break;
                }
            }
        }
        catch (...)
        {
        }
    }

    if (mappings.empty())
        return false;

    g_state.questions.clear();

    std::sort(
        mappings.begin(),
        mappings.end(),
        [](const StoredTestQuestion &left,
           const StoredTestQuestion &right)
        {
            return left.questionOrder < right.questionOrder;
        });

    for (const StoredTestQuestion &mapping : mappings)
    {
        for (const StoredQuestion &stored : storedQuestions)
        {
            if (stored.questionId != mapping.questionId ||
                stored.options.empty() ||
                stored.correctOption < 1)
                continue;

            QuizQuestion question;
            question.questionId = stored.questionId;
            question.content = stored.content;
            question.options = stored.options;
            question.correctOption = stored.correctOption;
            question.score = mapping.score;
            g_state.questions.push_back(question);
            break;
        }
    }

    return !g_state.questions.empty();
}

static int getNextAttemptId()
{
    std::ifstream file(ATTEMPT_FILE);

    if (!file.is_open())
        return 1;

    std::string line;
    int maxAttemptId = 0;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);

        std::string studentIdText;
        std::string attemptIdText;

        std::getline(ss, studentIdText, '|');
        std::getline(ss, attemptIdText, '|');

        try
        {
            int attemptId = std::stoi(attemptIdText);

            if (attemptId > maxAttemptId)
                maxAttemptId = attemptId;
        }
        catch (...)
        {
            continue;
        }
    }

    file.close();

    return maxAttemptId + 1;
}

static bool saveAttempt()
{
    if (!g_state.attempt)
        return false;

    std::ofstream file(
        ATTEMPT_FILE,
        std::ios::app);

    if (!file.is_open())
        return false;

    if (!g_state.studentUsername.empty())
        file << g_state.studentUsername;
    else
        file << g_state.studentId;

    file
        << "|"
        << g_state.attempt->getAttemptId()
        << "|"
        << g_state.attempt->getTestId()
        << "|"
        << g_state.attempt->getStartTime()
        << "|"
        << g_state.attempt->getSubmitTime()
        << "|";

    if (g_state.attempt->isSubmitted())
        file << "Submitted";
    else
        file << "InProgress";

    file << "\n";

    file.close();

    return true;
}

static bool saveResult()
{
    if (!g_state.result || !g_state.attempt)
        return false;

    std::ofstream file(RESULT_FILE, std::ios::app);

    if (!file.is_open())
        return false;

    if (!g_state.studentUsername.empty())
        file << g_state.studentUsername;
    else
        file << g_state.studentId;

    file << "|"
         << g_state.attempt->getAttemptId()
         << "|"
         << g_state.attempt->getTestId()
         << "|"
         << g_state.result->getTotalScore()
         << "|"
         << g_state.result->getMaxScore()
         << "|"
         << g_state.result->getPercentage()
         << "|"
         << g_state.result->getGrade()
         << "|"
         << g_state.result->getCorrectCount()
         << "|"
         << g_state.result->getWrongCount()
         << "|"
         << g_state.result->getUnansweredCount()
         << "|"
         << g_state.result->getTimeTaken()
         << "|"
         << g_state.result->getSubmittedAt()
         << "\n";

    return true;
}

// =====================================================
// TIME
// =====================================================

static std::string formatTime(int seconds)
{
    int minutes = seconds / 60;
    int secs = seconds % 60;

    std::ostringstream oss;

    oss << std::setfill('0')
        << std::setw(2) << minutes
        << ":"
        << std::setw(2) << secs;

    return oss.str();
}

// =====================================================
// MESSAGE
// =====================================================

static void showMessage(
    HWND hwnd,
    const std::string &message)
{
    MessageBoxA(
        hwnd,
        message.c_str(),
        "Quiz Examination System",
        MB_OK | MB_ICONINFORMATION);
}

// =====================================================
// SAMPLE QUIZ
// =====================================================

static void loadSampleQuiz()
{
    g_state.questions.clear();

    QuizQuestion q1;

    q1.questionId = 1;

    q1.content =
        "Which language is mainly used in this project?";

    q1.options =
        {
            "Python",
            "Java",
            "C++",
            "PHP"};

    q1.correctOption = 3;
    q1.score = 1.0;

    g_state.questions.push_back(q1);

    QuizQuestion q2;

    q2.questionId = 2;

    q2.content =
        "Which concept allows a class to inherit from another class?";

    q2.options =
        {
            "Inheritance",
            "Compilation",
            "Iteration",
            "Recursion"};

    q2.correctOption = 1;
    q2.score = 1.0;

    g_state.questions.push_back(q2);

    QuizQuestion q3;

    q3.questionId = 3;

    q3.content =
        "Which keyword is used to create an object dynamically in C++?";

    q3.options =
        {
            "class",
            "new",
            "object",
            "create"};

    q3.correctOption = 2;
    q3.score = 1.0;

    g_state.questions.push_back(q3);

    QuizQuestion q4;

    q4.questionId = 4;

    q4.content =
        "What does OOP stand for?";

    q4.options =
        {
            "Object Oriented Programming",
            "Open Online Program",
            "Object Operating Process",
            "Online Object Protocol"};

    q4.correctOption = 1;
    q4.score = 1.0;

    g_state.questions.push_back(q4);

    QuizQuestion q5;

    q5.questionId = 5;

    q5.content =
        "Which data type represents true or false?";

    q5.options =
        {
            "int",
            "double",
            "bool",
            "char"};

    q5.correctOption = 3;
    q5.score = 1.0;

    g_state.questions.push_back(q5);
}

// =====================================================
// QUESTION CONTROLS
// =====================================================

static void clearQuestionControls()
{
    for (HWND h : g_state.radioButtons)
    {
        if (h)
            DestroyWindow(h);
    }

    g_state.radioButtons.clear();
}

static void saveCurrentAnswer(HWND hwnd)
{
    if (!g_state.testStarted ||
        g_state.submitted ||
        g_state.currentQuestion < 0 ||
        g_state.currentQuestion >=
            static_cast<int>(g_state.questions.size()))
    {
        return;
    }

    if (g_state.radioButtons.empty())
        return;

    for (
        int i = 0;
        i < static_cast<int>(
                g_state.radioButtons.size());
        i++)
    {
        if (
            SendMessage(
                g_state.radioButtons[i],
                BM_GETCHECK,
                0,
                0) == BST_CHECKED)
        {
            int optionId = i + 1;

            g_state.answers[g_state.currentQuestion]
                .setSelectedOption(optionId);

            return;
        }
    }

    g_state.answers[g_state.currentQuestion]
        .clearAnswer();
}

static void restoreCurrentAnswer()
{
    if (
        g_state.currentQuestion < 0 ||
        g_state.currentQuestion >=
            static_cast<int>(
                g_state.answers.size()))
    {
        return;
    }

    int selected =
        g_state.answers[g_state.currentQuestion]
            .getSelectedOptionId();

    if (
        selected < 1 ||
        selected >
            static_cast<int>(
                g_state.radioButtons.size()))
    {
        return;
    }

    SendMessage(
        g_state.radioButtons[selected - 1],
        BM_SETCHECK,
        BST_CHECKED,
        0);
}

static void createQuestionControls(HWND hwnd)
{
    clearQuestionControls();

    if (g_state.questions.empty())
        return;

    QuizQuestion &q =
        g_state.questions[g_state.currentQuestion];

    int y = 200;

    for (
        int i = 0;
        i < static_cast<int>(
                q.options.size());
        i++)
    {
        HWND radio = CreateWindowA(
            "BUTTON",
            q.options[i].c_str(),
            WS_CHILD |
                WS_VISIBLE |
                BS_AUTORADIOBUTTON,
            70,
            y,
            650,
            35,
            hwnd,
            (HMENU)(INT_PTR)(ID_RADIO_BASE + i),
            GetModuleHandle(nullptr),
            nullptr);

        g_state.radioButtons.push_back(
            radio);

        y += 45;
    }

    restoreCurrentAnswer();
}

static void drawQuestion(HWND hwnd)
{
    InvalidateRect(
        hwnd,
        nullptr,
        TRUE);

    createQuestionControls(hwnd);
}

// =====================================================
// START TEST
// =====================================================

static void startTest(HWND hwnd)
{
    if (g_state.testStarted)
    {
        showMessage(
            hwnd,
            "The test has already started.");

        return;
    }

    bool loaded = loadPublishedQuiz();

    if (g_state.questions.empty())
    {
        showMessage(
            hwnd,
            loaded
                ? "The registered quiz has no available questions."
                : "Please register for a published quiz before starting.");

        return;
    }

    g_state.answers.clear();

    for (
        int i = 0;
        i < static_cast<int>(
                g_state.questions.size());
        i++)
    {
        g_state.answers.emplace_back(
            i + 1,
            1,
            g_state.questions[i].questionId);
    }

    delete g_state.attempt;

    g_state.attempt = new TestAttempt(
        getNextAttemptId(),
        g_state.testId,
        g_state.studentId);

    delete g_state.result;

    g_state.result = nullptr;

    g_state.currentQuestion = 0;

    g_state.remainingSeconds =
        g_state.timeLimit;

    g_state.testStarted = true;

    g_state.submitted = false;

    SetTimer(
        hwnd,
        g_state.timerId,
        1000,
        nullptr);

    drawQuestion(hwnd);
}

// =====================================================
// CALCULATE RESULT
// =====================================================

static void calculateResult()
{
    if (!g_state.attempt)
        return;

    double maximumScore = 0.0;

    for (int i = 0; i < static_cast<int>(g_state.questions.size()); ++i)
    {
        const QuizQuestion &q = g_state.questions[i];
        maximumScore += q.score;
        g_state.answers[i].evaluate(q.correctOption, q.score);
    }

    int secondsTaken =
        g_state.timeLimit -
        g_state.remainingSeconds;

    delete g_state.result;

    g_state.result =
        new Result(
            1,
            g_state.attempt->getAttemptId());

    g_state.result->calculate(
        g_state.answers,
        maximumScore,
        secondsTaken,
        g_state.attempt->getSubmitTime());

    saveResult();
}

// =====================================================
// SUBMIT TEST
// =====================================================

static void submitTest(
    HWND hwnd,
    bool autoSubmit)
{
    if (!g_state.testStarted)
    {
        showMessage(
            hwnd,
            "Please start the test first.");

        return;
    }

    if (g_state.submitted)
    {
        showMessage(
            hwnd,
            "This test has already been submitted.");

        return;
    }

    saveCurrentAnswer(hwnd);

    if (!autoSubmit)
    {
        int confirm =
            MessageBoxA(
                hwnd,
                "Are you sure you want to submit the test?",
                "Confirm Submission",
                MB_YESNO |
                    MB_ICONQUESTION);

        if (confirm != IDYES)
            return;
    }

    KillTimer(
        hwnd,
        g_state.timerId);

    g_state.attempt->submit();

    g_state.submitted = true;

    // Save attempt history
    if (!saveAttempt())
    {
        MessageBoxA(
            hwnd,
            "The test was submitted, but the attempt history could not be saved.",
            "Warning",
            MB_OK |
                MB_ICONWARNING);
    }

    calculateResult();

    clearQuestionControls();

    if (autoSubmit)
    {
        MessageBoxA(
            hwnd,
            "Time is up. Your test has been submitted automatically.",
            "Time Up",
            MB_OK |
                MB_ICONWARNING);
    }
    else
    {
        showMessage(
            hwnd,
            "Your test has been submitted successfully.");
    }

    InvalidateRect(
        hwnd,
        nullptr,
        TRUE);
}

// =====================================================
// SHOW RESULT
// =====================================================

static void showResult(HWND hwnd)
{
    if (
        !g_state.submitted ||
        !g_state.result)
    {
        showMessage(
            hwnd,
            "Please complete and submit the test first.");

        return;
    }

    std::ostringstream oss;

    oss << std::fixed
        << std::setprecision(2);

    oss << "EXAMINATION RESULT\n\n";

    oss << "Score: "
        << g_state.result->getTotalScore()
        << " / "
        << g_state.result->getMaxScore()
        << "\n";

    oss << "Percentage: "
        << g_state.result->getPercentage()
        << "%\n";

    oss << "Grade: "
        << g_state.result->getGrade()
        << "\n\n";

    oss << "Correct: "
        << g_state.result->getCorrectCount()
        << "\n";

    oss << "Wrong: "
        << g_state.result->getWrongCount()
        << "\n";

    oss << "Unanswered: "
        << g_state.result->getUnansweredCount()
        << "\n";

    oss << "Time taken: "
        << formatTime(
               g_state.result->getTimeTaken())
        << "\n\n";

    oss << "Submitted at: "
        << g_state.result->getSubmittedAt();

    MessageBoxA(
        hwnd,
        oss.str().c_str(),
        "Examination Result",
        MB_OK |
            MB_ICONINFORMATION);
}

// =====================================================
// NAVIGATION
// =====================================================

static void previousQuestion(HWND hwnd)
{
    if (
        !g_state.testStarted ||
        g_state.submitted)
        return;

    saveCurrentAnswer(hwnd);

    if (g_state.currentQuestion > 0)
    {
        g_state.currentQuestion--;

        drawQuestion(hwnd);
    }
}

static void nextQuestion(HWND hwnd)
{
    if (
        !g_state.testStarted ||
        g_state.submitted)
        return;

    saveCurrentAnswer(hwnd);

    if (
        g_state.currentQuestion <
        static_cast<int>(
            g_state.questions.size()) -
            1)
    {
        g_state.currentQuestion++;

        drawQuestion(hwnd);
    }
}

// =====================================================
// DRAW TEXT
// =====================================================

static void drawText(
    HDC hdc,
    const std::string &text,
    int x,
    int y,
    int size = 18)
{
    HFONT font =
        CreateFontA(
            size,
            0,
            0,
            0,
            FW_NORMAL,
            FALSE,
            FALSE,
            FALSE,
            ANSI_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH |
                FF_SWISS,
            "Segoe UI");

    HFONT oldFont =
        (HFONT)SelectObject(
            hdc,
            font);

    TextOutA(
        hdc,
        x,
        y,
        text.c_str(),
        static_cast<int>(
            text.length()));

    SelectObject(
        hdc,
        oldFont);

    DeleteObject(font);
}

// =====================================================
// DRAW STUDENT GUI
// =====================================================

static void drawStudentGUI(
    HWND hwnd,
    HDC hdc)
{
    RECT rect;

    GetClientRect(
        hwnd,
        &rect);

    int width = rect.right;

    HBRUSH background =
        CreateSolidBrush(
            RGB(245, 247, 250));

    FillRect(
        hdc,
        &rect,
        background);

    DeleteObject(
        background);

    RECT header =
        {
            0,
            0,
            width,
            90};

    HBRUSH headerBrush =
        CreateSolidBrush(
            RGB(37, 99, 235));

    FillRect(
        hdc,
        &header,
        headerBrush);

    DeleteObject(
        headerBrush);

    drawText(
        hdc,
        "Student Dashboard",
        30,
        22,
        26);

    drawText(
        hdc,
        "Quiz Examination System",
        30,
        55,
        14);

    drawText(
        hdc,
        "Student: " +
            g_state.studentName,
        500,
        25,
        16);

    if (!g_state.testStarted)
    {
        drawText(
            hdc,
            "Available Quiz",
            50,
            120,
            24);

        drawText(
            hdc,
            g_state.testTitle,
            70,
            170,
            20);

        std::ostringstream quizInfo;

        if (g_state.questions.empty())
        {
            quizInfo << "Register for a published quiz to begin";
        }
        else
        {
            quizInfo << g_state.questions.size()
                     << " Questions   |   "
                     << (g_state.timeLimit / 60)
                     << " Minutes";
        }

        drawText(
            hdc,
            quizInfo.str(),
            70,
            210,
            16);

        drawText(
            hdc,
            g_state.questions.empty()
                ? "Status: Registration required"
                : "Status: Published",
            70,
            245,
            16);

        return;
    }

    if (g_state.questions.empty())
        return;

    QuizQuestion &q =
        g_state.questions[g_state.currentQuestion];

    std::ostringstream title;

    title << "Question "
          << (g_state.currentQuestion + 1)
          << " / "
          << g_state.questions.size();

    drawText(
        hdc,
        title.str(),
        50,
        115,
        22);

    drawText(
        hdc,
        q.content,
        50,
        150,
        20);

    std::string timerText =
        "Time: " +
        formatTime(
            g_state.remainingSeconds);

    drawText(
        hdc,
        timerText,
        650,
        115,
        20);

    if (g_state.submitted)
    {
        drawText(
            hdc,
            "SUBMITTED",
            650,
            145,
            18);
    }
}

// =====================================================
// WINDOW PROCEDURE
// =====================================================

static LRESULT CALLBACK StudentGUIProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        CreateWindowA(
            "BUTTON",
            "Start Test",
            WS_CHILD |
                WS_VISIBLE |
                BS_PUSHBUTTON,
            50,
            285,
            140,
            40,
            hwnd,
            (HMENU)ID_START_TEST,
            GetModuleHandle(nullptr),
            nullptr);

        CreateWindowA(
            "BUTTON",
            "Previous",
            WS_CHILD |
                WS_VISIBLE |
                BS_PUSHBUTTON,
            50,
            390,
            120,
            40,
            hwnd,
            (HMENU)ID_PREVIOUS,
            GetModuleHandle(nullptr),
            nullptr);

        CreateWindowA(
            "BUTTON",
            "Next",
            WS_CHILD |
                WS_VISIBLE |
                BS_PUSHBUTTON,
            185,
            390,
            120,
            40,
            hwnd,
            (HMENU)ID_NEXT,
            GetModuleHandle(nullptr),
            nullptr);

        CreateWindowA(
            "BUTTON",
            "Submit Test",
            WS_CHILD |
                WS_VISIBLE |
                BS_PUSHBUTTON,
            320,
            390,
            140,
            40,
            hwnd,
            (HMENU)ID_SUBMIT_TEST,
            GetModuleHandle(nullptr),
            nullptr);

        CreateWindowA(
            "BUTTON",
            "View Result",
            WS_CHILD |
                WS_VISIBLE |
                BS_PUSHBUTTON,
            475,
            390,
            140,
            40,
            hwnd,
            (HMENU)ID_VIEW_RESULT,
            GetModuleHandle(nullptr),
            nullptr);

        return 0;
    }

    case WM_COMMAND:
    {
        int id =
            LOWORD(wParam);

        if (id == ID_START_TEST)
        {
            startTest(hwnd);
        }
        else if (id == ID_PREVIOUS)
        {
            previousQuestion(hwnd);
        }
        else if (id == ID_NEXT)
        {
            nextQuestion(hwnd);
        }
        else if (id == ID_SUBMIT_TEST)
        {
            submitTest(
                hwnd,
                false);
        }
        else if (id == ID_VIEW_RESULT)
        {
            showResult(hwnd);
        }

        return 0;
    }

    case WM_TIMER:
    {
        if (
            wParam == g_state.timerId &&
            g_state.testStarted &&
            !g_state.submitted)
        {
            if (g_state.remainingSeconds > 0)
            {
                g_state.remainingSeconds--;

                InvalidateRect(
                    hwnd,
                    nullptr,
                    TRUE);
            }
            else
            {
                submitTest(
                    hwnd,
                    true);
            }
        }

        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc =
            BeginPaint(
                hwnd,
                &ps);

        drawStudentGUI(
            hwnd,
            hdc);

        EndPaint(
            hwnd,
            &ps);

        return 0;
    }

    case WM_DESTROY:
    {
        KillTimer(
            hwnd,
            g_state.timerId);

        clearQuestionControls();

        delete g_state.attempt;

        delete g_state.result;

        g_state.attempt = nullptr;

        g_state.result = nullptr;

        g_state.hwnd = nullptr;

        return 0;
    }
    }

    return DefWindowProc(
        hwnd,
        msg,
        wParam,
        lParam);
}

// =====================================================
// OPEN STUDENT GUI
// =====================================================

void openStudentGUI(
    HWND parent,
    int studentId,
    const std::string &studentName,
    const std::string &studentUsername)
{
    g_state.studentId = studentId;

    g_state.studentName =
        studentName;

    g_state.studentUsername =
        studentUsername;

    if (!loadPublishedQuiz())
    {
        g_state.testTitle = "No registered quiz";
        g_state.timeLimit = 0;
    }

    g_state.testStarted = false;

    g_state.submitted = false;

    g_state.currentQuestion = 0;

    g_state.remainingSeconds = 300;

    clearQuestionControls();

    WNDCLASSA wc{};

    wc.lpfnWndProc =
        StudentGUIProc;

    wc.hInstance =
        GetModuleHandle(nullptr);

    wc.lpszClassName =
        "StudentQuizGUIClass";

    wc.hCursor =
        LoadCursor(
            nullptr,
            IDC_ARROW);

    RegisterClassA(&wc);

    HWND hwnd =
        CreateWindowExA(
            0,
            "StudentQuizGUIClass",
            "Student - Quiz Examination",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            900,
            600,
            parent,
            nullptr,
            GetModuleHandle(nullptr),
            nullptr);

    if (!hwnd)
    {
        MessageBoxA(
            parent,
            "Cannot create Student GUI.",
            "Error",
            MB_OK |
                MB_ICONERROR);

        return;
    }

    g_state.hwnd = hwnd;

    ShowWindow(
        hwnd,
        SW_SHOW);

    UpdateWindow(hwnd);
}