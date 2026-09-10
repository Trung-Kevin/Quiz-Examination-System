#ifndef USER_H
#define USER_H

#include <string>

class User
{
private:
    std::string userID;
    std::string username;
    std::string password;
    std::string email;
    std::string fullName;
    std::string dateOfBirth;
    std::string status;

public:
    bool login();
    void updateProfile();
    bool recoverPassword();
    bool changePassword();
};

#endif