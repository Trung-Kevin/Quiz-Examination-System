#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

class User
{
protected:
    int userId;
    string username;
    string password;
    string email;
    string fullName;
    string role;

public:
    User();
    User(int uId, string uname, string pwd, string mail, string name, string r = "Student");
    virtual ~User() = default;

    int getUserId() const;
    string getUsername() const;
    string getFullName() const;
    string getRole() const;

    void setRole(const string& newRole);
    void setFullName(const string& newName);
    void setEmail(const string& newEmail);

    bool login();
    void logout();
    void updateProfile();
    void displayInfo() const;
};

#endif