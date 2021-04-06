#ifndef __COMMAND_HANDLER__
#define __COMMAND_HANDLER__

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include<stdio.h>
#include<unistd.h>

#include "user.hpp"
#include "login.hpp"

class CommandHandler
{
public:
    CommandHandler();
    void command_parser(std::string input);
    void handle(std::vector<User> users);
private:
    std::string cmd;
    std::vector<std::string> args;
    Login login;
    bool logged_in;
};

#endif