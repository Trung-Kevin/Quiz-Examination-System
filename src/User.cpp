#include "User.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

namespace
{
    struct Account
    {
        string userID;
        string username;
        string password;
        string email;
        string fullName;
        string dateOfBirth;
        string status;
        string role;
    };

    const string USER_FILE = "data/users.txt";

    vector<Account> loadAccounts()
    {
        vector<Account> accounts;

        ifstream file(USER_FILE);

        if (!file.is_open())
        {
            return accounts;
        }

        string line;

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            Account acc;

            getline(ss, acc.userID, '|');
            getline(ss, acc.username, '|');
            getline(ss, acc.password, '|');
            getline(ss, acc.email, '|');
            getline(ss, acc.fullName, '|');
            getline(ss, acc.dateOfBirth, '|');
            getline(ss, acc.status, '|');
            getline(ss, acc.role, '|');

            accounts.push_back(acc);
        }

        file.close();

        return accounts;
    }

    void saveAccounts(const vector<Account> &accounts)
    {
        ofstream file(USER_FILE);

        for (const Account &acc : accounts)
        {
            file << acc.userID << "|"
                 << acc.username << "|"
                 << acc.password << "|"
                 << acc.email << "|"
                 << acc.fullName << "|"
                 << acc.dateOfBirth << "|"
                 << acc.status << "|"
                 << acc.role << "\n";
        }

        file.close();
    }
}

bool User::login()
{
    string usernameOrEmail;
    string password;

    cout << "===== LOGIN =====\n";

    cout << "Username or Email: ";
    cin >> usernameOrEmail;

    cout << "Password: ";
    cin >> password;

    vector<Account> accounts = loadAccounts();

    for (const Account &acc : accounts)
    {
        if ((acc.username == usernameOrEmail ||
             acc.email == usernameOrEmail) &&
            acc.password == password)
        {
            if (acc.status != "Active")
            {
                cout << "Account is locked.\n";
                return false;
            }

            cout << "Login successful!\n";
            cout << "Role: " << acc.role << "\n";

            return true;
        }
    }

    cout << "Invalid username/email or password.\n";

    return false;
}

void User::updateProfile()
{
    string username;

    cout << "===== UPDATE PROFILE =====\n";

    cout << "Enter username: ";
    cin >> username;

    vector<Account> accounts = loadAccounts();

    for (Account &acc : accounts)
    {
        if (acc.username == username)
        {
            cin.ignore();

            cout << "Full name: ";
            getline(cin, acc.fullName);

            cout << "Email: ";
            getline(cin, acc.email);

            cout << "Date of birth: ";
            getline(cin, acc.dateOfBirth);

            saveAccounts(accounts);

            cout << "Profile updated successfully!\n";

            return;
        }
    }

    cout << "User not found.\n";
}

bool User::recoverPassword()
{
    string email;

    cout << "===== RECOVER PASSWORD =====\n";

    cout << "Enter registered email: ";
    cin >> email;

    vector<Account> accounts = loadAccounts();

    for (const Account &acc : accounts)
    {
        if (acc.email == email)
        {
            cout << "Account found.\n";
            cout << "Username: " << acc.username << "\n";
            cout << "Password recovery successful.\n";

            return true;
        }
    }

    cout << "Email not found.\n";

    return false;
}

bool User::changePassword()
{
    string username;
    string oldPassword;
    string newPassword;

    cout << "===== CHANGE PASSWORD =====\n";

    cout << "Username: ";
    cin >> username;

    cout << "Old password: ";
    cin >> oldPassword;

    vector<Account> accounts = loadAccounts();

    for (Account &acc : accounts)
    {
        if (acc.username == username &&
            acc.password == oldPassword)
        {
            cout << "New password: ";
            cin >> newPassword;

            acc.password = newPassword;

            saveAccounts(accounts);

            cout << "Password changed successfully!\n";

            return true;
        }
    }

    cout << "Username or old password is incorrect.\n";

    return false;
}