#include "Admin.h"

Admin::Admin()
{
    adminID = "";
}

Admin::Admin(const std::string &id)
{
    adminID = id;
}

void Admin::manageUsers()
{
    // User management is handled by Admin GUI.
}

void Admin::manageTests()
{
    // Test management is handled by Admin GUI.
}

void Admin::manageData()
{
    // Data management is handled by Admin GUI.
}

void Admin::managePermissions()
{
    // Permission management is handled by Admin GUI.
}

std::string Admin::getAdminID() const
{
    return adminID;
}

void Admin::setAdminID(const std::string &id)
{
    adminID = id;
}