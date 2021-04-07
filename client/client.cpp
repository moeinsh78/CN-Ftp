#include "client.hpp"

using namespace std;
Client::Client()
{
}

void Client::start()
{
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    int s;
    int len;

    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_port = htons(SERVER_COMMAND_PORT);

    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("simplex-talk: socket");
        exit(1);
    }

    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("simplex-talk: connect");
        close(s);
        exit(1);
    }

    int data_channel_fd = create_data_channel();

    while (fgets(buf, sizeof(buf), stdin)) {
        buf[MAX_LINE-1] = '\0';
        len = strlen(buf) + 1;
        send(s, buf, len, 0);
        recv(s, buf, MAX_LINE, 0);
        cout << buf << endl;
    }   
}

int Client::create_data_channel()
{
    struct sockaddr_in data_sin;
    int data_sock;
    int len;

    bzero((char *)&data_sin, sizeof(data_sin));
    data_sin.sin_family = AF_INET;
    data_sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    data_sin.sin_port = htons(SERVER_DATA_PORT);

    if ((data_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("simplex-talk: socket");
        exit(1);
    }
    if (connect(data_sock, (struct sockaddr *)&data_sin, sizeof(data_sin)) < 0)
    {
        perror("simplex-talk: connect");
        close(data_sock);
        exit(1);
    }
    return data_sock;
}