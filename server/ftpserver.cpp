#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256

using namespace std;

int main() {
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    int len;
    int sock, new_sock;

    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("0.0.0.0");
    sin.sin_port = htons(SERVER_PORT);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error: creating socket");
        exit(1);
    }
    if ((bind(sock, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
        perror("Error: port binding");
        exit(1);
    }
    listen(sock, MAX_PENDING);

    while(1) {
        if ((new_sock = accept(sock, (struct sockaddr*)&sin, (socklen_t*)&len)) < 0) {
            perror("Error: accepting the connection");
            exit(1);
        }
        while(len = recv(new_sock, buf, sizeof(buf), 0))
            fputs(buf, stdout);
        close(new_sock);
    }
}