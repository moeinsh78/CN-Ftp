#ifndef _USER_
#define _USER_

#include <iostream>
#include <string>

class User {
public:
    User();
    User(std::string username, std::string password, int remaining_download_size, std::string is_admin);
    std::string get_username();
    std::string get_password();
    int get_remaining_download_size();
    void reduce_download_size(int dif);
    bool admin();
private:
    std::string username;
    std::string password;
    int remaining_download_size;
    bool is_admin;
};

#endif