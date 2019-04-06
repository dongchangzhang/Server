//
// Created by idz on 2019/4/2.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <string>
#include <cstring>

#include "constants.h"
#include "data/TimeCheck.h"
#include "data/GNC.h"

class Server {
public:
    Server(std::string _ip, int _port) : client_ip(_ip), port(_port) {
        init();
    }
    ~Server() {
//        close(fd);
    }
    void run();
    void run_test();

    void receive_info();
    void receive_photo();

    void send_gnc();
    void send_time_info();

    bool send();
    bool recv(int &recv_len);


private:
    bool init();
private:
    int port;
    int fd;
    std::string client_ip;
    struct sockaddr_in addr, dst;
    socklen_t len = sizeof(addr);
    unsigned char buffer[BUFFER_SIZE];

    int photo_code = 0;

    TimeCheck tc;
    GNC gnc;
};


#endif //SERVER_SERVER_H
