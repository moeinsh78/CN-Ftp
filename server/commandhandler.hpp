#ifndef __COMMAND_HANDLER__
#define __COMMAND_HANDLER__

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>

#include "user.hpp"
#include "login.hpp"

class CommandHandler
{
public:
    CommandHandler(std::string path);
    void command_parser(std::string input);
    void handle(std::vector<User> users,int data_channel_fd);
    void record_log(std::string message);
private:
    std::ofstream logfile;
    std::string cmd;
    std::vector<std::string> args;
    Login login;
    bool logged_in;
    std::string server_path;
    std::string client_directory;
};

#endif