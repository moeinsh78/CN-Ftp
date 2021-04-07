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

void* Client::handle_incoming_files(void* fd)
{
    int data_fd = *(int*) fd;
    int received_files = 0;
    char buf[MAX_FILE_SIZE];
    while(true) {
            char buf[MAX_FILE_SIZE];

        recv(data_fd, buf, MAX_FILE_SIZE, 0);
        received_files += 1;
        string file_name = "file" + to_string(received_files) + ".txt";
        FILE *f = fopen(file_name.c_str(), "w");
        fwrite(buf , sizeof(char), sizeof(buf) + 1, f);
        fclose(f);
    }
}

void* Client::handle_commands(void* fd)
{
    int command_fd = *(int*) fd;
    int received_files = 0;
    char buf[MAX_LINE];

    while (fgets(buf, sizeof(buf), stdin)) {
        buf[MAX_LINE-1] = '\0';
        int len = strlen(buf) + 1;
        send(command_fd, buf, len, 0);
        recv(command_fd, buf, MAX_LINE, 0);
        cout << buf << endl;
    }
}

void Client::start()
{
    struct sockaddr_in data_sin;
    struct sockaddr_in command_sin;
    int data_fd, command_fd;
    vector<thread> threads;

    bzero((char *)&command_sin, sizeof(command_sin));
    command_sin.sin_family = AF_INET;
    command_sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    command_sin.sin_port = htons(command_channel_port);

    bzero((char *)&data_sin, sizeof(data_sin));
    data_sin.sin_family = AF_INET;
    data_sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    data_sin.sin_port = htons(data_channel_port);

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

    thread data_thread(&Client::handle_incoming_files, this, (void*)&data_fd);
    thread command_thread(&Client::handle_commands, this, (void*)&command_fd);
    threads.push_back(move(data_thread));
    threads.push_back(move(command_thread));

    for (int i = 0; i < threads.size();i++)
    {
        if (threads[i].joinable())
            threads[i].join();
    }
}