#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include "User.h"

using namespace std;

class Admin : public User
{
private:
    string adminID;

public:
    void manageUsers();
    void manageTests();
    void manageData();
    void managePermissions();
};

#endif