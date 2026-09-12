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

    bool login(
        const std::string &usernameOrEmail,
        const std::string &password);

    void updateProfile();

    bool updateProfile(
        const std::string &username,
        const std::string &fullName,
        const std::string &email,
        const std::string &dateOfBirth);

    bool recoverPassword();

    bool recoverPassword(
        const std::string &email,
        std::string &password);

    bool changePassword();

    bool changePassword(
        const std::string &username,
        const std::string &oldPassword,
        const std::string &newPassword);
};

#endif