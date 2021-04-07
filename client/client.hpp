#ifndef _CLIENT_
#define _CLIENT_

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

#include "Json.hpp"

#define MAX_LINE 1024


class Client
{
public:
    Client();
    void read_config_file(std::string config_file_path);
    void start();
private:
    int command_channel_port;
    int data_channel_port;
};

#endif