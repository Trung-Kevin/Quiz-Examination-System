#ifndef STUDENTGUI_H
#define STUDENTGUI_H

#include <windows.h>
#include <string>

void openStudentGUI(
    HWND parent,
    int studentId,
    const std::string &studentName);

#endif