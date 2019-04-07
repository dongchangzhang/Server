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
    Server(std::string _ip, int _port) : client_ip(_ip), port(_port) {
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
    int photo_code = 0;
    std::string client_ip;
    sockaddr_in server, peer;
    socklen_t len = sizeof(server);
    unsigned char buffer[BUFFER_SIZE];
};


#endif //SERVER_SERVER_H
