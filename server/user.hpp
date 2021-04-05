#ifndef _USER_
#define _USER_

#include <iostream>
#include <string>

class User {
public:
    User(std::string username, std::string password, int remaining_download_size, bool is_admin);

private:
    std::string username;
    std::string password;
    int remaining_download_size;
    bool is_admin;
};

#endif