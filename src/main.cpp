#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "TestGui.h"
#include "QuestionGui.h"
#include "StudentGui.h"
#include "AdminGui.h"

using namespace std;

// =====================================================
// CONTROL IDs
// =====================================================

#define ID_USERNAME 101
#define ID_PASSWORD 102

#define ID_LOGIN 201
#define ID_REGISTER 202
#define ID_UPDATE 203
#define ID_CHANGE 204
#define ID_RECOVER 205

// =====================================================
// GLOBAL VARIABLES
// =====================================================

HWND hTitle = NULL;

HWND hUsernameLabel = NULL;
HWND hUsernameBox = NULL;

HWND hPasswordLabel = NULL;
HWND hPasswordBox = NULL;

HWND hLoginButton = NULL;
HWND hRegisterButton = NULL;
HWND hUpdateButton = NULL;
HWND hChangeButton = NULL;
HWND hRecoverButton = NULL;

HFONT hNormalFont = NULL;
HFONT hTitleFont = NULL;

// =====================================================
// GET TEXT
// =====================================================

string getText(HWND hwnd)
{
    char buffer[512] = {};

    GetWindowTextA(
        hwnd,
        buffer,
        sizeof(buffer));

    return string(buffer);
}

// =====================================================
// MESSAGE BOX
// =====================================================

void showMessage(
    HWND hwnd,
    const string &title,
    const string &message)
{
    MessageBoxA(
        hwnd,
        message.c_str(),
        title.c_str(),
        MB_OK | MB_ICONINFORMATION);
}

// =====================================================
// LARGE FORM WINDOW
// =====================================================

struct FormField
{
    string label;
    string value;
    bool password;
};

vector<FormField> formFields;
vector<string> formResult;
HWND formWindow = NULL;
bool formSubmitted = false;

#define ID_FORM_BASE 3000
#define ID_FORM_OK 3998
#define ID_FORM_CANCEL 3999

LRESULT CALLBACK FormWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        for (size_t i = 0; i < formFields.size(); ++i)
        {
            HWND label = CreateWindowA(
                "STATIC",
                formFields[i].label.c_str(),
                WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,
                35,
                30 + (int)i * 70,
                160,
                35,
                hwnd,
                NULL,
                NULL,
                NULL);

            HWND edit = CreateWindowA(
                "EDIT",
                formFields[i].value.c_str(),
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL |
                    (formFields[i].password ? ES_PASSWORD : 0),
                210,
                30 + (int)i * 70,
                390,
                38,
                hwnd,
                (HMENU)(INT_PTR)(ID_FORM_BASE + (int)i),
                NULL,
                NULL);

            SendMessageA(
                label,
                WM_SETFONT,
                (WPARAM)hNormalFont,
                TRUE);

            SendMessageA(
                edit,
                WM_SETFONT,
                (WPARAM)hNormalFont,
                TRUE);
        }

        int buttonY = 45 + (int)formFields.size() * 70;

        HWND ok = CreateWindowA(
            "BUTTON",
            "OK",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            235,
            buttonY,
            150,
            45,
            hwnd,
            (HMENU)ID_FORM_OK,
            NULL,
            NULL);

        HWND cancel = CreateWindowA(
            "BUTTON",
            "CANCEL",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            405,
            buttonY,
            150,
            45,
            hwnd,
            (HMENU)ID_FORM_CANCEL,
            NULL,
            NULL);

        SendMessageA(
            ok,
            WM_SETFONT,
            (WPARAM)hNormalFont,
            TRUE);

        SendMessageA(
            cancel,
            WM_SETFONT,
            (WPARAM)hNormalFont,
            TRUE);

        SetFocus(GetDlgItem(hwnd, ID_FORM_BASE));

        return 0;
    }

    case WM_COMMAND:
    {
        int id = LOWORD(wParam);

        if (id == ID_FORM_OK)
        {
            formResult.clear();

            for (size_t i = 0; i < formFields.size(); ++i)
            {
                char buffer[512] = {};

                GetWindowTextA(
                    GetDlgItem(hwnd, ID_FORM_BASE + (int)i),
                    buffer,
                    sizeof(buffer));

                formResult.push_back(buffer);
            }

            formSubmitted = true;

            DestroyWindow(hwnd);

            return 0;
        }

        if (id == ID_FORM_CANCEL)
        {
            formResult.clear();
            formSubmitted = false;

            DestroyWindow(hwnd);

            return 0;
        }

        return 0;
    }

    case WM_CLOSE:
        formResult.clear();
        formSubmitted = false;
        DestroyWindow(hwnd);
        return 0;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

vector<string> showForm(
    HWND parent,
    const string &title,
    const vector<string> &labels,
    const vector<string> &values = {},
    const vector<bool> &passwordFields = {})
{
    formFields.clear();
    formResult.clear();
    formSubmitted = false;

    for (size_t i = 0; i < labels.size(); ++i)
    {
        FormField field;

        field.label = labels[i];

        field.value =
            (i < values.size())
                ? values[i]
                : "";

        field.password =
            (i < passwordFields.size())
                ? passwordFields[i]
                : false;

        formFields.push_back(field);
    }

    static bool registered = false;

    const char CLASS_NAME[] =
        "QuizLargeFormWindow";

    if (!registered)
    {
        WNDCLASSA wc = {};

        wc.lpfnWndProc =
            FormWindowProc;

        wc.hInstance =
            GetModuleHandleA(NULL);

        wc.lpszClassName =
            CLASS_NAME;

        wc.hCursor =
            LoadCursorA(NULL, IDC_ARROW);

        wc.hbrBackground =
            (HBRUSH)(COLOR_WINDOW + 1);

        wc.style =
            CS_HREDRAW |
            CS_VREDRAW;

        RegisterClassA(&wc);

        registered = true;
    }

    int formHeight =
        150 +
        (int)labels.size() * 70;

    formWindow =
        CreateWindowExA(
            WS_EX_DLGMODALFRAME,
            CLASS_NAME,
            title.c_str(),
            WS_CAPTION | WS_SYSMENU,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            680,
            formHeight,
            parent,
            NULL,
            GetModuleHandleA(NULL),
            NULL);

    if (!formWindow)
        return {};

    EnableWindow(parent, FALSE);

    ShowWindow(
        formWindow,
        SW_SHOW);

    UpdateWindow(formWindow);

    RECT parentRect;
    RECT formRect;

    GetWindowRect(
        parent,
        &parentRect);

    GetWindowRect(
        formWindow,
        &formRect);

    int x =
        parentRect.left +
        ((parentRect.right - parentRect.left) -
         (formRect.right - formRect.left)) /
            2;

    int y =
        parentRect.top +
        ((parentRect.bottom - parentRect.top) -
         (formRect.bottom - formRect.top)) /
            2;

    SetWindowPos(
        formWindow,
        NULL,
        x,
        y,
        0,
        0,
        SWP_NOSIZE |
            SWP_NOZORDER |
            SWP_NOACTIVATE);

    SetForegroundWindow(formWindow);

    MSG msg;

    while (
        IsWindow(formWindow) &&
        GetMessageA(
            &msg,
            NULL,
            0,
            0))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    EnableWindow(parent, TRUE);

    SetForegroundWindow(parent);

    if (!formSubmitted)
        return {};

    return formResult;
}

// =====================================================
// ROLE DASHBOARD
// =====================================================

HWND dashboardWindow = NULL;

string currentUsername;
string currentFullName;
string currentRole;

int currentUserId = 0;

#define ID_DASH_BASE 5000
#define ID_DASH_LOGOUT 5099
#define ID_DASH_PROFILE 5100
#define ID_DASH_HEADER 5101

// =====================================================
// DASHBOARD CONTROLS
// =====================================================

vector<HWND> dashMenuButtons;
vector<HWND> dashContentControls;

HFONT dashTitleFont = NULL;
HFONT dashHeaderFont = NULL;
HFONT dashNormalFont = NULL;
HFONT dashSmallFont = NULL;

COLORREF DASH_BG =
    RGB(247, 249, 252);

COLORREF DASH_SIDEBAR =
    RGB(255, 255, 255);

COLORREF DASH_TEXT =
    RGB(30, 41, 59);

COLORREF DASH_MUTED =
    RGB(100, 116, 139);

COLORREF DASH_ACCENT =
    RGB(79, 70, 229);

COLORREF DASH_BORDER =
    RGB(226, 232, 240);

COLORREF DASH_CARD =
    RGB(255, 255, 255);

// =====================================================
// QUIZ REGISTRATION DATA
// =====================================================

string registrationFile =
    "data/registrations.txt";

vector<string> registeredQuizzes;

// -----------------------------------------------------
// Current student identifier
// -----------------------------------------------------

string getCurrentStudentId()
{
    /*
        Username is used instead of numeric ID because
        some existing users have IDs such as S1 or U003.
        It also prevents two students with the same numeric
        ID from sharing registration data.
    */

    return currentUsername;
}

// -----------------------------------------------------
// Load registrations from file
// -----------------------------------------------------

void loadRegisteredQuizzes()
{
    registeredQuizzes.clear();

    ifstream file(
        registrationFile);

    if (!file.is_open())
        return;

    string line;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string studentId;
        string quizName;

        getline(
            ss,
            studentId,
            '|');

        getline(
            ss,
            quizName,
            '|');

        if (
            studentId == getCurrentStudentId() &&
            !quizName.empty())
        {
            registeredQuizzes.push_back(
                quizName);
        }
    }

    file.close();
}

bool isQuizRegistered(const string &quizName);

vector<string> loadPublishedQuizItems()
{
    vector<string> items;
    ifstream file("data/tests.txt");

    if (!file.is_open())
        return items;

    string line;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        vector<string> fields;
        string field;

        while (getline(ss, field, '|'))
            fields.push_back(field);

        if (fields.size() < 10 || fields[9] != "Published")
            continue;

        items.push_back(
            fields[1] +
            " | " +
            fields[3] +
            " | " +
            fields[8] +
            " questions | " +
            fields[5] +
            " minutes | " +
            (isQuizRegistered(fields[1])
                 ? "Registered"
                 : "Published"));
    }

    return items;
}

vector<string> splitDataLine(const string &line)
{
    vector<string> fields;
    stringstream ss(line);
    string field;

    while (getline(ss, field, '|'))
        fields.push_back(field);

    return fields;
}

vector<string> loadStudentAttemptItems()
{
    vector<string> items;
    ifstream file("data/attempts.txt");
    string line;

    while (file.is_open() && getline(file, line))
    {
        vector<string> fields = splitDataLine(line);

        if (fields.size() < 6 || fields[0] != currentUsername)
            continue;

        items.push_back(
            "Attempt " + fields[1] +
            " | Test " + fields[2] +
            " | Started: " + fields[3] +
            " | Submitted: " + fields[4] +
            " | Status: " + fields[5]);
    }

    return items;
}

vector<string> loadStudentResultItems()
{
    vector<string> items;
    ifstream file("data/results.txt");
    string line;

    while (file.is_open() && getline(file, line))
    {
        vector<string> fields = splitDataLine(line);

        if (fields.size() < 12 || fields[0] != currentUsername)
            continue;

        items.push_back(
            "Attempt " + fields[1] +
            " | Score: " + fields[3] + "/" + fields[4] +
            " | Percentage: " + fields[5] + "%" +
            " | Grade: " + fields[6] +
            " | Correct: " + fields[7] +
            " | Wrong: " + fields[8] +
            " | Unanswered: " + fields[9] +
            " | Submitted: " + fields[11]);
    }

    return items;
}

vector<string> loadTeacherResultItems()
{
    vector<string> items;
    ifstream file("data/results.txt");
    string line;
    double totalPercentage = 0.0;
    double highestPercentage = 0.0;
    double lowestPercentage = 101.0;
    int count = 0;
    int passed = 0;

    while (file.is_open() && getline(file, line))
    {
        vector<string> fields = splitDataLine(line);

        if (fields.size() < 12)
            continue;

        try
        {
            double percentage = stod(fields[5]);
            totalPercentage += percentage;
            highestPercentage = max(highestPercentage, percentage);
            lowestPercentage = min(lowestPercentage, percentage);
            count++;

            if (percentage >= 60.0)
                passed++;

            items.push_back(
                "Student: " + fields[0] +
                " | Attempt: " + fields[1] +
                " | Score: " + fields[3] + "/" + fields[4] +
                " | Percentage: " + fields[5] + "%" +
                " | Grade: " + fields[6]);
        }
        catch (...)
        {
        }
    }

    if (count == 0)
    {
        items.push_back("No submitted examination results.");
        return items;
    }

    vector<string> summary;
    summary.push_back(
        "Average Score: " + to_string(totalPercentage / count) + "%");
    summary.push_back(
        "Highest Score: " + to_string(highestPercentage) + "%");
    summary.push_back(
        "Lowest Score: " + to_string(lowestPercentage) + "%");
    summary.push_back(
        "Pass Rate: " + to_string((passed * 100.0) / count) + "%");
    summary.push_back(
        "Fail Rate: " + to_string(((count - passed) * 100.0) / count) + "%");
    summary.push_back("--- Submitted Results ---");
    summary.insert(summary.end(), items.begin(), items.end());

    return summary;
}

vector<string> loadTeacherAttemptItems()
{
    vector<string> items;
    ifstream file("data/attempts.txt");
    string line;

    while (file.is_open() && getline(file, line))
    {
        vector<string> fields = splitDataLine(line);

        if (fields.size() < 6)
            continue;

        items.push_back(
            "Student: " + fields[0] +
            " | Attempt: " + fields[1] +
            " | Test: " + fields[2] +
            " | Status: " + fields[5] +
            " | Submitted: " + fields[4]);
    }

    if (items.empty())
        items.push_back("No quiz attempts recorded.");

    return items;
}

vector<string> loadTeacherRegistrationItems()
{
    vector<string> items;
    ifstream file(registrationFile);
    string line;

    while (file.is_open() && getline(file, line))
    {
        vector<string> fields = splitDataLine(line);

        if (fields.size() >= 2)
            items.push_back(
                "Student: " + fields[0] +
                " | Quiz: " + fields[1]);
    }

    if (items.empty())
        items.push_back("No student registrations recorded.");

    return items;
}

vector<string> loadUsersByRole(const string &wantedRole)
{
    vector<string> items;
    ifstream file("data/users.txt");
    string line;

    while (file.is_open() && getline(file, line))
    {
        vector<string> fields = splitDataLine(line);

        if (fields.size() >= 8 && fields[7] == wantedRole)
            items.push_back(
                fields[0] + " | " + fields[1] +
                " | " + fields[4] + " | " + fields[6]);
    }

    if (items.empty())
        items.push_back("No " + wantedRole + " accounts found.");

    return items;
}

vector<string> loadSystemLogItems()
{
    vector<string> items;
    const vector<string> files =
        {"data/users.txt",
         "data/registrations.txt",
         "data/attempts.txt",
         "data/results.txt",
         "data/tests.txt",
         "data/test_questions.txt"};

    for (const string &path : files)
    {
        ifstream file(path);
        int lineCount = 0;
        string line;

        while (file.is_open() && getline(file, line))
        {
            if (!line.empty())
                lineCount++;
        }

        items.push_back(
            path + " | Records: " + to_string(lineCount) +
            (file.is_open() ? " | Available" : " | Not created"));
    }

    return items;
}

// -----------------------------------------------------
// Check whether quiz is already registered
// -----------------------------------------------------

bool isQuizRegistered(
    const string &quizName)
{
    for (
        const string &q :
        registeredQuizzes)
    {
        if (q == quizName)
            return true;
    }

    return false;
}

// -----------------------------------------------------
// Save registration
// -----------------------------------------------------

bool saveQuizRegistration(
    const string &quizName)
{
    ofstream file(
        registrationFile,
        ios::app);

    if (!file.is_open())
        return false;

    file << getCurrentStudentId()
         << "|"
         << quizName
         << "\n";

    file.close();

    return true;
}

// =====================================================
// CLEAR DASHBOARD CONTROLS
// =====================================================

void clearDashboardControls()
{
    for (HWND h : dashMenuButtons)
    {
        if (IsWindow(h))
            DestroyWindow(h);
    }

    for (HWND h : dashContentControls)
    {
        if (IsWindow(h))
            DestroyWindow(h);
    }

    dashMenuButtons.clear();
    dashContentControls.clear();
}

// =====================================================
// CREATE DASHBOARD STATIC
// =====================================================

HWND createDashStatic(
    HWND parent,
    const string &text,
    int x,
    int y,
    int w,
    int height,
    HFONT font,
    DWORD style = SS_LEFT)
{
    HWND hWnd =
        CreateWindowA(
            "STATIC",
            text.c_str(),
            WS_VISIBLE |
                WS_CHILD |
                style,
            x,
            y,
            w,
            height,
            parent,
            NULL,
            NULL,
            NULL);

    if (font)
    {
        SendMessageA(
            hWnd,
            WM_SETFONT,
            (WPARAM)font,
            TRUE);
    }

    return hWnd;
}

// =====================================================
// CREATE DASHBOARD BUTTON
// =====================================================

HWND createDashButton(
    HWND parent,
    const string &text,
    int x,
    int y,
    int w,
    int height,
    int id,
    HFONT font)
{
    HWND hWnd =
        CreateWindowA(
            "BUTTON",
            text.c_str(),
            WS_VISIBLE |
                WS_CHILD |
                BS_PUSHBUTTON,
            x,
            y,
            w,
            height,
            parent,
            (HMENU)(INT_PTR)id,
            NULL,
            NULL);

    if (font)
    {
        SendMessageA(
            hWnd,
            WM_SETFONT,
            (WPARAM)font,
            TRUE);
    }

    return hWnd;
}

// =====================================================
// ADD DASHBOARD CARD
// =====================================================

void addDashCard(
    HWND hwnd,
    int x,
    int y,
    int w,
    int h,
    const string &title,
    const string &value,
    const string &detail)
{
    HWND card =
        CreateWindowA(
            "STATIC",
            "",
            WS_VISIBLE |
                WS_CHILD |
                SS_NOTIFY,
            x,
            y,
            w,
            h,
            hwnd,
            NULL,
            NULL,
            NULL);

    dashContentControls.push_back(card);

    HWND t =
        createDashStatic(
            hwnd,
            title,
            x + 22,
            y + 18,
            w - 44,
            28,
            dashSmallFont);

    HWND v =
        createDashStatic(
            hwnd,
            value,
            x + 22,
            y + 47,
            w - 44,
            42,
            dashHeaderFont);

    HWND d =
        createDashStatic(
            hwnd,
            detail,
            x + 22,
            y + 91,
            w - 44,
            26,
            dashSmallFont);

    dashContentControls.push_back(t);
    dashContentControls.push_back(v);
    dashContentControls.push_back(d);
}

// =====================================================
// ADD SECTION TITLE
// =====================================================

void addDashSectionTitle(
    HWND hwnd,
    const string &title,
    int x,
    int y,
    int w)
{
    HWND h =
        createDashStatic(
            hwnd,
            title,
            x,
            y,
            w,
            35,
            dashHeaderFont);

    dashContentControls.push_back(h);
}

// =====================================================
// STUDENT DASHBOARD
// =====================================================

void buildStudentDashboard(
    HWND hwnd,
    int width,
    int height)
{
    const int sidebar = 235;

    const int left =
        sidebar + 45;

    const int contentW =
        width - left - 45;

    createDashStatic(
        hwnd,
        "QuizExam",
        30,
        25,
        175,
        45,
        dashHeaderFont);

    createDashStatic(
        hwnd,
        "STUDENT",
        30,
        67,
        175,
        22,
        dashSmallFont);

    int y = 125;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Dashboard",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 0,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Available Quizzes",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 1,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Quiz Registration",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 2,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Quiz Examinations",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 3,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Previous Attempts",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 4,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Examination Results",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 5,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Personal Information",
            25,
            y,
            185,
            46,
            ID_DASH_PROFILE,
            dashNormalFont));

    createDashStatic(
        hwnd,
        "Signed in as\n" + currentFullName,
        30,
        height - 100,
        175,
        55,
        dashSmallFont);

    createDashStatic(
        hwnd,
        "Student",
        30,
        height - 45,
        175,
        25,
        dashSmallFont);

    string welcome =
        "Welcome back, " +
        currentFullName +
        ". Here is your examination dashboard.";

    createDashStatic(
        hwnd,
        "Student Dashboard",
        left,
        38,
        contentW - 260,
        45,
        dashTitleFont);

    createDashStatic(
        hwnd,
        welcome,
        left,
        82,
        contentW - 260,
        30,
        dashNormalFont);

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "LOGOUT",
            width - 155,
            48,
            110,
            40,
            ID_DASH_LOGOUT,
            dashNormalFont));

    int cardGap = 22;

    int cardW =
        (contentW - cardGap * 2) / 3;

    addDashCard(
        hwnd,
        left,
        145,
        cardW,
        135,
        "Available Quizzes",
        "--",
        "No quiz data yet");

    addDashCard(
        hwnd,
        left + cardW + cardGap,
        145,
        cardW,
        135,
        "Previous Attempts",
        "--",
        "No attempt data yet");

    addDashCard(
        hwnd,
        left + (cardW + cardGap) * 2,
        145,
        cardW,
        135,
        "Examination Results",
        "--",
        "No result data yet");

    addDashSectionTitle(
        hwnd,
        "Available Quizzes",
        left,
        315,
        contentW);

    int listY = 360;
    int listH = 105;

    int listW =
        (contentW * 2) / 3 - 12;

    addDashCard(
        hwnd,
        left,
        listY,
        listW,
        listH,
        "Available Quizzes",
        "No quizzes available",
        "Published quizzes will appear here when real Test data exists.");

    addDashCard(
        hwnd,
        left,
        listY + listH + 15,
        listW,
        listH,
        "Quiz Registration",
        "No registration data",
        "Quiz registration information will appear here when available.");

    addDashSectionTitle(
        hwnd,
        "Recent Examination Results",
        left + listW + 25,
        315,
        contentW - listW - 25);

    addDashCard(
        hwnd,
        left + listW + 25,
        listY,
        contentW - listW - 25,
        105,
        "Recent Results",
        "No results available",
        "Results will appear here after an examination is submitted.");

    addDashCard(
        hwnd,
        left + listW + 25,
        listY + 120,
        contentW - listW - 25,
        105,
        "Previous Attempts",
        "No attempts available",
        "Previous attempts will appear here when real data exists.");
}

// =====================================================
// TEACHER DASHBOARD
// =====================================================

void buildTeacherDashboard(
    HWND hwnd,
    int width,
    int height)
{
    const int sidebar = 235;

    const int left =
        sidebar + 45;

    const int contentW =
        width - left - 45;

    createDashStatic(
        hwnd,
        "QuizExam",
        30,
        25,
        175,
        45,
        dashHeaderFont);

    createDashStatic(
        hwnd,
        "TEACHER",
        30,
        67,
        175,
        22,
        dashSmallFont);

    int y = 125;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Dashboard",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 0,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Course & Topic Management",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 1,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Question Bank",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 2,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Test Management",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 3,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Results & Statistics",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 4,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Student Registrations",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 5,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Quiz Attempts",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 6,
            dashNormalFont));

    createDashStatic(
        hwnd,
        "Signed in as\n" + currentFullName,
        30,
        height - 100,
        175,
        55,
        dashSmallFont);

    createDashStatic(
        hwnd,
        "Teacher",
        30,
        height - 45,
        175,
        25,
        dashSmallFont);

    createDashStatic(
        hwnd,
        "Teacher Dashboard",
        left,
        38,
        contentW - 150,
        45,
        dashTitleFont);

    createDashStatic(
        hwnd,
        "Create, manage and monitor quizzes, questions and student results.",
        left,
        82,
        contentW - 150,
        30,
        dashNormalFont);

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "LOGOUT",
            width - 155,
            48,
            110,
            40,
            ID_DASH_LOGOUT,
            dashNormalFont));

    int cardGap = 22;

    int cardW =
        (contentW - cardGap * 2) / 3;

    addDashCard(
        hwnd,
        left,
        145,
        cardW,
        135,
        "Courses",
        "--",
        "No course data yet");

    addDashCard(
        hwnd,
        left + cardW + cardGap,
        145,
        cardW,
        135,
        "Question Bank",
        "--",
        "No question data yet");

    addDashCard(
        hwnd,
        left + (cardW + cardGap) * 2,
        145,
        cardW,
        135,
        "Published Tests",
        "--",
        "No test data yet");

    addDashSectionTitle(
        hwnd,
        "Quiz Management",
        left,
        315,
        contentW);

    int listY = 360;
    int listH = 105;

    int listW =
        (contentW * 2) / 3 - 12;

    addDashCard(
        hwnd,
        left,
        listY,
        listW,
        listH,
        "Test Management",
        "Create / Edit / Publish Tests",
        "Manage title, subject, time, questions and publication status.");

    addDashCard(
        hwnd,
        left,
        listY + listH + 15,
        listW,
        listH,
        "Question Bank",
        "Manage Questions & Answer Options",
        "Create, edit, delete, search and categorize questions.");

    addDashSectionTitle(
        hwnd,
        "Results & Statistics",
        left + listW + 25,
        315,
        contentW - listW - 25);

    addDashCard(
        hwnd,
        left + listW + 25,
        listY,
        contentW - listW - 25,
        105,
        "Average Score",
        "--",
        "No result data yet");

    addDashCard(
        hwnd,
        left + listW + 25,
        listY + 120,
        contentW - listW - 25,
        105,
        "Participation",
        "--",
        "No attempt data yet");
}

// =====================================================
// ADMIN DASHBOARD
// =====================================================

void buildAdminDashboard(
    HWND hwnd,
    int width,
    int height)
{
    const int sidebar = 235;

    const int left =
        sidebar + 45;

    const int contentW =
        width - left - 45;

    createDashStatic(
        hwnd,
        "QuizExam",
        30,
        25,
        175,
        45,
        dashHeaderFont);

    createDashStatic(
        hwnd,
        "ADMINISTRATOR",
        30,
        67,
        175,
        22,
        dashSmallFont);

    int y = 125;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Dashboard",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 0,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "User Management",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 1,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Student Information",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 2,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Teacher Information",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 3,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Quiz Monitoring",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 4,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "Reports",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 5,
            dashNormalFont));

    y += 55;

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "System Logs",
            25,
            y,
            185,
            46,
            ID_DASH_BASE + 6,
            dashNormalFont));

    createDashStatic(
        hwnd,
        "Signed in as\n" + currentFullName,
        30,
        height - 100,
        175,
        55,
        dashSmallFont);

    createDashStatic(
        hwnd,
        "Administrator",
        30,
        height - 45,
        175,
        25,
        dashSmallFont);

    createDashStatic(
        hwnd,
        "Administrator Dashboard",
        left,
        38,
        contentW - 150,
        45,
        dashTitleFont);

    createDashStatic(
        hwnd,
        "Centralized management and monitoring of the Quiz Examination System.",
        left,
        82,
        contentW - 150,
        30,
        dashNormalFont);

    dashMenuButtons.push_back(
        createDashButton(
            hwnd,
            "LOGOUT",
            width - 155,
            48,
            110,
            40,
            ID_DASH_LOGOUT,
            dashNormalFont));

    int cardGap = 22;

    int cardW =
        (contentW - cardGap * 2) / 3;

    addDashCard(
        hwnd,
        left,
        145,
        cardW,
        135,
        "Users",
        "--",
        "No user data yet");

    addDashCard(
        hwnd,
        left + cardW + cardGap,
        145,
        cardW,
        135,
        "Quizzes",
        "--",
        "No test data yet");

    addDashCard(
        hwnd,
        left + (cardW + cardGap) * 2,
        145,
        cardW,
        135,
        "Activities",
        "--",
        "No activity data yet");

    addDashSectionTitle(
        hwnd,
        "System Monitoring",
        left,
        315,
        contentW);

    int listY = 360;
    int listH = 105;

    int listW =
        (contentW * 2) / 3 - 12;

    addDashCard(
        hwnd,
        left,
        listY,
        listW,
        listH,
        "User Account Management",
        "Students & Teachers",
        "Add, edit, delete, lock/unlock accounts and manage permissions.");

    addDashCard(
        hwnd,
        left,
        listY + listH + 15,
        listW,
        listH,
        "Quiz System Monitoring",
        "Tests & Examination Data",
        "Monitor quiz status and manage examination data.");

    addDashSectionTitle(
        hwnd,
        "Reports & Logs",
        left + listW + 25,
        315,
        contentW - listW - 25);

    addDashCard(
        hwnd,
        left + listW + 25,
        listY,
        contentW - listW - 25,
        105,
        "Reports",
        "No report data",
        "Reports will appear here when examination data exists.");

    addDashCard(
        hwnd,
        left + listW + 25,
        listY + 120,
        contentW - listW - 25,
        105,
        "System Logs",
        "No log data",
        "System activity logs will appear here when available.");
}

// =====================================================
// DASHBOARD FEATURE WINDOW
// =====================================================

#define ID_FEATURE_CLOSE 7001
#define ID_FEATURE_ACTION 7002
#define ID_FEATURE_LIST 7003

string featureTitle;
string featureDescription;
vector<string> featureItems;
string featureActionText;

HWND featureWindow = NULL;
HWND featureList = NULL;
HWND featureActionButton = NULL;

// =====================================================
// OPEN FEATURE WINDOW
// =====================================================

void openFeatureWindow(
    HWND parent,
    const string &title,
    const string &description,
    const vector<string> &items,
    const string &actionText = "")
{
    featureTitle =
        title;

    featureDescription =
        description;

    featureItems =
        items;

    featureActionText =
        actionText;

    static bool registered = false;

    const char CLASS_NAME[] =
        "QuizFeatureWindow";

    if (!registered)
    {
        WNDCLASSA wc = {};

        wc.lpfnWndProc =
            [](HWND hwnd,
               UINT msg,
               WPARAM wParam,
               LPARAM lParam) -> LRESULT
        {
            switch (msg)
            {
            case WM_CREATE:
            {
                featureList =
                    CreateWindowExA(
                        WS_EX_CLIENTEDGE,
                        "LISTBOX",
                        "",
                        WS_VISIBLE |
                            WS_CHILD |
                            WS_VSCROLL |
                            LBS_NOTIFY |
                            LBS_NOINTEGRALHEIGHT,
                        35,
                        145,
                        700,
                        300,
                        hwnd,
                        (HMENU)(INT_PTR)ID_FEATURE_LIST,
                        GetModuleHandleA(NULL),
                        NULL);

                SendMessageA(
                    featureList,
                    WM_SETFONT,
                    (WPARAM)dashNormalFont,
                    TRUE);

                for (
                    const string &item :
                    featureItems)
                {
                    SendMessageA(
                        featureList,
                        LB_ADDSTRING,
                        0,
                        (LPARAM)item.c_str());
                }

                if (!featureActionText.empty())
                {
                    featureActionButton =
                        CreateWindowA(
                            "BUTTON",
                            featureActionText.c_str(),
                            WS_VISIBLE |
                                WS_CHILD |
                                BS_PUSHBUTTON,
                            35,
                            465,
                            210,
                            42,
                            hwnd,
                            (HMENU)(INT_PTR)ID_FEATURE_ACTION,
                            NULL,
                            NULL);

                    SendMessageA(
                        featureActionButton,
                        WM_SETFONT,
                        (WPARAM)dashNormalFont,
                        TRUE);
                }

                HWND closeButton =
                    CreateWindowA(
                        "BUTTON",
                        "CLOSE",
                        WS_VISIBLE |
                            WS_CHILD |
                            BS_PUSHBUTTON,
                        610,
                        465,
                        125,
                        42,
                        hwnd,
                        (HMENU)(INT_PTR)ID_FEATURE_CLOSE,
                        NULL,
                        NULL);

                SendMessageA(
                    closeButton,
                    WM_SETFONT,
                    (WPARAM)dashNormalFont,
                    TRUE);

                return 0;
            }

            case WM_COMMAND:
            {
                int id =
                    LOWORD(wParam);

                if (id == ID_FEATURE_CLOSE)
                {
                    DestroyWindow(hwnd);
                    return 0;
                }

                if (id == ID_FEATURE_ACTION)
                {
                    int selected =
                        (int)SendMessageA(
                            featureList,
                            LB_GETCURSEL,
                            0,
                            0);

                    if (selected == LB_ERR)
                    {
                        MessageBoxA(
                            hwnd,
                            "Please select a quiz first.",
                            "Quiz Examination System",
                            MB_OK | MB_ICONINFORMATION);

                        return 0;
                    }

                    char buffer[512] = {};

                    SendMessageA(
                        featureList,
                        LB_GETTEXT,
                        selected,
                        (LPARAM)buffer);

                    string selectedText =
                        buffer;

                    // =================================================
                    // QUIZ REGISTRATION
                    // =================================================

                    if (
                        featureTitle == "Available Quizzes" ||
                        featureTitle == "Quiz Registration")
                    {
                        string quizName =
                            selectedText;

                        size_t pos =
                            quizName.find("|");

                        if (pos != string::npos)
                        {
                            quizName =
                                quizName.substr(
                                    0,
                                    pos);
                        }

                        // Remove possible spaces at end
                        while (
                            !quizName.empty() &&
                            quizName.back() == ' ')
                        {
                            quizName.pop_back();
                        }

                        // Remove possible spaces at beginning
                        while (
                            !quizName.empty() &&
                            quizName.front() == ' ')
                        {
                            quizName.erase(
                                quizName.begin());
                        }

                        // -------------------------------------------------
                        // Already registered
                        // -------------------------------------------------

                        if (
                            isQuizRegistered(
                                quizName))
                        {
                            MessageBoxA(
                                hwnd,
                                "You are already registered for this quiz.",
                                "Quiz Registration",
                                MB_OK | MB_ICONINFORMATION);

                            return 0;
                        }

                        // -------------------------------------------------
                        // Save registration
                        // -------------------------------------------------

                        if (
                            saveQuizRegistration(
                                quizName))
                        {
                            registeredQuizzes.push_back(
                                quizName);

                            string message =
                                "Registration successful!\n\n"
                                "Quiz: " +
                                quizName;

                            MessageBoxA(
                                hwnd,
                                message.c_str(),
                                "Quiz Registration",
                                MB_OK | MB_ICONINFORMATION);
                        }
                        else
                        {
                            MessageBoxA(
                                hwnd,
                                "Cannot save registration data.\n\n"
                                "Please make sure the data folder exists.",
                                "Quiz Registration",
                                MB_OK | MB_ICONERROR);
                        }

                        return 0;
                    }

                    // =================================================
                    // OTHER FEATURES
                    // =================================================

                    MessageBoxA(
                        hwnd,
                        "The selected item has been processed.",
                        "Quiz Examination System",
                        MB_OK | MB_ICONINFORMATION);

                    return 0;
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

                RECT rc;

                GetClientRect(
                    hwnd,
                    &rc);

                HBRUSH bg =
                    CreateSolidBrush(
                        RGB(247, 249, 252));

                FillRect(
                    hdc,
                    &rc,
                    bg);

                DeleteObject(bg);

                SetBkMode(
                    hdc,
                    TRANSPARENT);

                SetTextColor(
                    hdc,
                    RGB(30, 41, 59));

                HFONT titleFont =
                    CreateFontA(
                        26,
                        0,
                        0,
                        0,
                        FW_BOLD,
                        FALSE,
                        FALSE,
                        FALSE,
                        DEFAULT_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        DEFAULT_PITCH |
                            FF_DONTCARE,
                        "Arial");

                HFONT normalFont =
                    CreateFontA(
                        17,
                        0,
                        0,
                        0,
                        FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        DEFAULT_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        DEFAULT_PITCH |
                            FF_DONTCARE,
                        "Arial");

                HFONT old =
                    (HFONT)SelectObject(
                        hdc,
                        titleFont);

                TextOutA(
                    hdc,
                    35,
                    28,
                    featureTitle.c_str(),
                    (int)featureTitle.size());

                SelectObject(
                    hdc,
                    normalFont);

                RECT descRect =
                    {
                        35,
                        72,
                        rc.right - 35,
                        125};

                DrawTextA(
                    hdc,
                    featureDescription.c_str(),
                    -1,
                    &descRect,
                    DT_WORDBREAK |
                        DT_LEFT);

                SelectObject(
                    hdc,
                    old);

                DeleteObject(titleFont);
                DeleteObject(normalFont);

                EndPaint(
                    hwnd,
                    &ps);

                return 0;
            }

            case WM_GETMINMAXINFO:
            {
                MINMAXINFO *info =
                    (MINMAXINFO *)lParam;

                info->ptMinTrackSize.x =
                    800;

                info->ptMinTrackSize.y =
                    600;

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
        };

        wc.hInstance =
            GetModuleHandleA(NULL);

        wc.lpszClassName =
            CLASS_NAME;

        wc.hCursor =
            LoadCursorA(
                NULL,
                IDC_ARROW);

        wc.hbrBackground =
            (HBRUSH)(COLOR_WINDOW + 1);

        RegisterClassA(&wc);

        registered = true;
    }

    featureWindow =
        CreateWindowExA(
            0,
            CLASS_NAME,
            title.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            800,
            600,
            parent,
            NULL,
            GetModuleHandleA(NULL),
            NULL);

    if (!featureWindow)
    {
        MessageBoxA(
            parent,
            "Cannot open this function window.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    ShowWindow(
        featureWindow,
        SW_SHOW);

    UpdateWindow(
        featureWindow);
}

// =====================================================
// DASHBOARD WINDOW PROCEDURE
// =====================================================

LRESULT CALLBACK DashboardWindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        dashTitleFont =
            CreateFontA(
                30,
                0,
                0,
                0,
                FW_BOLD,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                DEFAULT_PITCH |
                    FF_DONTCARE,
                "Arial");

        dashHeaderFont =
            CreateFontA(
                21,
                0,
                0,
                0,
                FW_BOLD,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                DEFAULT_PITCH |
                    FF_DONTCARE,
                "Arial");

        dashNormalFont =
            CreateFontA(
                17,
                0,
                0,
                0,
                FW_NORMAL,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                DEFAULT_PITCH |
                    FF_DONTCARE,
                "Arial");

        dashSmallFont =
            CreateFontA(
                14,
                0,
                0,
                0,
                FW_NORMAL,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                DEFAULT_PITCH |
                    FF_DONTCARE,
                "Arial");

        RECT rc;

        GetClientRect(
            hwnd,
            &rc);

        int width =
            rc.right - rc.left;

        int height =
            rc.bottom - rc.top;

        if (currentRole == "Student")
            buildStudentDashboard(
                hwnd,
                width,
                height);

        else if (currentRole == "Teacher")
            buildTeacherDashboard(
                hwnd,
                width,
                height);

        else if (currentRole == "Admin")
            buildAdminDashboard(
                hwnd,
                width,
                height);

        return 0;
    }

        // =================================================
        // BUTTON COMMAND
        // =================================================

    case WM_COMMAND:
    {
        int id =
            LOWORD(wParam);

        // -------------------------------------------------
        // LOGOUT
        // -------------------------------------------------

        if (id == ID_DASH_LOGOUT)
        {
            DestroyWindow(hwnd);
            return 0;
        }

        // -------------------------------------------------
        // PERSONAL INFORMATION
        // -------------------------------------------------

        if (id == ID_DASH_PROFILE)
        {
            openFeatureWindow(
                hwnd,
                "Personal Information",
                "Information of the currently signed-in account.",
                {"Username: " + currentUsername,
                 "Full name: " + currentFullName,
                 "Role: " + currentRole,
                 "User ID: " + to_string(currentUserId)});

            return 0;
        }

        // -------------------------------------------------
        // DASHBOARD MENU
        // -------------------------------------------------

        if (
            id >= ID_DASH_BASE &&
            id < ID_DASH_BASE + 20)
        {
            int index =
                id - ID_DASH_BASE;

            // =================================================
            // STUDENT
            // =================================================

            if (currentRole == "Student")
            {
                switch (index)
                {
                    // -------------------------------------------------
                    // Dashboard
                    // -------------------------------------------------

                case 0:
                {
                    clearDashboardControls();

                    RECT rc;

                    GetClientRect(
                        hwnd,
                        &rc);

                    buildStudentDashboard(
                        hwnd,
                        rc.right,
                        rc.bottom);

                    InvalidateRect(
                        hwnd,
                        NULL,
                        TRUE);

                    break;
                }

                    // -------------------------------------------------
                    // Available Quizzes
                    // -------------------------------------------------

                case 1:
                {
                    vector<string> availableQuizItems =
                        loadPublishedQuizItems();

                    bool hasPublishedQuiz =
                        !availableQuizItems.empty();

                    if (!hasPublishedQuiz)
                        availableQuizItems.push_back(
                            "No published quizzes available.");

                    openFeatureWindow(
                        hwnd,
                        "Available Quizzes",
                        "Published quizzes available for the current student.",
                        availableQuizItems,
                        hasPublishedQuiz
                            ? "Register Selected"
                            : "");

                    break;
                }

                    // -------------------------------------------------
                    // Quiz Registration
                    // -------------------------------------------------

                case 2:
                {
                    vector<string> registrationItems;

                    if (
                        registeredQuizzes.empty())
                    {
                        registrationItems =
                            {
                                "No quizzes registered yet. Select a quiz from Available Quizzes first."};
                    }
                    else
                    {
                        for (
                            const string &q :
                            registeredQuizzes)
                        {
                            registrationItems.push_back(
                                q +
                                " | Registration: Registered");
                        }
                    }

                    openFeatureWindow(
                        hwnd,
                        "Quiz Registration",
                        "Quizzes that the current student has registered for.",
                        registrationItems);

                    break;
                }

                    // -------------------------------------------------
                    // Quiz Examinations
                    // -------------------------------------------------

                case 3:
                    openStudentGUI(
                        hwnd,
                        currentUserId,
                        currentFullName,
                        currentUsername);
                    break;

                    // -------------------------------------------------
                    // Previous Attempts
                    // -------------------------------------------------

                case 4:
                {
                    vector<string> items =
                        loadStudentAttemptItems();

                    if (items.empty())
                        items.push_back("No previous attempts recorded yet.");

                    openFeatureWindow(
                        hwnd,
                        "Previous Attempts",
                        "Previous quiz attempts of the current student.",
                        items);

                    break;
                }

                    // -------------------------------------------------
                    // Examination Results
                    // -------------------------------------------------

                case 5:
                {
                    vector<string> items =
                        loadStudentResultItems();

                    if (items.empty())
                        items.push_back("No examination results available yet.");

                    openFeatureWindow(
                        hwnd,
                        "Examination Results",
                        "Results of completed examinations.",
                        items);

                    break;
                }
                }
            }

            // =================================================
            // TEACHER
            // =================================================

            else if (currentRole == "Teacher")
            {
                switch (index)
                {
                    // -------------------------------------------------
                    // Dashboard
                    // -------------------------------------------------

                case 0:
                {
                    clearDashboardControls();

                    RECT rc;

                    GetClientRect(
                        hwnd,
                        &rc);

                    buildTeacherDashboard(
                        hwnd,
                        rc.right,
                        rc.bottom);

                    InvalidateRect(
                        hwnd,
                        NULL,
                        TRUE);

                    break;
                }

                    // -------------------------------------------------
                    // Course & Topic Management
                    // Question Bank
                    // -------------------------------------------------

                case 1:
                case 2:
                    openQuestionGui(hwnd);
                    break;

                    // -------------------------------------------------
                    // Test Management
                    // -------------------------------------------------

                case 3:
                    openTestGui(hwnd);
                    break;

                    // -------------------------------------------------
                    // Results & Statistics
                    // -------------------------------------------------

                case 4:
                    openFeatureWindow(
                        hwnd,
                        "Results & Statistics",
                        "Teacher statistics and examination results.",
                        loadTeacherResultItems());

                    break;

                    // -------------------------------------------------
                    // Student Registrations
                    // -------------------------------------------------

                case 5:
                    openFeatureWindow(
                        hwnd,
                        "Student Registrations",
                        "Students registered for the teacher's quizzes.",
                        loadTeacherRegistrationItems());

                    break;

                    // -------------------------------------------------
                    // Quiz Attempts
                    // -------------------------------------------------

                case 6:
                    openFeatureWindow(
                        hwnd,
                        "Quiz Attempts",
                        "Student quiz attempts and submission information.",
                        loadTeacherAttemptItems());

                    break;
                }
            }

            // =================================================
            // ADMIN
            // =================================================

            else if (currentRole == "Admin")
            {
                switch (index)
                {
                    // -------------------------------------------------
                    // Dashboard
                    // -------------------------------------------------

                case 0:
                {
                    clearDashboardControls();

                    RECT rc;

                    GetClientRect(
                        hwnd,
                        &rc);

                    buildAdminDashboard(
                        hwnd,
                        rc.right,
                        rc.bottom);

                    InvalidateRect(
                        hwnd,
                        NULL,
                        TRUE);

                    break;
                }

                    // -------------------------------------------------
                    // User Management
                    // -------------------------------------------------

                case 1:
                    openAdminGui(hwnd);
                    break;

                    // -------------------------------------------------
                    // Student Information
                    // -------------------------------------------------

                case 2:
                    openFeatureWindow(
                        hwnd,
                        "Student Information",
                        "Student accounts currently stored in the system.",
                        loadUsersByRole("Student"));

                    break;

                    // -------------------------------------------------
                    // Teacher Information
                    // -------------------------------------------------

                case 3:
                    openFeatureWindow(
                        hwnd,
                        "Teacher Information",
                        "Teacher accounts currently stored in the system.",
                        loadUsersByRole("Teacher"));

                    break;

                    // -------------------------------------------------
                    // Quiz Monitoring
                    // -------------------------------------------------

                case 4:
                    openAdminGui(hwnd);
                    break;

                    // -------------------------------------------------
                    // Reports
                    // -------------------------------------------------

                case 5:
                    openFeatureWindow(
                        hwnd,
                        "Reports",
                        "Examination and system reports.",
                        loadTeacherResultItems());

                    break;

                    // -------------------------------------------------
                    // System Logs
                    // -------------------------------------------------

                case 6:
                    openFeatureWindow(
                        hwnd,
                        "System Logs",
                        "System activity and execution logs.",
                        loadSystemLogItems());

                    break;
                }
            }

            return 0;
        }

        return 0;
    }

        // =================================================
        // RESIZE
        // =================================================

    case WM_SIZE:
    {
        clearDashboardControls();

        RECT rc;

        GetClientRect(
            hwnd,
            &rc);

        if (currentRole == "Student")
        {
            buildStudentDashboard(
                hwnd,
                rc.right,
                rc.bottom);
        }
        else if (currentRole == "Teacher")
        {
            buildTeacherDashboard(
                hwnd,
                rc.right,
                rc.bottom);
        }
        else if (currentRole == "Admin")
        {
            buildAdminDashboard(
                hwnd,
                rc.right,
                rc.bottom);
        }

        InvalidateRect(
            hwnd,
            NULL,
            TRUE);

        return 0;
    }

        // =================================================
        // MINIMUM WINDOW SIZE
        // =================================================

    case WM_GETMINMAXINFO:
    {
        MINMAXINFO *info =
            (MINMAXINFO *)lParam;

        info->ptMinTrackSize.x =
            1050;

        info->ptMinTrackSize.y =
            650;

        return 0;
    }

        // =================================================
        // BACKGROUND
        // =================================================

    case WM_ERASEBKGND:
        return 1;

        // =================================================
        // PAINT
        // =================================================

    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc =
            BeginPaint(
                hwnd,
                &ps);

        RECT rc;

        GetClientRect(
            hwnd,
            &rc);

        HBRUSH bgBrush =
            CreateSolidBrush(
                DASH_BG);

        FillRect(
            hdc,
            &rc,
            bgBrush);

        DeleteObject(bgBrush);

        RECT sideRect =
            {
                0,
                0,
                235,
                rc.bottom};

        HBRUSH sideBrush =
            CreateSolidBrush(
                DASH_SIDEBAR);

        FillRect(
            hdc,
            &sideRect,
            sideBrush);

        DeleteObject(sideBrush);

        HPEN pen =
            CreatePen(
                PS_SOLID,
                1,
                DASH_BORDER);

        HPEN oldPen =
            (HPEN)SelectObject(
                hdc,
                pen);

        MoveToEx(
            hdc,
            235,
            0,
            NULL);

        LineTo(
            hdc,
            235,
            rc.bottom);

        SelectObject(
            hdc,
            oldPen);

        DeleteObject(pen);

        EndPaint(
            hwnd,
            &ps);

        return 0;
    }

        // =================================================
        // DESTROY DASHBOARD
        // =================================================

    case WM_DESTROY:
    {
        clearDashboardControls();

        if (dashTitleFont)
        {
            DeleteObject(
                dashTitleFont);

            dashTitleFont = NULL;
        }

        if (dashHeaderFont)
        {
            DeleteObject(
                dashHeaderFont);

            dashHeaderFont = NULL;
        }

        if (dashNormalFont)
        {
            DeleteObject(
                dashNormalFont);

            dashNormalFont = NULL;
        }

        if (dashSmallFont)
        {
            DeleteObject(
                dashSmallFont);

            dashSmallFont = NULL;
        }

        dashboardWindow = NULL;

        ShowWindow(
            GetParent(hwnd),
            SW_SHOW);

        SetForegroundWindow(
            GetParent(hwnd));

        return 0;
    }
    }

    return DefWindowProcA(
        hwnd,
        msg,
        wParam,
        lParam);
}

// =====================================================
// OPEN DASHBOARD
// =====================================================

void openDashboard(
    HWND parent,
    const string &role,
    const string &username,
    const string &fullName)
{
    currentRole =
        role;

    currentUsername =
        username;

    currentFullName =
        fullName;

    // =================================================
    // LOAD STUDENT REGISTRATION DATA
    // =================================================

    if (currentRole == "Student")
    {
        loadRegisteredQuizzes();
    }

    static bool registered = false;

    const char CLASS_NAME[] =
        "QuizRoleDashboard";

    if (!registered)
    {
        WNDCLASSA wc = {};

        wc.lpfnWndProc =
            DashboardWindowProc;

        wc.hInstance =
            GetModuleHandleA(NULL);

        wc.lpszClassName =
            CLASS_NAME;

        wc.hCursor =
            LoadCursorA(
                NULL,
                IDC_ARROW);

        wc.hbrBackground =
            (HBRUSH)(COLOR_WINDOW + 1);

        wc.style =
            CS_HREDRAW |
            CS_VREDRAW;

        RegisterClassA(&wc);

        registered = true;
    }

    dashboardWindow =
        CreateWindowExA(
            0,
            CLASS_NAME,
            "Quiz Examination System",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            1250,
            760,
            parent,
            NULL,
            GetModuleHandleA(NULL),
            NULL);

    if (!dashboardWindow)
        return;

    ShowWindow(
        parent,
        SW_HIDE);

    ShowWindow(
        dashboardWindow,
        SW_SHOW);

    UpdateWindow(
        dashboardWindow);
}

// =====================================================
// LOGIN
// =====================================================

void loginAccount(HWND hwnd)
{
    string username =
        getText(hUsernameBox);

    string password =
        getText(hPasswordBox);

    if (
        username.empty() ||
        password.empty())
    {
        showMessage(
            hwnd,
            "Login",
            "Please enter username/email and password.");

        return;
    }

    ifstream file(
        "data/users.txt");

    if (!file.is_open())
    {
        showMessage(
            hwnd,
            "Error",
            "Cannot open data/users.txt");

        return;
    }

    string line;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string id;
        string dbUsername;
        string dbPassword;
        string email;

        string fullName;
        string dateOfBirth;
        string status;
        string role;

        getline(ss, id, '|');
        getline(ss, dbUsername, '|');
        getline(ss, dbPassword, '|');
        getline(ss, email, '|');
        getline(ss, fullName, '|');
        getline(ss, dateOfBirth, '|');
        getline(ss, status, '|');
        getline(ss, role, '|');

        if (
            (username == dbUsername ||
             username == email) &&
            password == dbPassword)
        {
            file.close();

            if (status != "Active")
            {
                showMessage(
                    hwnd,
                    "Login",
                    "This account is not active.");

                return;
            }

            currentUserId = 0;

            try
            {
                currentUserId =
                    stoi(id);
            }
            catch (...)
            {
                /*
                    IDs such as U001/S1 are not purely numeric.
                    Dashboard navigation does not require
                    numeric ID.
                */
            }

            openDashboard(
                hwnd,
                role,
                dbUsername,
                fullName);

            return;
        }
    }

    file.close();

    showMessage(
        hwnd,
        "Login",
        "Invalid username/email or password.");
}

// =====================================================
// REGISTER
// =====================================================

void registerAccount(HWND hwnd)
{
    vector<string> data =
        showForm(
            hwnd,
            "Register Account",
            {"Full name:",
             "Email:",
             "Username:",
             "Password:",
             "Date of birth:"},
            {},
            {false,
             false,
             false,
             true,
             false});

    if (data.size() != 5)
        return;

    string fullName =
        data[0];

    string email =
        data[1];

    string username =
        data[2];

    string password =
        data[3];

    string dateOfBirth =
        data[4];

    for (
        const string &value :
        data)
    {
        if (value.empty())
        {
            showMessage(
                hwnd,
                "Register",
                "Please fill in all fields.");

            return;
        }
    }

    ifstream checkFile(
        "data/users.txt");

    if (!checkFile.is_open())
    {
        showMessage(
            hwnd,
            "Error",
            "Cannot open data/users.txt");

        return;
    }

    string line;

    while (
        getline(
            checkFile,
            line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string id;
        string dbUsername;
        string dbPassword;
        string dbEmail;

        getline(
            ss,
            id,
            '|');

        getline(
            ss,
            dbUsername,
            '|');

        getline(
            ss,
            dbPassword,
            '|');

        getline(
            ss,
            dbEmail,
            '|');

        if (dbUsername == username)
        {
            checkFile.close();

            showMessage(
                hwnd,
                "Register",
                "Username already exists.");

            return;
        }

        if (dbEmail == email)
        {
            checkFile.close();

            showMessage(
                hwnd,
                "Register",
                "Email already exists.");

            return;
        }
    }

    checkFile.close();

    ofstream file(
        "data/users.txt",
        ios::app);

    if (!file.is_open())
    {
        showMessage(
            hwnd,
            "Error",
            "Cannot open data/users.txt");

        return;
    }

    static int nextID = 100;

    file << "S"
         << nextID++
         << "|"
         << username
         << "|"
         << password
         << "|"
         << email
         << "|"
         << fullName
         << "|"
         << dateOfBirth
         << "|Active|Student\n";

    file.close();

    showMessage(
        hwnd,
        "Register",
        "Registration successful!");
}

// =====================================================
// UPDATE PROFILE
// =====================================================

void updateProfile(HWND hwnd)
{
    string username =
        getText(hUsernameBox);

    if (username.empty())
    {
        showMessage(
            hwnd,
            "Update Profile",
            "Please enter username/email first.");

        return;
    }

    vector<string> data =
        showForm(
            hwnd,
            "Update Profile",
            {"New full name:",
             "New email:",
             "New date of birth:"});

    if (data.size() != 3)
        return;

    if (
        data[0].empty() ||
        data[1].empty() ||
        data[2].empty())
    {
        showMessage(
            hwnd,
            "Update Profile",
            "Please fill in all fields.");

        return;
    }

    string fullName =
        data[0];

    string email =
        data[1];

    string dateOfBirth =
        data[2];

    ifstream input(
        "data/users.txt");

    if (!input.is_open())
    {
        showMessage(
            hwnd,
            "Error",
            "Cannot open data/users.txt");

        return;
    }

    string allData;
    string line;
    bool found = false;

    while (
        getline(
            input,
            line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string id;
        string dbUsername;
        string password;
        string dbEmail;

        string oldFullName;
        string oldDateOfBirth;
        string status;
        string role;

        getline(ss, id, '|');
        getline(ss, dbUsername, '|');
        getline(ss, password, '|');
        getline(ss, dbEmail, '|');
        getline(ss, oldFullName, '|');
        getline(ss, oldDateOfBirth, '|');
        getline(ss, status, '|');
        getline(ss, role, '|');

        if (
            dbUsername == username ||
            dbEmail == username)
        {
            oldFullName =
                fullName;

            dbEmail =
                email;

            oldDateOfBirth =
                dateOfBirth;

            found = true;
        }

        allData +=
            id + "|" +
            dbUsername + "|" +
            password + "|" +
            dbEmail + "|" +
            oldFullName + "|" +
            oldDateOfBirth + "|" +
            status + "|" +
            role + "\n";
    }

    input.close();

    if (!found)
    {
        showMessage(
            hwnd,
            "Update Profile",
            "User not found.");

        return;
    }

    ofstream output(
        "data/users.txt");

    if (!output.is_open())
    {
        showMessage(
            hwnd,
            "Error",
            "Cannot save data/users.txt");

        return;
    }

    output << allData;

    output.close();

    showMessage(
        hwnd,
        "Update Profile",
        "Profile updated successfully.");
}

// =====================================================
// CHANGE PASSWORD
// =====================================================

void changePassword(HWND hwnd)
{
    string username =
        getText(hUsernameBox);

    if (username.empty())
    {
        showMessage(
            hwnd,
            "Change Password",
            "Please enter username/email first.");

        return;
    }

    vector<string> data =
        showForm(
            hwnd,
            "Change Password",
            {"Current password:",
             "New password:"},
            {},
            {true,
             true});

    if (data.size() != 2)
        return;

    string oldPassword =
        data[0];

    string newPassword =
        data[1];

    if (
        oldPassword.empty() ||
        newPassword.empty())
    {
        showMessage(
            hwnd,
            "Change Password",
            "Please fill in all fields.");

        return;
    }

    ifstream input(
        "data/users.txt");

    if (!input.is_open())
    {
        showMessage(
            hwnd,
            "Error",
            "Cannot open data/users.txt");

        return;
    }

    string allData;
    string line;
    bool found = false;

    while (
        getline(
            input,
            line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string id;
        string dbUsername;
        string password;
        string email;

        string fullName;
        string dateOfBirth;
        string status;
        string role;

        getline(ss, id, '|');
        getline(ss, dbUsername, '|');
        getline(ss, password, '|');
        getline(ss, email, '|');
        getline(ss, fullName, '|');
        getline(ss, dateOfBirth, '|');
        getline(ss, status, '|');
        getline(ss, role, '|');

        if (
            (dbUsername == username ||
             email == username) &&
            password == oldPassword)
        {
            password =
                newPassword;

            found = true;
        }

        allData +=
            id + "|" +
            dbUsername + "|" +
            password + "|" +
            email + "|" +
            fullName + "|" +
            dateOfBirth + "|" +
            status + "|" +
            role + "\n";
    }

    input.close();

    if (!found)
    {
        showMessage(
            hwnd,
            "Change Password",
            "Username or current password is incorrect.");

        return;
    }

    ofstream output(
        "data/users.txt");

    if (!output.is_open())
    {
        showMessage(
            hwnd,
            "Error",
            "Cannot save data/users.txt");

        return;
    }

    output << allData;

    output.close();

    showMessage(
        hwnd,
        "Change Password",
        "Password changed successfully.");
}

// =====================================================
// RECOVER PASSWORD
// =====================================================

void recoverPassword(HWND hwnd)
{
    vector<string> data =
        showForm(
            hwnd,
            "Recover Password",
            {"Registered email:"});

    if (
        data.size() != 1 ||
        data[0].empty())
    {
        return;
    }

    string email =
        data[0];

    ifstream file(
        "data/users.txt");

    if (!file.is_open())
    {
        showMessage(
            hwnd,
            "Error",
            "Cannot open data/users.txt");

        return;
    }

    string line;

    while (
        getline(
            file,
            line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string id;
        string username;
        string password;
        string dbEmail;

        string fullName;
        string dateOfBirth;
        string status;
        string role;

        getline(ss, id, '|');
        getline(ss, username, '|');
        getline(ss, password, '|');
        getline(ss, dbEmail, '|');
        getline(ss, fullName, '|');
        getline(ss, dateOfBirth, '|');
        getline(ss, status, '|');
        getline(ss, role, '|');

        if (dbEmail == email)
        {
            file.close();

            string message =
                "Account found!\n\n"
                "Username: " +
                username +
                "\nPassword: " +
                password;

            showMessage(
                hwnd,
                "Recover Password",
                message);

            return;
        }
    }

    file.close();

    showMessage(
        hwnd,
        "Recover Password",
        "Email not found.");
}

// =====================================================
// RESIZE CONTROLS
// =====================================================

void resizeControls(
    HWND hwnd,
    int width,
    int height)
{
    if (width < 700)
        width = 700;

    if (height < 550)
        height = 550;

    // =================================================
    // TITLE
    // =================================================

    int titleWidth = 520;
    int titleHeight = 55;

    int titleX =
        (width - titleWidth) / 2;

    MoveWindow(
        hTitle,
        titleX,
        45,
        titleWidth,
        titleHeight,
        TRUE);

    // =================================================
    // FORM
    // =================================================

    int labelWidth = 220;
    int editWidth = 340;
    int gap = 20;

    int totalWidth =
        labelWidth +
        gap +
        editWidth;

    int startX =
        (width - totalWidth) / 2;

    // Username

    MoveWindow(
        hUsernameLabel,
        startX,
        150,
        labelWidth,
        40,
        TRUE);

    MoveWindow(
        hUsernameBox,
        startX +
            labelWidth +
            gap,
        150,
        editWidth,
        40,
        TRUE);

    // Password

    MoveWindow(
        hPasswordLabel,
        startX,
        220,
        labelWidth,
        40,
        TRUE);

    MoveWindow(
        hPasswordBox,
        startX +
            labelWidth +
            gap,
        220,
        editWidth,
        40,
        TRUE);

    // =================================================
    // LOGIN + REGISTER
    // =================================================

    int buttonWidth = 180;
    int buttonHeight = 50;
    int buttonGap = 25;

    int twoButtonWidth =
        buttonWidth * 2 +
        buttonGap;

    int buttonX =
        (width - twoButtonWidth) / 2;

    MoveWindow(
        hLoginButton,
        buttonX,
        300,
        buttonWidth,
        buttonHeight,
        TRUE);

    MoveWindow(
        hRegisterButton,
        buttonX +
            buttonWidth +
            buttonGap,
        300,
        buttonWidth,
        buttonHeight,
        TRUE);

    // =================================================
    // OTHER BUTTONS
    // =================================================

    int bigButtonWidth = 385;
    int bigButtonHeight = 50;

    int bigX =
        (width - bigButtonWidth) / 2;

    MoveWindow(
        hUpdateButton,
        bigX,
        375,
        bigButtonWidth,
        bigButtonHeight,
        TRUE);

    MoveWindow(
        hChangeButton,
        bigX,
        445,
        bigButtonWidth,
        bigButtonHeight,
        TRUE);

    MoveWindow(
        hRecoverButton,
        bigX,
        515,
        bigButtonWidth,
        bigButtonHeight,
        TRUE);

    InvalidateRect(
        hwnd,
        NULL,
        TRUE);
}

// =====================================================
// MAIN WINDOW PROCEDURE
// =====================================================

LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
        // =================================================
        // CREATE
        // =================================================

    case WM_CREATE:
    {
        // ---------------------------------------------
        // FONTS
        // ---------------------------------------------

        hNormalFont =
            CreateFontA(
                20,
                0,
                0,
                0,
                FW_NORMAL,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                DEFAULT_PITCH |
                    FF_DONTCARE,
                "Arial");

        hTitleFont =
            CreateFontA(
                26,
                0,
                0,
                0,
                FW_BOLD,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                DEFAULT_PITCH |
                    FF_DONTCARE,
                "Arial");

        // ---------------------------------------------
        // TITLE
        // ---------------------------------------------

        hTitle =
            CreateWindowA(
                "STATIC",
                "QUIZ EXAMINATION SYSTEM",
                WS_VISIBLE |
                    WS_CHILD |
                    SS_CENTER |
                    SS_CENTERIMAGE,
                0,
                0,
                520,
                55,
                hwnd,
                NULL,
                NULL,
                NULL);

        SendMessageA(
            hTitle,
            WM_SETFONT,
            (WPARAM)hTitleFont,
            TRUE);

        // ---------------------------------------------
        // USERNAME LABEL
        // ---------------------------------------------

        hUsernameLabel =
            CreateWindowA(
                "STATIC",
                "Username / Email:",
                WS_VISIBLE |
                    WS_CHILD |
                    SS_CENTERIMAGE,
                0,
                0,
                220,
                40,
                hwnd,
                NULL,
                NULL,
                NULL);

        SendMessageA(
            hUsernameLabel,
            WM_SETFONT,
            (WPARAM)hNormalFont,
            TRUE);

        // ---------------------------------------------
        // USERNAME BOX
        // ---------------------------------------------

        hUsernameBox =
            CreateWindowA(
                "EDIT",
                "",
                WS_VISIBLE |
                    WS_CHILD |
                    WS_BORDER |
                    ES_AUTOHSCROLL,
                0,
                0,
                340,
                40,
                hwnd,
                (HMENU)ID_USERNAME,
                NULL,
                NULL);

        SendMessageA(
            hUsernameBox,
            WM_SETFONT,
            (WPARAM)hNormalFont,
            TRUE);

        // ---------------------------------------------
        // PASSWORD LABEL
        // ---------------------------------------------

        hPasswordLabel =
            CreateWindowA(
                "STATIC",
                "Password:",
                WS_VISIBLE |
                    WS_CHILD |
                    SS_CENTERIMAGE,
                0,
                0,
                220,
                40,
                hwnd,
                NULL,
                NULL,
                NULL);

        SendMessageA(
            hPasswordLabel,
            WM_SETFONT,
            (WPARAM)hNormalFont,
            TRUE);

        // ---------------------------------------------
        // PASSWORD BOX
        // ---------------------------------------------

        hPasswordBox =
            CreateWindowA(
                "EDIT",
                "",
                WS_VISIBLE |
                    WS_CHILD |
                    WS_BORDER |
                    ES_PASSWORD |
                    ES_AUTOHSCROLL,
                0,
                0,
                340,
                40,
                hwnd,
                (HMENU)ID_PASSWORD,
                NULL,
                NULL);

        SendMessageA(
            hPasswordBox,
            WM_SETFONT,
            (WPARAM)hNormalFont,
            TRUE);

        // ---------------------------------------------
        // LOGIN
        // ---------------------------------------------

        hLoginButton =
            CreateWindowA(
                "BUTTON",
                "LOGIN",
                WS_VISIBLE |
                    WS_CHILD |
                    BS_PUSHBUTTON,
                0,
                0,
                180,
                50,
                hwnd,
                (HMENU)ID_LOGIN,
                NULL,
                NULL);

        // ---------------------------------------------
        // REGISTER
        // ---------------------------------------------

        hRegisterButton =
            CreateWindowA(
                "BUTTON",
                "REGISTER",
                WS_VISIBLE |
                    WS_CHILD |
                    BS_PUSHBUTTON,
                0,
                0,
                180,
                50,
                hwnd,
                (HMENU)ID_REGISTER,
                NULL,
                NULL);

        // ---------------------------------------------
        // UPDATE
        // ---------------------------------------------

        hUpdateButton =
            CreateWindowA(
                "BUTTON",
                "UPDATE PROFILE",
                WS_VISIBLE |
                    WS_CHILD |
                    BS_PUSHBUTTON,
                0,
                0,
                385,
                50,
                hwnd,
                (HMENU)ID_UPDATE,
                NULL,
                NULL);

        // ---------------------------------------------
        // CHANGE PASSWORD
        // ---------------------------------------------

        hChangeButton =
            CreateWindowA(
                "BUTTON",
                "CHANGE PASSWORD",
                WS_VISIBLE |
                    WS_CHILD |
                    BS_PUSHBUTTON,
                0,
                0,
                385,
                50,
                hwnd,
                (HMENU)ID_CHANGE,
                NULL,
                NULL);

        // ---------------------------------------------
        // RECOVER PASSWORD
        // ---------------------------------------------

        hRecoverButton =
            CreateWindowA(
                "BUTTON",
                "RECOVER PASSWORD",
                WS_VISIBLE |
                    WS_CHILD |
                    BS_PUSHBUTTON,
                0,
                0,
                385,
                50,
                hwnd,
                (HMENU)ID_RECOVER,
                NULL,
                NULL);

        // ---------------------------------------------
        // BUTTON FONT
        // ---------------------------------------------

        SendMessageA(
            hLoginButton,
            WM_SETFONT,
            (WPARAM)hNormalFont,
            TRUE);

        SendMessageA(
            hRegisterButton,
            WM_SETFONT,
            (WPARAM)hNormalFont,
            TRUE);

        SendMessageA(
            hUpdateButton,
            WM_SETFONT,
            (WPARAM)hNormalFont,
            TRUE);

        SendMessageA(
            hChangeButton,
            WM_SETFONT,
            (WPARAM)hNormalFont,
            TRUE);

        SendMessageA(
            hRecoverButton,
            WM_SETFONT,
            (WPARAM)hNormalFont,
            TRUE);

        // ---------------------------------------------
        // INITIAL LAYOUT
        // ---------------------------------------------

        RECT rect;

        GetClientRect(
            hwnd,
            &rect);

        resizeControls(
            hwnd,
            rect.right - rect.left,
            rect.bottom - rect.top);

        return 0;
    }

        // =================================================
        // PREVENT TOO SMALL WINDOW
        // =================================================

    case WM_GETMINMAXINFO:
    {
        MINMAXINFO *info =
            (MINMAXINFO *)lParam;

        info->ptMinTrackSize.x =
            700;

        info->ptMinTrackSize.y =
            550;

        return 0;
    }

        // =================================================
        // RESIZE
        // =================================================

    case WM_SIZE:
    {
        int width =
            LOWORD(lParam);

        int height =
            HIWORD(lParam);

        resizeControls(
            hwnd,
            width,
            height);

        return 0;
    }

        // =================================================
        // BUTTON CLICK
        // =================================================

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_LOGIN:
            loginAccount(hwnd);
            break;

        case ID_REGISTER:
            registerAccount(hwnd);
            break;

        case ID_UPDATE:
            updateProfile(hwnd);
            break;

        case ID_CHANGE:
            changePassword(hwnd);
            break;

        case ID_RECOVER:
            recoverPassword(hwnd);
            break;
        }

        return 0;
    }

        // =================================================
        // CLOSE
        // =================================================

    case WM_DESTROY:
    {
        if (hNormalFont)
            DeleteObject(hNormalFont);

        if (hTitleFont)
            DeleteObject(hTitleFont);

        PostQuitMessage(0);

        return 0;
    }
    }

    return DefWindowProcA(
        hwnd,
        msg,
        wParam,
        lParam);
}

// =====================================================
// MAIN
// =====================================================

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    const char CLASS_NAME[] =
        "QuizExaminationSystem";

    WNDCLASSA wc = {};

    wc.lpfnWndProc =
        WindowProc;

    wc.hInstance =
        hInstance;

    wc.lpszClassName =
        CLASS_NAME;

    wc.hCursor =
        LoadCursorA(
            NULL,
            IDC_ARROW);

    wc.hbrBackground =
        (HBRUSH)(COLOR_WINDOW + 1);

    wc.style =
        CS_HREDRAW |
        CS_VREDRAW;

    RegisterClassA(&wc);

    HWND hwnd =
        CreateWindowExA(
            0,
            CLASS_NAME,
            "Quiz Examination System",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            900,
            650,
            NULL,
            NULL,
            hInstance,
            NULL);

    if (!hwnd)
    {
        std::ofstream errorFile("data/startup_error.txt");
        errorFile << "CreateWindowExA failed. Win32 error: "
                  << GetLastError()
                  << "\n";

        MessageBoxA(
            NULL,
            "The main window could not be created. See data/startup_error.txt.",
            "Quiz Examination System",
            MB_OK | MB_ICONERROR);

        return 0;
    }

    ShowWindow(
        hwnd,
        nCmdShow);

    UpdateWindow(hwnd);

    MSG msg = {};

    while (
        GetMessageA(
            &msg,
            NULL,
            0,
            0))
    {
        TranslateMessage(&msg);

        DispatchMessageA(&msg);
    }

    return 0;
}