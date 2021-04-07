#include "server.hpp"

using namespace std;
using json = nlohmann::json;

string path;

typedef struct 
{
    int data_sock_fd;
    int command_sock_fd;
} Sockets;

Sockets sockets_fd_array[MAX_PENDING];

Server::Server()
{
}

void Server::record_log(string message)
{
    string file;
    file = path + "/server.log";
    logfile.open(file.c_str(), std::ofstream::out | std::ofstream::app);
    time_t now = time(0);
    logfile << message <<  " -- " << (char*) ctime(&now);
    logfile.close();
}

void Server::read_config_file(std::string config_file_path) 
{
    ifstream file(config_file_path);
    json j;
    file >> j;
    for (auto& user: j["users"])
    {
        User new_user = User(
            user["user"].get<string>(),
            user["password"].get<string>(),
            stoi(user["size"].get<string>()),
            user["admin"].get<string>());
        users.push_back(new_user);
    }
    for (auto& system_file: j["files"])
        system_files.push_back(system_file.get<string>());

    command_channel_port = j["commandChannelPort"];
    data_channel_port = j["dataChannelPort"];
}

void* Server::connect(void* thread_number)
{
    int thread_id = (long)thread_number;
    string log_str = "Client connected. It will be handled by thread number " + to_string(thread_id);
    record_log(log_str);
    int command_channel_fd = sockets_fd_array[thread_id].command_sock_fd;
    int data_channel_fd = sockets_fd_array[thread_id].data_sock_fd;
    char buf[MAX_LINE];
    int len;
    CommandHandler command_handler(path,system_files);
    while((len = recv(command_channel_fd, buf, sizeof(buf), 0))){
        command_handler.command_parser(string(buf));
        try
        {
            command_handler.handle(this->users,data_channel_fd);
        } catch (string excep)
        {
            send(command_channel_fd, excep.c_str(), MAX_LINE, 0);
        }
    }
    close(data_channel_fd);
    close(command_channel_fd);
    return NULL;
}

void Server::start() 
{
    system("pwd >> temp.txt");
    ifstream infile("temp.txt");
    getline(infile, path);
    remove("temp.txt");

    struct sockaddr_in data_sin;
    struct sockaddr_in command_sin;
    int command_fd, command_new_fd;
    int data_fd, data_new_fd;
    int command_len, data_len;
    long thread_number = 0;
    vector<thread> threads;
    
    bzero((char *)&command_sin, sizeof(command_sin));
    command_sin.sin_family = AF_INET;
    command_sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    command_sin.sin_port = htons(command_channel_port);

    bzero((char *)&data_sin, sizeof(data_sin));
    data_sin.sin_family = AF_INET;
    data_sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    data_sin.sin_port = htons(data_channel_port);

    if ((command_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Command socket creation Error.");
        exit(1);
    }
    if ((data_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Data socket creation Error.");
        exit(1);
    }

    if ((::bind(command_fd, (struct sockaddr *)&command_sin, sizeof(command_sin))) < 0) {
        perror("Command socket port binding Error.");
        exit(1);
    }
    
    if ((::bind(data_fd, (struct sockaddr *)&data_sin, sizeof(data_sin))) < 0) {
        perror("Data socket port binding Error.");
        exit(1);
    }

    record_log("Ftp server started");

    listen(command_fd, MAX_PENDING);
    listen(data_fd, MAX_PENDING);

    while(1) {
        if ((command_new_fd = accept(command_fd, (struct sockaddr*)&command_sin, (socklen_t*)&command_len)) < 0) {
            perror("Unacceptable connection Error.");
            exit(1);
        }
        if ((data_new_fd = accept(data_fd, (struct sockaddr*)&data_sin, (socklen_t*)&data_len)) < 0) {
            perror("Unacceptable connection Error.");
            exit(1);
        }
        sockets_fd_array[thread_number].command_sock_fd = command_new_fd;
        sockets_fd_array[thread_number].data_sock_fd = data_new_fd;
        thread new_thread(&Server::connect, this,(void*)thread_number);
        threads.push_back(move(new_thread));
        thread_number++;
    }
    for (int i = 0; i < threads.size();i++)
    {
        if (threads[i].joinable())
            threads[i].join();
    }
}