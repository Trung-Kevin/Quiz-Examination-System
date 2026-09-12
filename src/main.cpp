#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include "TestGui.h"
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

#include <vector>

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
                35, 30 + (int)i * 70, 160, 35,
                hwnd, NULL, NULL, NULL);

            HWND edit = CreateWindowA(
                "EDIT",
                formFields[i].value.c_str(),
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL |
                    (formFields[i].password ? ES_PASSWORD : 0),
                210, 30 + (int)i * 70, 390, 38,
                hwnd, (HMENU)(ID_FORM_BASE + (int)i),
                NULL, NULL);

            SendMessageA(label, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
            SendMessageA(edit, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
        }

        int buttonY = 45 + (int)formFields.size() * 70;

        HWND ok = CreateWindowA(
            "BUTTON", "OK",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            235, buttonY, 150, 45,
            hwnd, (HMENU)ID_FORM_OK, NULL, NULL);

        HWND cancel = CreateWindowA(
            "BUTTON", "CANCEL",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            405, buttonY, 150, 45,
            hwnd, (HMENU)ID_FORM_CANCEL, NULL, NULL);

        SendMessageA(ok, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
        SendMessageA(cancel, WM_SETFONT, (WPARAM)hNormalFont, TRUE);

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
        field.value = (i < values.size()) ? values[i] : "";
        field.password = (i < passwordFields.size()) ? passwordFields[i] : false;
        formFields.push_back(field);
    }

    static bool registered = false;
    const char CLASS_NAME[] = "QuizLargeFormWindow";

    if (!registered)
    {
        WNDCLASSA wc = {};
        wc.lpfnWndProc = FormWindowProc;
        wc.hInstance = GetModuleHandleA(NULL);
        wc.lpszClassName = CLASS_NAME;
        wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.style = CS_HREDRAW | CS_VREDRAW;

        RegisterClassA(&wc);
        registered = true;
    }

    int formHeight = 150 + (int)labels.size() * 70;

    formWindow = CreateWindowExA(
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
    ShowWindow(formWindow, SW_SHOW);
    UpdateWindow(formWindow);

    // Center the form relative to the main window.
    RECT parentRect, formRect;
    GetWindowRect(parent, &parentRect);
    GetWindowRect(formWindow, &formRect);

    int x = parentRect.left +
            ((parentRect.right - parentRect.left) -
             (formRect.right - formRect.left)) /
                2;

    int y = parentRect.top +
            ((parentRect.bottom - parentRect.top) -
             (formRect.bottom - formRect.top)) /
                2;

    SetWindowPos(
        formWindow, NULL, x, y, 0, 0,
        SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

    SetForegroundWindow(formWindow);

    MSG msg;
    while (IsWindow(formWindow) &&
           GetMessageA(&msg, NULL, 0, 0))
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

#define ID_DASH_BASE 5000
#define ID_DASH_LOGOUT 5099
#define ID_DASH_PROFILE 5100
#define ID_DASH_HEADER 5101

// Dashboard controls
vector<HWND> dashMenuButtons;
vector<HWND> dashContentControls;
HFONT dashTitleFont = NULL;
HFONT dashHeaderFont = NULL;
HFONT dashNormalFont = NULL;
HFONT dashSmallFont = NULL;

COLORREF DASH_BG = RGB(247, 249, 252);
COLORREF DASH_SIDEBAR = RGB(255, 255, 255);
COLORREF DASH_TEXT = RGB(30, 41, 59);
COLORREF DASH_MUTED = RGB(100, 116, 139);
COLORREF DASH_ACCENT = RGB(79, 70, 229);
COLORREF DASH_BORDER = RGB(226, 232, 240);
COLORREF DASH_CARD = RGB(255, 255, 255);

void clearDashboardControls()
{
    for (HWND h : dashMenuButtons)
        if (IsWindow(h))
            DestroyWindow(h);

    for (HWND h : dashContentControls)
        if (IsWindow(h))
            DestroyWindow(h);

    dashMenuButtons.clear();
    dashContentControls.clear();
}

HWND createDashStatic(HWND parent, const string &text, int x, int y, int w, int height,
                      HFONT font, DWORD style = SS_LEFT)
{
    HWND hWnd = CreateWindowA(
        "STATIC", text.c_str(),
        WS_VISIBLE | WS_CHILD | style,
        x, y, w, height,
        parent, NULL, NULL, NULL);

    if (font)
        SendMessageA(hWnd, WM_SETFONT, (WPARAM)font, TRUE);

    return hWnd;
}

HWND createDashButton(HWND parent, const string &text, int x, int y, int w, int height,
                      int id, HFONT font)
{
    HWND hWnd = CreateWindowA(
        "BUTTON", text.c_str(),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        x, y, w, height,
        parent, (HMENU)(INT_PTR)id, NULL, NULL);

    if (font)
        SendMessageA(hWnd, WM_SETFONT, (WPARAM)font, TRUE);

    return hWnd;
}

void addDashCard(HWND hwnd, int x, int y, int w, int h,
                 const string &title, const string &value, const string &detail)
{
    HWND card = CreateWindowA(
        "STATIC", "",
        WS_VISIBLE | WS_CHILD | SS_NOTIFY,
        x, y, w, h,
        hwnd, NULL, NULL, NULL);

    // The static itself is only used as a visual container.
    // Its children provide the card content.
    dashContentControls.push_back(card);

    HWND t = createDashStatic(hwnd, title, x + 22, y + 18, w - 44, 28, dashSmallFont);
    HWND v = createDashStatic(hwnd, value, x + 22, y + 47, w - 44, 42, dashHeaderFont);
    HWND d = createDashStatic(hwnd, detail, x + 22, y + 91, w - 44, 26, dashSmallFont);

    dashContentControls.push_back(t);
    dashContentControls.push_back(v);
    dashContentControls.push_back(d);
}

void addDashSectionTitle(HWND hwnd, const string &title, int x, int y, int w)
{
    HWND h = createDashStatic(hwnd, title, x, y, w, 35, dashHeaderFont);
    dashContentControls.push_back(h);
}

void buildStudentDashboard(HWND hwnd, int width, int height)
{
    const int sidebar = 235;
    const int left = sidebar + 45;
    const int contentW = width - left - 45;

    createDashStatic(
        hwnd, "QuizExam", 30, 25, 175, 45,
        dashHeaderFont);

    createDashStatic(
        hwnd, "STUDENT", 30, 67, 175, 22,
        dashSmallFont);

    int y = 125;

    dashMenuButtons.push_back(
        createDashButton(hwnd, "Dashboard", 25, y, 185, 46,
                         ID_DASH_BASE + 0, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(
        createDashButton(hwnd, "Available Quizzes", 25, y, 185, 46,
                         ID_DASH_BASE + 1, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(
        createDashButton(hwnd, "Quiz Registration", 25, y, 185, 46,
                         ID_DASH_BASE + 2, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(
        createDashButton(hwnd, "Quiz Examinations", 25, y, 185, 46,
                         ID_DASH_BASE + 3, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(
        createDashButton(hwnd, "Previous Attempts", 25, y, 185, 46,
                         ID_DASH_BASE + 4, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(
        createDashButton(hwnd, "Examination Results", 25, y, 185, 46,
                         ID_DASH_BASE + 5, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(
        createDashButton(hwnd, "Personal Information", 25, y, 185, 46,
                         ID_DASH_PROFILE, dashNormalFont));

    createDashStatic(
        hwnd,
        "Signed in as\n" + currentFullName,
        30, height - 100, 175, 55,
        dashSmallFont);

    createDashStatic(
        hwnd,
        "Student",
        30, height - 45, 175, 25,
        dashSmallFont);

    string welcome =
        "Welcome back, " + currentFullName +
        ". Here is your examination dashboard.";

    createDashStatic(
        hwnd, "Student Dashboard",
        left, 38, contentW - 260, 45,
        dashTitleFont);

    createDashStatic(
        hwnd, welcome,
        left, 82, contentW - 260, 30,
        dashNormalFont);

    dashMenuButtons.push_back(
        createDashButton(hwnd, "LOGOUT",
                         width - 155, 48, 110, 40,
                         ID_DASH_LOGOUT, dashNormalFont));

    int cardGap = 22;
    int cardW = (contentW - cardGap * 2) / 3;

    // Real data will be displayed here after the Test/Attempt/Result
    // classes are integrated. Do not show fake statistics.
    addDashCard(
        hwnd, left, 145, cardW, 135,
        "Available Quizzes", "--", "No quiz data yet");

    addDashCard(
        hwnd, left + cardW + cardGap, 145, cardW, 135,
        "Previous Attempts", "--", "No attempt data yet");

    addDashCard(
        hwnd, left + (cardW + cardGap) * 2, 145, cardW, 135,
        "Examination Results", "--", "No result data yet");

    addDashSectionTitle(
        hwnd, "Available Quizzes", left, 315, contentW);

    int listY = 360;
    int listH = 105;
    int listW = (contentW * 2) / 3 - 12;

    addDashCard(
        hwnd, left, listY, listW, listH,
        "Available Quizzes", "No quizzes available",
        "Published quizzes will appear here when real Test data exists.");

    addDashCard(
        hwnd, left, listY + listH + 15, listW, listH,
        "Quiz Registration", "No registration data",
        "Quiz registration information will appear here when available.");

    addDashSectionTitle(
        hwnd, "Recent Examination Results",
        left + listW + 25, 315, contentW - listW - 25);

    addDashCard(
        hwnd, left + listW + 25, listY,
        contentW - listW - 25, 105,
        "Recent Results", "No results available",
        "Results will appear here after an examination is submitted.");

    addDashCard(
        hwnd, left + listW + 25, listY + 120,
        contentW - listW - 25, 105,
        "Previous Attempts", "No attempts available",
        "Previous attempts will appear here when real data exists.");
}

void buildTeacherDashboard(HWND hwnd, int width, int height)
{
    const int sidebar = 235;
    const int left = sidebar + 45;
    const int contentW = width - left - 45;

    createDashStatic(hwnd, "QuizExam", 30, 25, 175, 45, dashHeaderFont);
    createDashStatic(hwnd, "TEACHER", 30, 67, 175, 22, dashSmallFont);

    int y = 125;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "Dashboard", 25, y, 185, 46, ID_DASH_BASE + 0, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "Course & Topic Management", 25, y, 185, 46,
        ID_DASH_BASE + 1, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "Question Bank", 25, y, 185, 46,
        ID_DASH_BASE + 2, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "Test Management", 25, y, 185, 46,
        ID_DASH_BASE + 3, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "Results & Statistics", 25, y, 185, 46,
        ID_DASH_BASE + 4, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "Student Registrations", 25, y, 185, 46,
        ID_DASH_BASE + 5, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "Quiz Attempts", 25, y, 185, 46,
        ID_DASH_BASE + 6, dashNormalFont));

    createDashStatic(hwnd,
                     "Signed in as\n" + currentFullName,
                     30, height - 100, 175, 55, dashSmallFont);

    createDashStatic(hwnd, "Teacher", 30, height - 45, 175, 25, dashSmallFont);

    createDashStatic(hwnd, "Teacher Dashboard",
                     left, 38, contentW - 150, 45, dashTitleFont);

    createDashStatic(hwnd,
                     "Create, manage and monitor quizzes, questions and student results.",
                     left, 82, contentW - 150, 30, dashNormalFont);

    dashMenuButtons.push_back(createDashButton(
        hwnd, "LOGOUT", width - 155, 48, 110, 40,
        ID_DASH_LOGOUT, dashNormalFont));

    int cardGap = 22;
    int cardW = (contentW - cardGap * 2) / 3;

    addDashCard(hwnd, left, 145, cardW, 135,
                "Courses", "--", "No course data yet");

    addDashCard(hwnd, left + cardW + cardGap, 145, cardW, 135,
                "Question Bank", "--", "No question data yet");

    addDashCard(hwnd, left + (cardW + cardGap) * 2, 145, cardW, 135,
                "Published Tests", "--", "No test data yet");

    addDashSectionTitle(hwnd, "Quiz Management", left, 315, contentW);

    int listY = 360;
    int listH = 105;
    int listW = (contentW * 2) / 3 - 12;

    addDashCard(hwnd, left, listY, listW, listH,
                "Test Management", "Create / Edit / Publish Tests",
                "Manage title, subject, time, questions and publication status.");

    addDashCard(hwnd, left, listY + listH + 15, listW, listH,
                "Question Bank", "Manage Questions & Answer Options",
                "Create, edit, delete, search and categorize questions.");

    addDashSectionTitle(hwnd, "Results & Statistics",
                        left + listW + 25, 315, contentW - listW - 25);

    addDashCard(hwnd, left + listW + 25, listY,
                contentW - listW - 25, 105,
                "Average Score", "--",
                "No result data yet");

    addDashCard(hwnd, left + listW + 25, listY + 120,
                contentW - listW - 25, 105,
                "Participation", "--",
                "No attempt data yet");
}

void buildAdminDashboard(HWND hwnd, int width, int height)
{
    const int sidebar = 235;
    const int left = sidebar + 45;
    const int contentW = width - left - 45;

    createDashStatic(hwnd, "QuizExam", 30, 25, 175, 45, dashHeaderFont);
    createDashStatic(hwnd, "ADMINISTRATOR", 30, 67, 175, 22, dashSmallFont);

    int y = 125;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "Dashboard", 25, y, 185, 46, ID_DASH_BASE + 0, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "User Management", 25, y, 185, 46,
        ID_DASH_BASE + 1, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "Student Information", 25, y, 185, 46,
        ID_DASH_BASE + 2, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "Teacher Information", 25, y, 185, 46,
        ID_DASH_BASE + 3, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "Quiz Monitoring", 25, y, 185, 46,
        ID_DASH_BASE + 4, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "Reports", 25, y, 185, 46,
        ID_DASH_BASE + 5, dashNormalFont));
    y += 55;

    dashMenuButtons.push_back(createDashButton(
        hwnd, "System Logs", 25, y, 185, 46,
        ID_DASH_BASE + 6, dashNormalFont));

    createDashStatic(hwnd,
                     "Signed in as\n" + currentFullName,
                     30, height - 100, 175, 55, dashSmallFont);

    createDashStatic(hwnd, "Administrator", 30, height - 45, 175, 25, dashSmallFont);

    createDashStatic(hwnd, "Administrator Dashboard",
                     left, 38, contentW - 150, 45, dashTitleFont);

    createDashStatic(hwnd,
                     "Centralized management and monitoring of the Quiz Examination System.",
                     left, 82, contentW - 150, 30, dashNormalFont);

    dashMenuButtons.push_back(createDashButton(
        hwnd, "LOGOUT", width - 155, 48, 110, 40,
        ID_DASH_LOGOUT, dashNormalFont));

    int cardGap = 22;
    int cardW = (contentW - cardGap * 2) / 3;

    addDashCard(hwnd, left, 145, cardW, 135,
                "Users", "--", "No user data yet");

    addDashCard(hwnd, left + cardW + cardGap, 145, cardW, 135,
                "Quizzes", "--", "No test data yet");

    addDashCard(hwnd, left + (cardW + cardGap) * 2, 145, cardW, 135,
                "Activities", "--", "No activity data yet");

    addDashSectionTitle(hwnd, "System Monitoring", left, 315, contentW);

    int listY = 360;
    int listH = 105;
    int listW = (contentW * 2) / 3 - 12;

    addDashCard(hwnd, left, listY, listW, listH,
                "User Account Management", "Students & Teachers",
                "Add, edit, delete, lock/unlock accounts and manage permissions.");

    addDashCard(hwnd, left, listY + listH + 15, listW, listH,
                "Quiz System Monitoring", "Tests & Examination Data",
                "Monitor quiz status and manage examination data.");

    addDashSectionTitle(hwnd, "Reports & Logs",
                        left + listW + 25, 315, contentW - listW - 25);

    addDashCard(hwnd, left + listW + 25, listY,
                contentW - listW - 25, 105,
                "Reports", "No report data",
                "Reports will appear here when examination data exists.");

    addDashCard(hwnd, left + listW + 25, listY + 120,
                contentW - listW - 25, 105,
                "System Logs", "No log data",
                "System activity logs will appear here when available.");
}

LRESULT CALLBACK DashboardWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        dashTitleFont = CreateFontA(
            30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

        dashHeaderFont = CreateFontA(
            21, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

        dashNormalFont = CreateFontA(
            17, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

        dashSmallFont = CreateFontA(
            14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

        RECT rc;
        GetClientRect(hwnd, &rc);

        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;

        if (currentRole == "Student")
            buildStudentDashboard(hwnd, width, height);
        else if (currentRole == "Teacher")
            buildTeacherDashboard(hwnd, width, height);
        else if (currentRole == "Admin")
            buildAdminDashboard(hwnd, width, height);

        return 0;
    }

    case WM_COMMAND:
    {
        int id = LOWORD(wParam);

        if (id == ID_DASH_LOGOUT)
        {
            DestroyWindow(hwnd);
            return 0;
        }

        if (id == ID_DASH_PROFILE)
        {
            showMessage(
                hwnd,
                "Personal Information",
                "Current account:\n\n"
                "Username: " +
                    currentUsername +
                    "\nFull name: " + currentFullName +
                    "\nRole: " + currentRole);
            return 0;
        }

        if (id >= ID_DASH_BASE && id < ID_DASH_BASE + 20)
        {
            int index = id - ID_DASH_BASE;

            if (currentRole == "Student")
            {
                switch (index)
                {
                case 0:
                    break;

                case 1:
                    showMessage(hwnd, "Available Quizzes",
                                "This screen will display assigned and published quizzes with:\n"
                                "- Quiz title\n"
                                "- Description\n"
                                "- Time limit\n"
                                "- Total marks\n"
                                "- Publication status");
                    break;

                case 2:
                    showMessage(hwnd, "Quiz Registration",
                                "Students can register for an available quiz and view registration status.");
                    break;

                case 3:
                    showMessage(hwnd, "Quiz Examinations",
                                "The examination screen will display questions, answer areas and remaining time.");
                    break;

                case 4:
                    showMessage(hwnd, "Previous Attempts",
                                "Previous quiz attempts will be displayed here.");
                    break;

                case 5:
                    showMessage(hwnd, "Examination Results",
                                "Results will show total marks, obtained marks, percentage, grade and completion time.");
                    break;
                }
            }
            else if (currentRole == "Teacher")
            {
                switch (index)
                {
                case 1:
                    showMessage(hwnd, "Course & Topic Management",
                                "Create, edit and delete courses and topics.");
                    break;

                case 2:
                    showMessage(hwnd, "Question Bank",
                                "Create, edit, delete, search and categorize questions and answer options.");
                    break;

                case 3:

                    openTestGui(hwnd);
                    break;

                case 4:
                    showMessage(hwnd, "Results & Statistics",
                                "View student scores, participation, highest, lowest, average, pass and fail statistics.");
                    break;

                case 5:
                    showMessage(hwnd, "Student Registrations",
                                "View students registered for teacher quizzes.");
                    break;

                case 6:
                    showMessage(hwnd, "Quiz Attempts",
                                "View student quiz attempts and submission information.");
                    break;
                }
            }
            else if (currentRole == "Admin")
            {
                switch (index)
                {
                case 1:
                    showMessage(hwnd, "User Management",
                                "Add, edit, delete, lock/unlock accounts and manage permissions.");
                    break;

                case 2:
                    showMessage(hwnd, "Student Information",
                                "View and manage student information.");
                    break;

                case 3:
                    showMessage(hwnd, "Teacher Information",
                                "View and manage teacher information.");
                    break;

                case 4:
                    showMessage(hwnd, "Quiz Monitoring",
                                "Monitor quiz status and examination activity.");
                    break;

                case 5:
                    showMessage(hwnd, "Reports",
                                "View reports related to examination activities and system usage.");
                    break;

                case 6:
                    showMessage(hwnd, "System Logs",
                                "View user, action, execution time and related details.");
                    break;
                }
            }

            return 0;
        }

        return 0;
    }

    case WM_GETMINMAXINFO:
    {
        MINMAXINFO *info = (MINMAXINFO *)lParam;
        info->ptMinTrackSize.x = 1050;
        info->ptMinTrackSize.y = 650;
        return 0;
    }

    case WM_ERASEBKGND:
        return 1;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rc;
        GetClientRect(hwnd, &rc);

        HBRUSH bgBrush = CreateSolidBrush(DASH_BG);
        FillRect(hdc, &rc, bgBrush);
        DeleteObject(bgBrush);

        RECT sideRect = {0, 0, 235, rc.bottom};
        HBRUSH sideBrush = CreateSolidBrush(DASH_SIDEBAR);
        FillRect(hdc, &sideRect, sideBrush);
        DeleteObject(sideBrush);

        HPEN pen = CreatePen(PS_SOLID, 1, DASH_BORDER);
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        MoveToEx(hdc, 235, 0, NULL);
        LineTo(hdc, 235, rc.bottom);
        SelectObject(hdc, oldPen);
        DeleteObject(pen);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
    {
        clearDashboardControls();

        if (dashTitleFont)
        {
            DeleteObject(dashTitleFont);
            dashTitleFont = NULL;
        }

        if (dashHeaderFont)
        {
            DeleteObject(dashHeaderFont);
            dashHeaderFont = NULL;
        }

        if (dashNormalFont)
        {
            DeleteObject(dashNormalFont);
            dashNormalFont = NULL;
        }

        if (dashSmallFont)
        {
            DeleteObject(dashSmallFont);
            dashSmallFont = NULL;
        }

        dashboardWindow = NULL;
        ShowWindow(GetParent(hwnd), SW_SHOW);
        SetForegroundWindow(GetParent(hwnd));
        return 0;
    }
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

void openDashboard(HWND parent, const string &role, const string &username, const string &fullName)
{
    currentRole = role;
    currentUsername = username;
    currentFullName = fullName;

    static bool registered = false;
    const char CLASS_NAME[] = "QuizRoleDashboard";

    if (!registered)
    {
        WNDCLASSA wc = {};
        wc.lpfnWndProc = DashboardWindowProc;
        wc.hInstance = GetModuleHandleA(NULL);
        wc.lpszClassName = CLASS_NAME;
        wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        RegisterClassA(&wc);
        registered = true;
    }

    dashboardWindow = CreateWindowExA(
        0,
        CLASS_NAME,
        "Quiz Examination System",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1250, 760,
        parent, NULL, GetModuleHandleA(NULL), NULL);

    if (!dashboardWindow)
        return;

    ShowWindow(parent, SW_HIDE);
    ShowWindow(dashboardWindow, SW_SHOW);
    UpdateWindow(dashboardWindow);
}

// =====================================================
// LOGIN
// =====================================================

void loginAccount(HWND hwnd)
{
    string username = getText(hUsernameBox);
    string password = getText(hPasswordBox);

    if (username.empty() || password.empty())
    {
        showMessage(hwnd, "Login", "Please enter username/email and password.");
        return;
    }

    ifstream file("data/users.txt");

    if (!file.is_open())
    {
        showMessage(hwnd, "Error", "Cannot open data/users.txt");
        return;
    }

    string line;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string id, dbUsername, dbPassword, email;
        string fullName, dateOfBirth, status, role;

        getline(ss, id, '|');
        getline(ss, dbUsername, '|');
        getline(ss, dbPassword, '|');
        getline(ss, email, '|');
        getline(ss, fullName, '|');
        getline(ss, dateOfBirth, '|');
        getline(ss, status, '|');
        getline(ss, role, '|');

        if ((username == dbUsername || username == email) &&
            password == dbPassword)
        {
            file.close();

            if (status != "Active")
            {
                showMessage(hwnd, "Login", "This account is not active.");
                return;
            }

            // Login thành công -> lưu thông tin phiên đăng nhập
            // và chuyển thẳng tới Dashboard theo Role.
            openDashboard(hwnd, role, dbUsername, fullName);
            return;
        }
    }

    file.close();

    showMessage(hwnd, "Login", "Invalid username/email or password.");
}

// =====================================================
// REGISTER
// =====================================================

void registerAccount(HWND hwnd)
{
    vector<string> data = showForm(
        hwnd,
        "Register Account",
        {"Full name:",
         "Email:",
         "Username:",
         "Password:",
         "Date of birth:"},
        {},
        {false, false, false, true, false});

    if (data.size() != 5)
        return;

    string fullName = data[0];
    string email = data[1];
    string username = data[2];
    string password = data[3];
    string dateOfBirth = data[4];

    for (const string &value : data)
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

    ifstream checkFile("data/users.txt");

    if (!checkFile.is_open())
    {
        showMessage(
            hwnd,
            "Error",
            "Cannot open data/users.txt");
        return;
    }

    string line;

    while (getline(checkFile, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string id, dbUsername, dbPassword, dbEmail;

        getline(ss, id, '|');
        getline(ss, dbUsername, '|');
        getline(ss, dbPassword, '|');
        getline(ss, dbEmail, '|');

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

    ofstream file("data/users.txt", ios::app);

    if (!file.is_open())
    {
        showMessage(
            hwnd,
            "Error",
            "Cannot open data/users.txt");
        return;
    }

    static int nextID = 100;

    file << "S" << nextID++
         << "|" << username
         << "|" << password
         << "|" << email
         << "|" << fullName
         << "|" << dateOfBirth
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
    string username = getText(hUsernameBox);

    if (username.empty())
    {
        showMessage(
            hwnd,
            "Update Profile",
            "Please enter username/email first.");
        return;
    }

    vector<string> data = showForm(
        hwnd,
        "Update Profile",
        {"New full name:",
         "New email:",
         "New date of birth:"});

    if (data.size() != 3)
        return;

    if (data[0].empty() || data[1].empty() || data[2].empty())
    {
        showMessage(
            hwnd,
            "Update Profile",
            "Please fill in all fields.");
        return;
    }

    string fullName = data[0];
    string email = data[1];
    string dateOfBirth = data[2];

    ifstream input("data/users.txt");

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

    while (getline(input, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string id, dbUsername, password, dbEmail;
        string oldFullName, oldDateOfBirth, status, role;

        getline(ss, id, '|');
        getline(ss, dbUsername, '|');
        getline(ss, password, '|');
        getline(ss, dbEmail, '|');
        getline(ss, oldFullName, '|');
        getline(ss, oldDateOfBirth, '|');
        getline(ss, status, '|');
        getline(ss, role, '|');

        if (dbUsername == username || dbEmail == username)
        {
            oldFullName = fullName;
            dbEmail = email;
            oldDateOfBirth = dateOfBirth;
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

    ofstream output("data/users.txt");

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
    string username = getText(hUsernameBox);

    if (username.empty())
    {
        showMessage(
            hwnd,
            "Change Password",
            "Please enter username/email first.");
        return;
    }

    vector<string> data = showForm(
        hwnd,
        "Change Password",
        {"Current password:",
         "New password:"},
        {},
        {true, true});

    if (data.size() != 2)
        return;

    string oldPassword = data[0];
    string newPassword = data[1];

    if (oldPassword.empty() || newPassword.empty())
    {
        showMessage(
            hwnd,
            "Change Password",
            "Please fill in all fields.");
        return;
    }

    ifstream input("data/users.txt");

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

    while (getline(input, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string id, dbUsername, password, email;
        string fullName, dateOfBirth, status, role;

        getline(ss, id, '|');
        getline(ss, dbUsername, '|');
        getline(ss, password, '|');
        getline(ss, email, '|');
        getline(ss, fullName, '|');
        getline(ss, dateOfBirth, '|');
        getline(ss, status, '|');
        getline(ss, role, '|');

        if ((dbUsername == username || email == username) &&
            password == oldPassword)
        {
            password = newPassword;
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

    ofstream output("data/users.txt");

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
    vector<string> data = showForm(
        hwnd,
        "Recover Password",
        {"Registered email:"});

    if (data.size() != 1 || data[0].empty())
        return;

    string email = data[0];

    ifstream file("data/users.txt");

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

        string id, username, password, dbEmail;
        string fullName, dateOfBirth, status, role;

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
                "\nPassword: " + password;

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
    // Không cho giao diện bị méo khi cửa sổ quá nhỏ

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

    // Ép Windows vẽ lại toàn bộ giao diện

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

        info->ptMinTrackSize.x = 700;
        info->ptMinTrackSize.y = 550;

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
        return 0;

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