#ifndef _USER_
#define _USER_

#include <iostream>
#include <string>

class User {
public:
    User(std::string username, std::string password, int remaining_download_size, bool is_admin);
    void get_attr();
    std::string get_username();
    std::string get_password();
private:
    std::string username;
    std::string password;
    int remaining_download_size;
    bool is_admin;
};

#endif