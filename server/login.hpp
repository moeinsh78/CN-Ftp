#ifndef __LOGIN__
#define __LOGIN__

#include "user.hpp"
#include <string.h>
#include <vector>

class Login
{
public:
    Login();
    void find_username(std::vector<User> users,std::string username);
    bool login(std::string pass);
private:
    User user;
    bool logged_in;
    bool found_flag;
};

#endif