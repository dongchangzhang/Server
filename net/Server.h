//
// Created by idz on 2019/4/2.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <cstring>

#include "../constants.h"
#include "../data/TimeCheck.h"
#include "../data/GNC.h"

// As a server
// recv photo

class Server {
public:
    Server(int _port) : port(_port) {
        init();
    }
    ~Server() {
        close(fd);
    }
    void recv_photo();
    bool recv_into_buff(int &recv_len);

private:
    bool init();
private:
    int fd, port;
    int height, width;
    int photo_code = 0;
    sockaddr_in server, peer;
    socklen_t len = sizeof(server);
    unsigned char buffer[BUFFER_SIZE];
};


#endif //SERVER_SERVER_H
