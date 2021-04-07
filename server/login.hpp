#ifndef __LOGIN__
#define __LOGIN__

#include "user.hpp"
#include <string.h>
#include <vector>
#include <fstream>


class Login
{
public:
    Login();
    void find_username(std::vector<User> users,std::string username);
    bool login(std::string pass);
    bool quit();
    void record_log(std::string message);
    std::string get_logged_username();
private:
    std::ofstream logfile;
    User user;
    bool logged_in;
    bool found_flag;
};

#endif