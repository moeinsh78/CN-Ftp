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
#include <fcntl.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "user.hpp"
#include "login.hpp"

#define SYNTAX_ERROR "501: Syntax error in parameters or arguments."
#define USER_LOGGEDIN "230: User logged in, proceed. Logged out if appropriate."
#define LOGIN_ERROR "332: Need account for login."
#define LIST_TRANS "226: List transfer done."
#define ERROR "500: Error."
#define CHANGE_SUCCESSFUL "250: Successful change."
#define DOWNLOAD_SUCCESSFUL "226: Successful Download."
#define OPEN_ERROR "425: Can't open data connection."
#define QUIT_SUCCESSFUL "221: Successful Quit."
#define FILE_UNAVAILABLE "550: File unavailable."

class CommandHandler
{
public:
    CommandHandler(std::string path,std::vector<std::string> files);
    void command_parser(std::string input);
    void handle(std::vector<User> users,int data_channel_fd);
    void record_log(std::string message);
    bool in_system_files(std::string file);
private:
    std::ofstream logfile;
    std::string cmd;
    std::vector<std::string> args;
    Login login;
    bool logged_in;
    std::string server_path;
    std::string client_directory;
    std::vector<std::string> system_files;
};

#endif