#include "client.hpp"

using namespace std;
using json = nlohmann::json;

Client::Client()
{
}

void Client::read_config_file(std::string config_file_path) 
{
    ifstream file(config_file_path);
    json j;
    file >> j;
    command_channel_port = j["commandChannelPort"];
    data_channel_port = j["dataChannelPort"];
}

void Client::start()
{
    struct sockaddr_in data_sin;
    struct sockaddr_in command_sin;
    int data_fd, command_fd;
    char buf[MAX_LINE];
    
    bzero((char *)&command_sin, sizeof(command_sin));
    command_sin.sin_family = AF_INET;
    command_sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    command_sin.sin_port = htons(command_channel_port);

    bzero((char *)&data_sin, sizeof(data_sin));
    data_sin.sin_family = AF_INET;
    data_sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    data_sin.sin_port = htons(data_channel_port);

    cout << "Command channel port: " << command_channel_port << endl;
    cout << "Data channel port: " << data_channel_port << endl;
    
    if ((command_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error creating command channel socket.");
        exit(1);
    }
    if ((data_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error creating data channel socket.");
        exit(1);
    }

    if (connect(command_fd, (struct sockaddr *)&command_sin, sizeof(command_sin)) < 0)
    {
        perror("Error creating command channel connection.");
        close(command_fd);
        exit(1);
    }
    if (connect(data_fd, (struct sockaddr *)&data_sin, sizeof(data_sin)) < 0)
    {
        perror("Error creating data channel connection.");
        close(data_fd);
        exit(1);
    }

    while (fgets(buf, sizeof(buf), stdin)) {
        buf[MAX_LINE-1] = '\0';
        int len = strlen(buf) + 1;
        send(command_fd, buf, len, 0);
        recv(command_fd, buf, MAX_LINE, 0);
        cout << buf << endl;
    }   
}