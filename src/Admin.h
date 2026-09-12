#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User
{
private:
    std::string adminID;

public:
    Admin();
    Admin(const std::string &id);

    void manageUsers();
    void manageTests();
    void manageData();
    void managePermissions();

    std::string getAdminID() const;
    void setAdminID(const std::string &id);
};

#endif