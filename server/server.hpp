#ifndef _SERVER_
#define _SERVER_

#include <iostream>
#include <vector>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <pthread.h>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#include "Json.hpp"
#include "commandhandler.hpp"
#include "user.hpp"

#define MAX_PENDING 5
#define MAX_LINE 1024

class Server 
{
public:
    Server();
    void read_config_file(std::string config_file_path);
    void start();
    void* connect(void* thread_number);
    void record_log(std::string message);

private:
    std::ofstream logfile;
    std::vector<User> users;
    std::vector<std::string> system_files;
    int command_channel_port;
    int data_channel_port;
};

#endif