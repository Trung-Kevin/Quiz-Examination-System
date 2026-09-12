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

static void showMessage(HWND hwnd, const std::string &message)
{
    MessageBoxA(
        hwnd,
        message.c_str(),
        "Quiz Examination System",
        MB_OK | MB_ICONINFORMATION);
}

static void loadSampleQuiz()
{
    g_state.questions.clear();

    QuizQuestion q1;
    q1.questionId = 1;
    q1.content = "Which language is mainly used in this project?";
    q1.options =
        {
            "Python",
            "Java",
            "C++",
            "PHP"};
    q1.correctOption = 3;
    q1.score = 1.0;

    QuizQuestion q2;
    q2.questionId = 2;
    q2.content = "Which concept allows a class to inherit from another class?";
    q2.options =
        {
            "Inheritance",
            "Compilation",
            "Iteration",
            "Recursion"};
    q2.correctOption = 1;
    q2.score = 1.0;

    QuizQuestion q3;
    q3.questionId = 3;
    q3.content = "Which keyword is used to create an object dynamically in C++?";
    q3.options =
        {
            "class",
            "new",
            "object",
            "create"};
    q3.correctOption = 2;
    q3.score = 1.0;

    QuizQuestion q4;
    q4.questionId = 4;
    q4.content = "What does OOP stand for?";
    q4.options =
        {
            "Object Oriented Programming",
            "Open Online Program",
            "Object Operating Process",
            "Online Object Protocol"};
    q4.correctOption = 1;
    q4.score = 1.0;

    QuizQuestion q5;
    q5.questionId = 5;
    q5.content = "Which data type represents true or false?";
    q5.options =
        {
            "int",
            "double",
            "bool",
            "char"};
    q5.correctOption = 3;
    q5.score = 1.0;
}

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

    for (int i = 0; i < static_cast<int>(g_state.radioButtons.size()); i++)
    {
        if (SendMessage(
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

    g_state.answers[g_state.currentQuestion].clearAnswer();
}

static void restoreCurrentAnswer()
{
    if (g_state.currentQuestion < 0 ||
        g_state.currentQuestion >=
            static_cast<int>(g_state.answers.size()))
    {
        return;
    }

    int selected =
        g_state.answers[g_state.currentQuestion]
            .getSelectedOptionId();

    if (selected < 1 ||
        selected > static_cast<int>(g_state.radioButtons.size()))
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

    int y = 155;

    for (int i = 0; i < static_cast<int>(q.options.size()); i++)
    {
        HWND radio = CreateWindowA(
            "BUTTON",
            q.options[i].c_str(),
            WS_CHILD | WS_VISIBLE |
                BS_AUTORADIOBUTTON,
            70,
            y,
            650,
            35,
            hwnd,
            (HMENU)(ID_RADIO_BASE + i),
            GetModuleHandle(nullptr),
            nullptr);

        g_state.radioButtons.push_back(radio);

        y += 45;
    }

    restoreCurrentAnswer();
}

static void drawQuestion(HWND hwnd)
{
    InvalidateRect(hwnd, nullptr, TRUE);
    createQuestionControls(hwnd);
}

static void startTest(HWND hwnd)
{
    if (g_state.testStarted)
    {
        showMessage(hwnd, "The test has already started.");
        return;
    }

    loadSampleQuiz();

    if (g_state.questions.empty())
    {
        showMessage(hwnd, "No quiz questions available.");
        return;
    }

    g_state.answers.clear();

    for (int i = 0; i < static_cast<int>(g_state.questions.size()); i++)
    {
        g_state.answers.emplace_back(
            i + 1,
            1,
            g_state.questions[i].questionId);
    }

    delete g_state.attempt;
    g_state.attempt = new TestAttempt(
        1,
        1,
        g_state.studentId);

    g_state.currentQuestion = 0;

    g_state.timeLimit = 300;
    g_state.remainingSeconds = g_state.timeLimit;

    g_state.testStarted = true;
    g_state.submitted = false;

    SetTimer(
        hwnd,
        g_state.timerId,
        1000,
        nullptr);

    drawQuestion(hwnd);
}

static void calculateResult()
{
    if (!g_state.attempt)
        return;

    double maximumScore = 0.0;

    for (const QuizQuestion &q : g_state.questions)
    {
        maximumScore += q.score;
    }

    int secondsTaken =
        g_state.timeLimit - g_state.remainingSeconds;

    g_state.result = new Result(1, g_state.attempt->getAttemptId());

    g_state.result->calculate(
        g_state.answers,
        maximumScore,
        secondsTaken,
        g_state.attempt->getSubmitTime());
}

static void submitTest(HWND hwnd, bool autoSubmit)
{
    if (!g_state.testStarted)
    {
        showMessage(hwnd, "Please start the test first.");
        return;
    }

    if (g_state.submitted)
    {
        showMessage(hwnd, "This test has already been submitted.");
        return;
    }

    saveCurrentAnswer(hwnd);

    if (!autoSubmit)
    {
        int confirm = MessageBoxA(
            hwnd,
            "Are you sure you want to submit the test?",
            "Confirm Submission",
            MB_YESNO | MB_ICONQUESTION);

        if (confirm != IDYES)
            return;
    }

    KillTimer(hwnd, g_state.timerId);

    g_state.attempt->submit();

    g_state.submitted = true;

    calculateResult();

    clearQuestionControls();

    if (autoSubmit)
    {
        MessageBoxA(
            hwnd,
            "Time is up. Your test has been submitted automatically.",
            "Time Up",
            MB_OK | MB_ICONWARNING);
    }
    else
    {
        showMessage(
            hwnd,
            "Your test has been submitted successfully.");
    }

    InvalidateRect(hwnd, nullptr, TRUE);
}

static void showResult(HWND hwnd)
{
    if (!g_state.submitted || !g_state.result)
    {
        showMessage(
            hwnd,
            "Please complete and submit the test first.");
        return;
    }

    std::ostringstream oss;

    oss << std::fixed << std::setprecision(2);

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
        << formatTime(g_state.result->getTimeTaken())
        << "\n\n";

    oss << "Submitted at: "
        << g_state.result->getSubmittedAt();

    MessageBoxA(
        hwnd,
        oss.str().c_str(),
        "Examination Result",
        MB_OK | MB_ICONINFORMATION);
}

static void previousQuestion(HWND hwnd)
{
    if (!g_state.testStarted ||
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
    if (!g_state.testStarted ||
        g_state.submitted)
        return;

    saveCurrentAnswer(hwnd);

    if (g_state.currentQuestion <
        static_cast<int>(g_state.questions.size()) - 1)
    {
        g_state.currentQuestion++;
        drawQuestion(hwnd);
    }
}

static void drawText(
    HDC hdc,
    const std::string &text,
    int x,
    int y,
    int size = 18)
{
    HFONT font = CreateFontA(
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
        DEFAULT_PITCH | FF_SWISS,
        "Segoe UI");

    HFONT oldFont =
        (HFONT)SelectObject(hdc, font);

    TextOutA(
        hdc,
        x,
        y,
        text.c_str(),
        static_cast<int>(text.length()));

    SelectObject(hdc, oldFont);
    DeleteObject(font);
}

static void drawStudentGUI(
    HWND hwnd,
    HDC hdc)
{
    RECT rect;
    GetClientRect(hwnd, &rect);

    int width = rect.right;
    int height = rect.bottom;

    HBRUSH background =
        CreateSolidBrush(RGB(245, 247, 250));

    FillRect(hdc, &rect, background);

    DeleteObject(background);

    // Header
    RECT header =
        {
            0,
            0,
            width,
            90};

    HBRUSH headerBrush =
        CreateSolidBrush(RGB(37, 99, 235));

    FillRect(
        hdc,
        &header,
        headerBrush);

    DeleteObject(headerBrush);

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

    // Student name
    drawText(
        hdc,
        "Student: " + g_state.studentName,
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
            "C++ Programming Fundamentals",
            70,
            170,
            20);

        drawText(
            hdc,
            "5 Questions   |   5 Minutes   |   5 Marks",
            70,
            210,
            16);

        drawText(
            hdc,
            "Status: Published",
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
        145,
        20);

    // Timer
    std::string timerText =
        "Time: " +
        formatTime(g_state.remainingSeconds);

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
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
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
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
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
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
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
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
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
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
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
        int id = LOWORD(wParam);

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
            submitTest(hwnd, false);
        }
        else if (id == ID_VIEW_RESULT)
        {
            showResult(hwnd);
        }

        return 0;
    }

    case WM_TIMER:
    {
        if (wParam == g_state.timerId &&
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
                submitTest(hwnd, true);
            }
        }

        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc =
            BeginPaint(hwnd, &ps);

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

        delete g_state.attempt;
        delete g_state.result;

        g_state.attempt = nullptr;
        g_state.result = nullptr;

        return 0;
    }
    }

    return DefWindowProc(
        hwnd,
        msg,
        wParam,
        lParam);
}

void openStudentGUI(
    HWND parent,
    int studentId,
    const std::string &studentName)
{
    g_state.studentId = studentId;
    g_state.studentName = studentName;

    WNDCLASSA wc{};

    wc.lpfnWndProc = StudentGUIProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = "StudentQuizGUIClass";
    wc.hCursor = LoadCursor(
        nullptr,
        IDC_ARROW);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
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
            MB_OK | MB_ICONERROR);

        return;
    }

    g_state.hwnd = hwnd;

    ShowWindow(
        hwnd,
        SW_SHOW);

    UpdateWindow(hwnd);
}