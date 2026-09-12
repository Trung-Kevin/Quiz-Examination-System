#include "AdminGui.h"
#include "Admin.h"

#include <windows.h>
#include <string>
#include <vector>
#include <sstream>

// =====================================================
// DATA STRUCTURES
// =====================================================

struct AdminUser
{
    int id;
    std::string username;
    std::string role;
    std::string status;
};

struct AdminTest
{
    int id;
    std::string title;
    std::string status;
};

// =====================================================
// GLOBAL VARIABLES
// =====================================================

static HWND hAdminWindow = nullptr;

static HWND hUserTitle = nullptr;
static HWND hUsernameLabel = nullptr;
static HWND hRoleLabel = nullptr;
static HWND hUsername = nullptr;
static HWND hRole = nullptr;

static HWND hAddButton = nullptr;
static HWND hRemoveButton = nullptr;
static HWND hStatusButton = nullptr;

static HWND hUserList = nullptr;

static HWND hTestTitle = nullptr;
static HWND hTestList = nullptr;
static HWND hTestDataButton = nullptr;

static HWND hDataTitle = nullptr;
static HWND hDataInfo = nullptr;
static HWND hRefreshDataButton = nullptr;
static HWND hPermissionButton = nullptr;

// =====================================================
// SAMPLE DATA
// =====================================================

static std::vector<AdminUser> users =
    {
        {1, "student01", "Student", "Active"},
        {2, "teacher01", "Teacher", "Active"},
        {3, "admin01", "Admin", "Active"}};

static std::vector<AdminTest> tests =
    {
        {1, "Midterm Programming Test", "Published"},
        {2, "Final C++ Examination", "Draft"}};

// =====================================================
// HELPER
// =====================================================

static void setFont(HWND control, HFONT font)
{
    if (control)
        SendMessageA(control, WM_SETFONT, (WPARAM)font, TRUE);
}

static HWND createStatic(
    const char *text,
    DWORD style,
    int x, int y, int w, int h,
    HWND parent,
    HFONT font)
{
    HWND control = CreateWindowA(
        "STATIC",
        text,
        WS_CHILD | WS_VISIBLE | style,
        x, y, w, h,
        parent,
        nullptr,
        GetModuleHandleA(nullptr),
        nullptr);

    setFont(control, font);
    return control;
}

static HWND createButton(
    const char *text,
    int id,
    int x, int y, int w, int h,
    HWND parent,
    HFONT font)
{
    HWND control = CreateWindowA(
        "BUTTON",
        text,
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        x, y, w, h,
        parent,
        (HMENU)(INT_PTR)id,
        GetModuleHandleA(nullptr),
        nullptr);

    setFont(control, font);
    return control;
}

static HWND createListBox(
    int id,
    int x, int y, int w, int h,
    HWND parent,
    HFONT font)
{
    HWND control = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD |
            WS_VISIBLE |
            LBS_NOTIFY |
            WS_VSCROLL |
            LBS_NOINTEGRALHEIGHT,
        x, y, w, h,
        parent,
        (HMENU)(INT_PTR)id,
        GetModuleHandleA(nullptr),
        nullptr);

    setFont(control, font);
    return control;
}

static HWND createEdit(
    const char *text,
    int id,
    int x, int y, int w, int h,
    HWND parent,
    HFONT font)
{
    HWND control = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        text,
        WS_CHILD |
            WS_VISIBLE |
            ES_AUTOHSCROLL,
        x, y, w, h,
        parent,
        (HMENU)(INT_PTR)id,
        GetModuleHandleA(nullptr),
        nullptr);

    setFont(control, font);
    return control;
}

// =====================================================
// REFRESH USER LIST
// =====================================================

static void refreshUserList()
{
    if (!hUserList)
        return;

    SendMessageA(hUserList, LB_RESETCONTENT, 0, 0);

    for (const auto &user : users)
    {
        std::stringstream ss;

        ss << user.id
           << " | "
           << user.username
           << " | Role: "
           << user.role
           << " | "
           << user.status;

        std::string text = ss.str();

        SendMessageA(
            hUserList,
            LB_ADDSTRING,
            0,
            (LPARAM)text.c_str());
    }
}

// =====================================================
// REFRESH TEST LIST
// =====================================================

static void refreshTestList()
{
    if (!hTestList)
        return;

    SendMessageA(hTestList, LB_RESETCONTENT, 0, 0);

    for (const auto &test : tests)
    {
        std::stringstream ss;

        ss << test.id
           << " | "
           << test.title
           << " | "
           << test.status;

        std::string text = ss.str();

        SendMessageA(
            hTestList,
            LB_ADDSTRING,
            0,
            (LPARAM)text.c_str());
    }
}

// =====================================================
// GET TEXT
// =====================================================

static std::string getText(HWND hwnd)
{
    if (!hwnd)
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

// =====================================================
// ADD USER
// =====================================================

static void addUser()
{
    std::string username = getText(hUsername);
    std::string role = getText(hRole);

    if (username.empty())
    {
        MessageBoxA(
            hAdminWindow,
            "Please enter username.",
            "Admin",
            MB_OK | MB_ICONWARNING);
        return;
    }

    if (role.empty())
        role = "Student";

    for (const auto &user : users)
    {
        if (user.username == username)
        {
            MessageBoxA(
                hAdminWindow,
                "Username already exists.",
                "Admin",
                MB_OK | MB_ICONWARNING);
            return;
        }
    }

    int newID = 1;

    for (const auto &user : users)
    {
        if (user.id >= newID)
            newID = user.id + 1;
    }

    users.push_back(
        {newID, username, role, "Active"});

    refreshUserList();

    SetWindowTextA(hUsername, "");
    SetWindowTextA(hRole, "");

    MessageBoxA(
        hAdminWindow,
        "User added successfully.",
        "Admin",
        MB_OK | MB_ICONINFORMATION);
}

// =====================================================
// REMOVE USER
// =====================================================

static void removeUser()
{
    if (!hUserList)
        return;

    int index = (int)SendMessageA(
        hUserList,
        LB_GETCURSEL,
        0,
        0);

    if (index == LB_ERR)
    {
        MessageBoxA(
            hAdminWindow,
            "Please select a user.",
            "Admin",
            MB_OK | MB_ICONWARNING);
        return;
    }

    if (index >= 0 && index < (int)users.size())
    {
        std::string username = users[index].username;

        if (users[index].role == "Admin")
        {
            MessageBoxA(
                hAdminWindow,
                "The Admin account cannot be removed.",
                "Admin",
                MB_OK | MB_ICONWARNING);
            return;
        }

        users.erase(users.begin() + index);

        refreshUserList();

        std::string message =
            "User \"" + username + "\" has been removed.";

        MessageBoxA(
            hAdminWindow,
            message.c_str(),
            "Admin",
            MB_OK | MB_ICONINFORMATION);
    }
}

// =====================================================
// TOGGLE USER STATUS
// =====================================================

static void toggleUserStatus()
{
    if (!hUserList)
        return;

    int index = (int)SendMessageA(
        hUserList,
        LB_GETCURSEL,
        0,
        0);

    if (index == LB_ERR)
    {
        MessageBoxA(
            hAdminWindow,
            "Please select a user.",
            "Admin",
            MB_OK | MB_ICONWARNING);
        return;
    }

    if (index >= 0 && index < (int)users.size())
    {
        if (users[index].role == "Admin")
        {
            MessageBoxA(
                hAdminWindow,
                "The Admin account status cannot be disabled.",
                "Admin",
                MB_OK | MB_ICONWARNING);
            return;
        }

        if (users[index].status == "Active")
            users[index].status = "Inactive";
        else
            users[index].status = "Active";

        refreshUserList();
    }
}

// =====================================================
// VIEW TEST DATA
// =====================================================

static void viewTestData()
{
    std::stringstream ss;

    ss << "Total tests: "
       << tests.size()
       << "\r\n\r\n";

    for (const auto &test : tests)
    {
        ss << "Test ID: "
           << test.id
           << "\r\n";

        ss << "Title: "
           << test.title
           << "\r\n";

        ss << "Status: "
           << test.status
           << "\r\n";

        ss << "-------------------------\r\n";
    }

    MessageBoxA(
        hAdminWindow,
        ss.str().c_str(),
        "Test Data",
        MB_OK | MB_ICONINFORMATION);
}

// =====================================================
// SHOW DATA MANAGEMENT
// =====================================================

static void showDataManagement()
{
    if (!hDataInfo)
        return;

    std::stringstream ss;

    ss << "SYSTEM DATA\r\n\r\n";
    ss << "Users: " << users.size() << "\r\n";
    ss << "Tests: " << tests.size() << "\r\n";
    ss << "Question data: Available\r\n";
    ss << "Course data: Available\r\n";
    ss << "Topic data: Available\r\n";

    SetWindowTextA(
        hDataInfo,
        ss.str().c_str());
}

// =====================================================
// SHOW PERMISSIONS
// =====================================================

static void showPermissions()
{
    MessageBoxA(
        hAdminWindow,

        "ADMIN PERMISSIONS\r\n\r\n"
        "Admin:\r\n"
        "- Manage users\r\n"
        "- Manage tests\r\n"
        "- Manage system data\r\n"
        "- Manage permissions\r\n\r\n"
        "Teacher:\r\n"
        "- Manage courses\r\n"
        "- Manage questions\r\n"
        "- Manage tests\r\n"
        "- View results\r\n\r\n"
        "Student:\r\n"
        "- View subjects\r\n"
        "- View tests\r\n"
        "- Take tests\r\n"
        "- View results",

        "Permissions",
        MB_OK | MB_ICONINFORMATION);
}

// =====================================================
// RESIZE ADMIN GUI
// =====================================================

static void resizeAdminGui(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    const int margin = 24;
    const int gap = 20;

    // -------------------------------------------------
    // Two main columns
    // -------------------------------------------------

    int contentWidth = width - margin * 2;
    int leftWidth = (contentWidth - gap) / 2;
    int rightWidth = contentWidth - gap - leftWidth;

    if (leftWidth < 380)
        leftWidth = 380;

    if (rightWidth < 380)
        rightWidth = 380;

    int rightX = margin + leftWidth + gap;

    // -------------------------------------------------
    // USER MANAGEMENT
    // -------------------------------------------------

    SetWindowPos(
        hUserTitle,
        nullptr,
        margin,
        18,
        leftWidth,
        28,
        SWP_NOZORDER);

    int labelWidth = 75;
    int editX = margin + labelWidth + 8;
    int editWidth = leftWidth - labelWidth - 8;

    SetWindowPos(
        hUsernameLabel,
        nullptr,
        margin,
        58,
        labelWidth,
        26,
        SWP_NOZORDER);

    SetWindowPos(
        hUsername,
        nullptr,
        editX,
        56,
        editWidth,
        28,
        SWP_NOZORDER);

    SetWindowPos(
        hRoleLabel,
        nullptr,
        margin,
        93,
        labelWidth,
        26,
        SWP_NOZORDER);

    SetWindowPos(
        hRole,
        nullptr,
        editX,
        91,
        editWidth,
        28,
        SWP_NOZORDER);

    int buttonY = 130;
    int buttonGap = 8;
    int buttonWidth =
        (leftWidth - buttonGap * 2) / 3;

    SetWindowPos(
        hAddButton,
        nullptr,
        margin,
        buttonY,
        buttonWidth,
        32,
        SWP_NOZORDER);

    SetWindowPos(
        hRemoveButton,
        nullptr,
        margin + buttonWidth + buttonGap,
        buttonY,
        buttonWidth,
        32,
        SWP_NOZORDER);

    SetWindowPos(
        hStatusButton,
        nullptr,
        margin + (buttonWidth + buttonGap) * 2,
        buttonY,
        buttonWidth,
        32,
        SWP_NOZORDER);

    int listTop = 175;
    int bottomSpace = 30;
    int userListHeight =
        height - listTop - bottomSpace;

    if (userListHeight < 180)
        userListHeight = 180;

    SetWindowPos(
        hUserList,
        nullptr,
        margin,
        listTop,
        leftWidth,
        userListHeight,
        SWP_NOZORDER);

    // -------------------------------------------------
    // TEST MANAGEMENT
    // -------------------------------------------------

    SetWindowPos(
        hTestTitle,
        nullptr,
        rightX,
        18,
        rightWidth,
        28,
        SWP_NOZORDER);

    int testListTop = 56;
    int testButtonHeight = 32;
    int testButtonY =
        height - margin - testButtonHeight;

    int testListHeight =
        testButtonY - testListTop - 12;

    if (testListHeight < 180)
        testListHeight = 180;

    SetWindowPos(
        hTestList,
        nullptr,
        rightX,
        testListTop,
        rightWidth,
        testListHeight,
        SWP_NOZORDER);

    SetWindowPos(
        hTestDataButton,
        nullptr,
        rightX,
        testButtonY,
        145,
        testButtonHeight,
        SWP_NOZORDER);

    // -------------------------------------------------
    // DATA MANAGEMENT
    // -------------------------------------------------

    int dataTop = height - 235;

    if (dataTop < 430)
        dataTop = 430;

    SetWindowPos(
        hDataTitle,
        nullptr,
        margin,
        dataTop,
        width - margin * 2,
        28,
        SWP_NOZORDER);

    int dataInfoTop = dataTop + 35;
    int dataInfoHeight = 125;

    SetWindowPos(
        hDataInfo,
        nullptr,
        margin,
        dataInfoTop,
        width - margin * 2,
        dataInfoHeight,
        SWP_NOZORDER);

    int dataButtonY =
        dataInfoTop + dataInfoHeight + 10;

    SetWindowPos(
        hRefreshDataButton,
        nullptr,
        margin,
        dataButtonY,
        130,
        32,
        SWP_NOZORDER);

    SetWindowPos(
        hPermissionButton,
        nullptr,
        margin + 140,
        dataButtonY,
        160,
        32,
        SWP_NOZORDER);
}

// =====================================================
// ADMIN WINDOW PROCEDURE
// =====================================================

static LRESULT CALLBACK AdminWindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        hAdminWindow = hwnd;

        HFONT font =
            (HFONT)GetStockObject(DEFAULT_GUI_FONT);

        // -------------------------------------------------
        // USER MANAGEMENT
        // -------------------------------------------------

        hUserTitle = createStatic(
            "USER MANAGEMENT",
            SS_LEFT,
            24, 18, 300, 28,
            hwnd,
            font);

        hUsernameLabel = createStatic(
            "Username:",
            SS_CENTERIMAGE,
            24, 58, 75, 26,
            hwnd,
            font);

        hUsername = createEdit(
            "",
            201,
            107, 56, 250, 28,
            hwnd,
            font);

        hRoleLabel = createStatic(
            "Role:",
            SS_CENTERIMAGE,
            24, 93, 75, 26,
            hwnd,
            font);

        hRole = createEdit(
            "Student",
            202,
            107, 91, 250, 28,
            hwnd,
            font);

        hAddButton = createButton(
            "Add User",
            203,
            24, 130, 130, 32,
            hwnd,
            font);

        hRemoveButton = createButton(
            "Remove",
            204,
            162, 130, 130, 32,
            hwnd,
            font);

        hStatusButton = createButton(
            "Activate / Disable",
            205,
            300, 130, 150, 32,
            hwnd,
            font);

        hUserList = createListBox(
            206,
            24, 175, 500, 250,
            hwnd,
            font);

        // -------------------------------------------------
        // TEST MANAGEMENT
        // -------------------------------------------------

        hTestTitle = createStatic(
            "TEST MANAGEMENT",
            SS_LEFT,
            544, 18, 300, 28,
            hwnd,
            font);

        hTestList = createListBox(
            207,
            544, 56, 500, 330,
            hwnd,
            font);

        hTestDataButton = createButton(
            "View Test Data",
            208,
            544, 398, 145, 32,
            hwnd,
            font);

        // -------------------------------------------------
        // DATA MANAGEMENT
        // -------------------------------------------------

        hDataTitle = createStatic(
            "DATA MANAGEMENT",
            SS_LEFT,
            24, 450, 300, 28,
            hwnd,
            font);

        hDataInfo = CreateWindowExA(
            WS_EX_CLIENTEDGE,
            "EDIT",
            "",
            WS_CHILD |
                WS_VISIBLE |
                ES_MULTILINE |
                ES_READONLY |
                WS_VSCROLL,
            24, 485, 1020, 125,
            hwnd,
            nullptr,
            GetModuleHandleA(nullptr),
            nullptr);

        setFont(hDataInfo, font);

        hRefreshDataButton = createButton(
            "Refresh Data",
            210,
            24, 620, 130, 32,
            hwnd,
            font);

        hPermissionButton = createButton(
            "Manage Permissions",
            211,
            164, 620, 160, 32,
            hwnd,
            font);

        // -------------------------------------------------
        // INITIAL DATA
        // -------------------------------------------------

        refreshUserList();
        refreshTestList();
        showDataManagement();

        resizeAdminGui(hwnd);

        return 0;
    }

    case WM_COMMAND:
    {
        int id = LOWORD(wParam);

        switch (id)
        {
        case 203:
            addUser();
            break;

        case 204:
            removeUser();
            break;

        case 205:
            toggleUserStatus();
            break;

        case 208:
            viewTestData();
            break;

        case 210:
            showDataManagement();
            break;

        case 211:
            showPermissions();
            break;
        }

        return 0;
    }

    case WM_SIZE:
        resizeAdminGui(hwnd);
        return 0;

    case WM_GETMINMAXINFO:
    {
        MINMAXINFO *info =
            (MINMAXINFO *)lParam;

        info->ptMinTrackSize.x = 900;
        info->ptMinTrackSize.y = 650;

        return 0;
    }

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
    {
        hAdminWindow = nullptr;

        hUserTitle = nullptr;
        hUsernameLabel = nullptr;
        hRoleLabel = nullptr;
        hUsername = nullptr;
        hRole = nullptr;

        hAddButton = nullptr;
        hRemoveButton = nullptr;
        hStatusButton = nullptr;

        hUserList = nullptr;

        hTestTitle = nullptr;
        hTestList = nullptr;
        hTestDataButton = nullptr;

        hDataTitle = nullptr;
        hDataInfo = nullptr;
        hRefreshDataButton = nullptr;
        hPermissionButton = nullptr;

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
// OPEN ADMIN GUI
// =====================================================

void openAdminGui(HWND parent)
{
    const char *className =
        "AdminGuiWindow";

    static bool registered = false;

    if (!registered)
    {
        WNDCLASSA wc = {};

        wc.lpfnWndProc =
            AdminWindowProc;

        wc.hInstance =
            GetModuleHandleA(nullptr);

        wc.lpszClassName =
            className;

        wc.hCursor =
            LoadCursorA(nullptr, IDC_ARROW);

        wc.hbrBackground =
            (HBRUSH)(COLOR_WINDOW + 1);

        wc.style =
            CS_HREDRAW |
            CS_VREDRAW;

        if (!RegisterClassA(&wc))
        {
            DWORD error = GetLastError();

            if (error != ERROR_CLASS_ALREADY_EXISTS)
            {
                MessageBoxA(
                    parent,
                    "Cannot register Admin window class.",
                    "Error",
                    MB_OK | MB_ICONERROR);

                return;
            }
        }

        registered = true;
    }

    HWND hwnd = CreateWindowExA(
        0,
        className,
        "Quiz Examination System - Admin",
        WS_OVERLAPPEDWINDOW |
            WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1100,
        760,
        parent,
        nullptr,
        GetModuleHandleA(nullptr),
        nullptr);

    if (!hwnd)
    {
        MessageBoxA(
            parent,
            "Cannot open Admin window.",
            "Error",
            MB_OK | MB_ICONERROR);

        return;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
}