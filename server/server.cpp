#include "server.hpp"

using namespace std;
using json = nlohmann::json;


void Server::read_config_file(std::string config_file_path) 
{
    ifstream file(config_file_path);
    json j;
    file >> j;
    for (auto& user: j["users"])
    {
        User new_user = new User(
            user["user"].get<string>(),
            user["password"].get<string>(),
            stoi(user["size"].get<string>()),
            user["admin"].get<string>());
        users.push_back(new_user);
        new_user.get_attr();
    }
    for (auto& system_file: j["files"])
        system_files.push_back(system_file.get<string>());
    
}

void* Server::connect(void* temp_fd)
{
    int fd = *(int*) temp_fd;

    char buf[MAX_LINE];
    int len;
    while(len = recv(fd, buf, sizeof(buf), 0)){
            command_handler.command_parser(string(buf));
            command_handler.handle(std::vector<User> users);
    }
        close(fd);
    return NULL;
}

void Server::start() 
{
    struct sockaddr_in sin;
    int fd, new_fd;
    int len;
    int thread_number = 0;
    vector<thread> threads;

    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("0.0.0.0");
    sin.sin_port = htons(SERVER_PORT);

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation Error.");
        exit(1);
    }
    if ((bind(fd, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
        perror("Binding Error.");
        exit(1);
    }

    listen(fd, MAX_PENDING);

    while(1) {
        if ((new_fd = accept(fd, (struct sockaddr*)&sin, (socklen_t*)&len)) < 0) {
            perror("Unacceptable connection Error.");
            exit(1);
        }
        thread new_thread(&Server::connect, this,(void*)&new_fd);
        threads.push_back(move(new_thread));
        thread_number++;
    }
    for (int i = 0; i < threads.size();i++)
    {
        if (threads[i].joinable())
            threads[i].join();
    }
}
