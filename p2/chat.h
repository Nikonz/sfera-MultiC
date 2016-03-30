#ifndef CHAT_H_INCLUDED
#define CHAT_H_INCLUDED

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <list>

#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <system_error>

#define SERVER 0
#define CLIENT 1

const unsigned short MY_PORT        = 31000;
const std::string    MY_IP          = "127.0.0.1";
const int            BACKLOG        = 30;
const int            EPOLL_SIZE     = 30;
const int            EPOLL_TIMEOUT  = 1000;
const int            BUF_SIZE       = 1024;

int         my_socket(int domain, int type, int protocol);
int         my_accept(int server, sockaddr_in* client);
void        my_connect(int client, sockaddr_in* server);
void        my_bind(int sock, sockaddr_in* addr);
void        my_listen(int sock, int backlog);
void        my_select(int num, fd_set* readSock, fd_set* writeSock, fd_set* exceptSock, int utimeout);
int         my_recv(int sock, void* buf, size_t len);
int         my_send(int sock, void* buf, size_t len);
void        my_setsockreuse(int sock);
sockaddr_in my_sockaddr_in(short domain, unsigned short port, char* ip);

int         my_epoll_create(int size);
void        my_epoll_ctl(int epfd, int op, int fd, epoll_event *ev);
int         my_epoll_wait(int epfd, epoll_event *evlist, int maxevents, int timeout);

int         my_setnonblocking(int sockfd);
void        my_sendall(int sock, void* buf, size_t len);
int         my_recvall(int sock, void* buf, size_t len);


#endif // CHAT_H_INCLUDED
