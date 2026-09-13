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

// =====================================================
// REGISTER - CONSOLE VERSION
// =====================================================

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

    return registerAccount(
        fullName,
        email,
        username,
        password,
        dateOfBirth);
}

// =====================================================
// REGISTER - GUI VERSION
// =====================================================

bool Student::registerAccount(
    const string &fullName,
    const string &email,
    const string &username,
    const string &password,
    const string &dateOfBirth)
{
    ifstream checkFile(
        "data/users.txt");

    string line;

    while (getline(checkFile, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string userID;
        string oldUsername;
        string oldPassword;
        string oldEmail;

        getline(ss, userID, '|');
        getline(ss, oldUsername, '|');
        getline(ss, oldPassword, '|');
        getline(ss, oldEmail, '|');

        if (oldUsername == username ||
            oldEmail == email)
        {
            checkFile.close();

            return false;
        }
    }

    checkFile.close();

    // Find next student ID

    int nextID = 1;

    ifstream idFile(
        "data/users.txt");

    while (getline(idFile, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string id;

        getline(ss, id, '|');

        if (id.length() > 1 &&
            id[0] == 'S')
        {
            try
            {
                int number =
                    stoi(id.substr(1));

                if (number >= nextID)
                {
                    nextID = number + 1;
                }
            }
            catch (...)
            {
                // Ignore invalid ID
            }
        }
    }

    idFile.close();

    // Append new account

    ofstream file(
        "data/users.txt",
        ios::app);

    if (!file.is_open())
    {
        return false;
    }

    file << "S" << nextID << "|"
         << username << "|"
         << password << "|"
         << email << "|"
         << fullName << "|"
         << dateOfBirth << "|"
         << "Active|"
         << "Student\n";

    file.close();

    return true;
}

// =====================================================
// OTHER STUDENT FUNCTIONS
// =====================================================

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

    return TestAttempt(0, 0, 0);
}

vector<Result> Student::viewResults()
{
    cout << "===== RESULTS =====\n";
    cout << "Displaying student results...\n";

    return {};
}