//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_SERVER_H
#define MDPI_CAMERA_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <cstring>

class Server {
public:
    explicit Server(int _port) : port(_port) {
        init();
    }
    ~Server() {
        close(fd);
    }

    int recv(u_char *buffer, int max_len);
    void close_server();

private:
    bool init();

private:
    int fd, port;
    socklen_t len;
    sockaddr_in server, peer;
};

#endif //MDPI_CAMERA_SERVER_H
