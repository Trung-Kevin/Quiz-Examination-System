#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

class User
{
private:
    string userID;
    string username;
    string password;
    string email;
    string fullName;
    string dateOfBirth;
    string status;

public:
    bool login();
    void updateProfile();
    bool recoverPassword();
    bool changePassword();
};

#endif