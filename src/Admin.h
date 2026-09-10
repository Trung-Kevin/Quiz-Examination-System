#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include <string>

class Admin : public User
{
private:
    std::string adminID;

public:
    void manageUsers();
    void manageTests();
    void manageData();
    void managePermissions();
};

#endif