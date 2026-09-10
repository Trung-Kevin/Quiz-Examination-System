#include <iostream>
#include "User.h"
#include "Student.h"
#include "Teacher.h"

using namespace std;

int main() {
    // Test User
    User user(
        "U001",
        "kien",
        "123456",
        "kien@gmail.com",
        "Nguyen Kien",
        "01/01/2006",
        "Active"
    );

    cout << "=== TEST USER ===" << endl;

    if (user.login("kien", "123456")) {
        cout << "Login successful!" << endl;
    } else {
        cout << "Login failed!" << endl;
    }

    // Test Student
    Student student(
        "U002",
        "student01",
        "123456",
        "student@gmail.com",
        "Student Test",
        "02/02/2006",
        "Active",
        "SV001"
    );

    cout << "\n=== TEST STUDENT ===" << endl;
    cout << "Student ID: " << student.getStudentID() << endl;
    student.viewTests();
    student.viewResults();

    // Test Teacher
    Teacher teacher(
        "U003",
        "teacher01",
        "123456",
        "teacher@gmail.com",
        "Teacher Test",
        "03/03/1990",
        "Active",
        "GV001"
    );

    cout << "\n=== TEST TEACHER ===" << endl;
    cout << "Teacher ID: " << teacher.getTeacherID() << endl;
    teacher.manageCourse();
    teacher.manageQuestions();
    teacher.manageTests();

    return 0;
}