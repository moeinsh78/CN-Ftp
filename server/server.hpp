#ifndef _SERVER_
#define _SERVER_

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
// #include <json/value.h>
#include <fstream>


#include "user.hpp"

#define SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256

class Server {
public:
    Server();
    void read_config_file(std::string config_file_path);
    void start();
private:
    std::vector<User> users;
    std::vector<std::string> system_files;
    bool is_admin;
};

#endif