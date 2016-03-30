#include "chat.h"

const int SLEEP_TIME = (int)1e4;

int main()
{
    ///int he = gethostbyname();
    int client = my_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr = my_sockaddr_in(AF_INET, MY_PORT, (char*)MY_IP.c_str());
    my_connect(client, &addr);

    my_setnonblocking(client);

    /*
    if (!fork()) {
        fd_set allSock;
        fd_set readSock;
        FD_ZERO(&allSock);
        FD_ZERO(&readSock);
        FD_SET(client, &allSock);
        char buf[BUF_SIZE + 1];

        while (true) {
            readSock = allSock;
            my_select(client + 1, &readSock, NULL, NULL, SLEEP_TIME);
            if (FD_ISSET(client, &readSock)) {
                int len = my_recvall(CLIENT, client, buf, BUF_SIZE);
                /*
                if (len == 0) {
                    std::cout << "Connection closed by server.\n";
                    kill(getppid(), SIGTERM);
                    exit(0);
                }

                buf[len] = '\0';
                std::cout << "Received: " << buf;
            }
        }
    } else {
        char buf[BUF_SIZE + 1];
        while (true) {
            int len = read(STDIN_FILENO, buf, BUF_SIZE);
            buf[len] = '\0';
            //std::cerr << buf;
            my_sendall(client, buf, len);
            usleep(SLEEP_TIME);
        }
    } */
    close(client);

    return 0;
}
