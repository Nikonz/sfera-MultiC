#include "chat.h"

int my_socket(int domain, int type, int protocol)
{
    int sock = socket(domain, type, protocol);
    if (sock == -1) {
        std::cerr << "Can't create socket\n";
        throw std::system_error(errno, std::system_category());
    }
    return sock;
}

int my_accept(int server, sockaddr_in* client)
{
    int len = sizeof(sockaddr);
    int sock = accept(server, (sockaddr*)client, (socklen_t*)&len);
    if (sock == -1) {
        std::cerr << "Accept failed\n";
        throw std::system_error(errno, std::system_category());
    } else {
        std::cout << "accepted connection\n";
        //std::cerr << "accepted connection\n";
    }
    return sock;
}

void my_connect(int client, sockaddr_in* server)
{
    if (connect(client, (sockaddr*)server, sizeof(sockaddr)) == -1) {
        std::cerr << "Connect failed\n";
        throw std::system_error(errno, std::system_category());
    }
}

void my_bind(int sock, sockaddr_in* addr)
{
    if (bind(sock, (sockaddr*)addr, sizeof(sockaddr)) == -1) {
        std::cerr << "Bind failed\n";
        throw std::system_error(errno, std::system_category());
    }
}

void my_listen(int sock, int backlog)
{
    if (listen(sock, backlog) == -1) {
        std::cerr << "Listen failed\n";
        throw std::system_error(errno, std::system_category());
    }
}

void my_select(int num, fd_set* readSock, fd_set* writeSock, fd_set* exceptSock, int utimeout)
{
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = utimeout;

    if (select(num, readSock, writeSock, exceptSock, &timeout) == -1) {
        std::cerr << "Select failed\n";
        throw std::system_error(errno, std::system_category());
    }
}

int my_recv(int sock, void* buf, size_t len)
{
    int res = recv(sock, buf, len, 0);
    if (res == -1) {
        //std::cerr << "Recv failed\n";
        //abort();
    } else if (res == 0) {

    }
    return res;
}

int my_send(int sock, void* buf, size_t len)
{
    int res = send(sock, buf, len, 0);
    if (res == -1) {
        std::cerr << "Send failed\n";
        throw std::system_error(errno, std::system_category());
    }
    return res;
}

void my_setsockreuse(int sock)
{
    int tmp = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)) == -1) {
        std::cerr << "Setsockreuse failed\n";
        throw std::system_error(errno, std::system_category());
    }
}

sockaddr_in my_sockaddr_in(short domain, unsigned short port, char* ip)
{
    sockaddr_in addr;
    addr.sin_family      = domain;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    memset(&(addr.sin_zero), '\0', 8);
    return addr;
}

int my_epoll_create(int size)
{
    int fd = epoll_create(size);
    if (fd == -1) {
        std::cerr << "Can't create epoll\n";
        throw std::system_error(errno, std::system_category());
    }
    return fd;
}

void my_epoll_ctl(int epfd, int op, int fd, epoll_event *ev)
{
    if (epoll_ctl(epfd, op, fd, ev) == -1) {
        std::cerr << "Epoll_ctl failed\n";
        throw std::system_error(errno, std::system_category());
    }
}

int my_epoll_wait(int epfd, epoll_event *evlist, int maxevents, int timeout)
{
    int num = epoll_wait(epfd, evlist, maxevents, timeout);
    if (num == -1) {
        std::cerr << "Epoll_wait failed\n";
        throw std::system_error(errno, std::system_category());
    }
}

int my_setnonblocking(int sock)
{
   if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFD, 0)|O_NONBLOCK) == -1) {
        std::cerr << "Setnonblocking failed\n";
        throw std::system_error(errno, std::system_category());
   }
   return 0;
}

void my_sendall(int sock, void* buf, size_t len)
{
    int sent = 0;
    while (sent != len) {
        sent += my_send(sock, buf + sent, len - sent);
    }
    //std::cerr << "Sent = " << sent << "\n";
}

int my_recvall(int sock, void* buf, size_t len)
{
    int got = 0, tmp;
    while ((tmp = my_recv(sock, buf + got, len - got)) > 0) {
        got += tmp;
    }
    return got;
}
