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

#define SERVER_PORT 5432
#define MAX_LINE 256

class Client
{
public:
    Client();
    void start();
};

#endif