#include "server.hpp"

using namespace std;

void Server::read_config_file(std::string config_file_path) {

    ifstream config_file(config_file_path, ifstream::binary);
    config_file >> config;

    cout << people; //This will print the entire json object.

    //The following lines will let you access the indexed objects.
    cout<<people["Anna"]; //Prints the value for "Anna"
    cout<<people["ben"]; //Prints the value for "Ben"
    cout<<people["Anna"]["profession"]; //Prints the value corresponding to "profession" in the json for "Anna"

    cout<<people["profession"]; //NULL! There is no element with key "profession". Hence a new empty element will be created.
}

void Server::start() {
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    int len;
    int sock, new_sock;

    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("0.0.0.0");
    sin.sin_port = htons(SERVER_PORT);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation Error.");
        exit(1);
    }
    if ((bind(sock, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
        perror("Binding Error.");
        exit(1);
    }
    listen(sock, MAX_PENDING);

    while(1) {
        if ((new_sock = accept(sock, (struct sockaddr*)&sin, (socklen_t*)&len)) < 0) {
            perror("Unacceptable connection Error.");
            exit(1);
        }
        while(len = recv(new_sock, buf, sizeof(buf), 0))
            fputs(buf, stdout);
        close(new_sock);
    }
}