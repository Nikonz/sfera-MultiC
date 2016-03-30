#include "chat.h"

#include <ctime>
#include <vector>

//using namespace std;

const int TIMEOUT = 500;

void sig(int sig)
{
    exit(0);
}

int main()
{
    signal(SIGCHLD, sig);

    //freopen("log.txt", "w", stdout);
    if (!fork()) {
        //std::cout << getpid() << " " << getppid() << "\n";
        //std::cerr << getpid() << " " << getppid() << "\n";
        usleep(TIMEOUT * 1000000);
        kill(getppid(), SIGCHLD);
        //raise(SIGINT);
        exit(0);
    } else {
        int server = my_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in servAddr = my_sockaddr_in(AF_INET, MY_PORT, (char *)MY_IP.c_str());

        my_setnonblocking(server);

        my_setsockreuse(server);
        my_bind(server, &servAddr);
        my_listen(server, BACKLOG);

        epoll_event event, events[EPOLL_SIZE] = {0};
        event.events = EPOLLIN | EPOLLET;
        char buf[BUF_SIZE + 1];

        int epoll = my_epoll_create(EPOLL_SIZE);
        event.data.fd = server;
        my_epoll_ctl(epoll, EPOLL_CTL_ADD, server, &event);

        bool clients[EPOLL_SIZE] = {0};
        int maxfd = -1;
        int cnt = -1;

        //std::cout << "Start server\n";
        while (true) {
            if (!cnt) break;
            //events[0].data.fd = 11;
            //std::cout << cnt;
            fflush(stdout);
            int events_cnt = my_epoll_wait(epoll, events, EPOLL_SIZE, -1);
            //cnt = (cnt == -1 ? 0 : cnt);
            //std::cerr << events_cnt << std::endl;
            //if (clock() > TIMEOUT) break;


            for (int i = 0; i < events_cnt; i++) {
                //std::cerr << events[i].events << " " << events[i].data.fd << "\n";
                if (events[i].events & (EPOLLERR | EPOLLHUP)) {
                    cnt--;
                    std::cout << "connection terminated\n";
                    clients[events[i].data.fd] = false;
                    my_epoll_ctl(epoll, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                } else if (events[i].data.fd == server) {
                    sockaddr_in clientAddr;

                    int client;
                    int len = sizeof(clientAddr);
                    while ((client = accept(server, (sockaddr*)&clientAddr, (socklen_t*)&len)) != -1) {
                        //cnt++;

                        my_setnonblocking(client);
                        event.data.fd = client;
                        event.events = EPOLLIN | EPOLLET;
                        my_epoll_ctl(epoll, EPOLL_CTL_ADD, client, &event);

                        std::cout << "accepted connection\n";
                        clients[client] = true;
                        maxfd = (maxfd > client ? maxfd : client);

                        my_sendall(client, (void *)"Welcome!\n", sizeof("Welcome!\n"));
                    }

                } else if (events[i].events & EPOLLIN) {
                    int len = my_recvall(events[i].data.fd, buf, BUF_SIZE);
                    buf[len] = '\0';

                    if (len == 0) {
                        //close(events[i].data.fd);
                        //cnt--;
                        std::cout << "connection terminated\n";
                        clients[events[i].data.fd] = false;
                        my_epoll_ctl(epoll, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    } else {
                        //std::cerr << "here\n";
                        std::cout << buf;
                        for (int j = 0; j <= maxfd; j++) {
                            if (clients[j]) {
                                //std::cerr << "client = " << j << std::endl;
                                //my_sendall(events[j].data.fd, (void *)"Server: ", sizeof("Server: "));
                                my_sendall(j, buf, len);
                            }
                        }
                    }
                } else {
                    std::cerr << "Error\n";
                    abort();
                }

            }
        }
        close(server);
        close(epoll);
    }

    return 0;
}
