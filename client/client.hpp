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

#define SERVER_DATA_PORT 2345
#define SERVER_COMMAND_PORT 5432
#define MAX_LINE 1024

class Client
{
public:
    Client();
    void start();
    int create_data_channel();
};

#endif