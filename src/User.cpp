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

            if (!acc.username.empty())
            {
                accounts.push_back(acc);
            }
        }

        file.close();

        return accounts;
    }

    bool saveAccounts(const vector<Account> &accounts)
    {
        ofstream file(USER_FILE);

        if (!file.is_open())
        {
            return false;
        }

        for (const Account &acc : accounts)
        {
            file << acc.userID << "|"
                 << acc.username << "|"
                 << acc.password << "|"
                 << acc.email << "|"
                 << acc.fullName << "|"
                 << acc.dateOfBirth << "|"
                 << acc.status << "|"
                 << acc.role
                 << "\n";
        }

        file.close();

        return true;
    }

    bool accountExists(
        const vector<Account> &accounts,
        const string &username,
        const string &email)
    {
        for (const Account &acc : accounts)
        {
            if (acc.username == username ||
                acc.email == email)
            {
                return true;
            }
        }

        return false;
    }
}

// =====================================================
// LOGIN - CONSOLE VERSION
// =====================================================

bool User::login()
{
    string usernameOrEmail;
    string password;

    cout << "===== LOGIN =====\n";

    cout << "Username or Email: ";
    cin >> usernameOrEmail;

    cout << "Password: ";
    cin >> password;

    return login(
        usernameOrEmail,
        password);
}

// =====================================================
// LOGIN - GUI VERSION
// =====================================================

bool User::login(
    const string &usernameOrEmail,
    const string &password)
{
    vector<Account> accounts = loadAccounts();

    for (const Account &acc : accounts)
    {
        if ((acc.username == usernameOrEmail ||
             acc.email == usernameOrEmail) &&
            acc.password == password)
        {
            if (acc.status != "Active")
            {
                return false;
            }

            return true;
        }
    }

    return false;
}

// =====================================================
// UPDATE PROFILE - CONSOLE VERSION
// =====================================================

void User::updateProfile()
{
    string username;
    string fullName;
    string email;
    string dateOfBirth;

    cout << "===== UPDATE PROFILE =====\n";

    cout << "Username: ";
    cin >> username;

    cin.ignore();

    cout << "Full name: ";
    getline(cin, fullName);

    cout << "Email: ";
    getline(cin, email);

    cout << "Date of birth: ";
    getline(cin, dateOfBirth);

    if (updateProfile(
            username,
            fullName,
            email,
            dateOfBirth))
    {
        cout << "Profile updated successfully!\n";
    }
    else
    {
        cout << "Username not found.\n";
    }
}

// =====================================================
// UPDATE PROFILE - GUI VERSION
// =====================================================

bool User::updateProfile(
    const string &username,
    const string &fullName,
    const string &email,
    const string &dateOfBirth)
{
    vector<Account> accounts = loadAccounts();

    for (Account &acc : accounts)
    {
        if (acc.username == username)
        {
            // Check email used by another account

            for (const Account &other : accounts)
            {
                if (other.username != username &&
                    other.email == email)
                {
                    return false;
                }
            }

            acc.fullName = fullName;
            acc.email = email;
            acc.dateOfBirth = dateOfBirth;

            return saveAccounts(accounts);
        }
    }

    return false;
}

// =====================================================
// RECOVER PASSWORD - CONSOLE VERSION
// =====================================================

bool User::recoverPassword()
{
    string email;

    cout << "===== RECOVER PASSWORD =====\n";

    cout << "Enter registered email: ";
    cin >> email;

    string password;

    bool result = recoverPassword(
        email,
        password);

    if (result)
    {
        cout << "Account found.\n";
        cout << "Password: " << password << "\n";
    }
    else
    {
        cout << "Email not found.\n";
    }

    return result;
}

// =====================================================
// RECOVER PASSWORD - GUI VERSION
// =====================================================

bool User::recoverPassword(
    const string &email,
    string &password)
{
    vector<Account> accounts = loadAccounts();

    for (const Account &acc : accounts)
    {
        if (acc.email == email)
        {
            password = acc.password;

            return true;
        }
    }

    return false;
}

// =====================================================
// CHANGE PASSWORD - CONSOLE VERSION
// =====================================================

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

    cout << "New password: ";
    cin >> newPassword;

    bool result = changePassword(
        username,
        oldPassword,
        newPassword);

    if (result)
    {
        cout << "Password changed successfully!\n";
    }
    else
    {
        cout << "Username or old password is incorrect.\n";
    }

    return result;
}

// =====================================================
// CHANGE PASSWORD - GUI VERSION
// =====================================================

bool User::changePassword(
    const string &username,
    const string &oldPassword,
    const string &newPassword)
{
    vector<Account> accounts = loadAccounts();

    for (Account &acc : accounts)
    {
        if (acc.username == username &&
            acc.password == oldPassword)
        {
            acc.password = newPassword;

            return saveAccounts(accounts);
        }
    }

    return false;
}