#include "Student.h"

#include "Course.h"
#include "Test.h"
#include "TestAttempt.h"
#include "Result.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

bool Student::registerAccount()
{
    string fullName;
    string email;
    string username;
    string password;
    string dateOfBirth;

    cout << "===== REGISTER ACCOUNT =====\n";

    cin.ignore();

    cout << "Full name: ";
    getline(cin, fullName);

    cout << "Email: ";
    getline(cin, email);

    cout << "Username: ";
    getline(cin, username);

    cout << "Password: ";
    getline(cin, password);

    cout << "Date of birth: ";
    getline(cin, dateOfBirth);

    ifstream checkFile("data/users.txt");

    string line;

    while (getline(checkFile, line))
    {
        if (line.find("|" + email + "|") != string::npos)
        {
            cout << "Email already exists.\n";
            return false;
        }
    }

    checkFile.close();

    ofstream file("data/users.txt", ios::app);

    if (!file.is_open())
    {
        cout << "Cannot open user data file.\n";
        return false;
    }

    static int nextID = 1;

    file << "S" << nextID++ << "|"
         << username << "|"
         << password << "|"
         << email << "|"
         << fullName << "|"
         << dateOfBirth << "|"
         << "Active|"
         << "Student\n";

    file.close();

    cout << "Registration successful!\n";

    return true;
}

vector<Course> Student::viewSubject()
{
    cout << "===== COURSE LIST =====\n";
    cout << "Displaying available courses...\n";

    return {};
}

vector<Test> Student::viewTests()
{
    cout << "===== TEST LIST =====\n";
    cout << "Displaying available tests...\n";

    return {};
}

TestAttempt Student::takeTest()
{
    cout << "===== TAKE TEST =====\n";
    cout << "Starting test...\n";

    return TestAttempt();
}

vector<Result> Student::viewResults()
{
    cout << "===== RESULTS =====\n";
    cout << "Displaying student results...\n";

    return {};
}