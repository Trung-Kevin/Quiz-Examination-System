#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
protected:
    string userID;
    string username;
    string password;
    string email;
    string fullName;
    string dateOfBirth;
    string status;

public:
    User();

    User(string userID,
         string username,
         string password,
         string email,
         string fullName,
         string dateOfBirth,
         string status);

    bool login(string username, string password);
    void updateProfile(string email,
                       string fullName,
                       string dateOfBirth);

    void recoverPassword();
    void changePassword(string newPassword);

    // Getter
    string getUserID();
    string getUsername();
    string getPassword();
    string getEmail();
    string getFullName();
    string getDateOfBirth();
    string getStatus();

    // Setter
    void setEmail(string email);
    void setFullName(string fullName);
    void setDateOfBirth(string dateOfBirth);
    void setPassword(string password);
};

#endif