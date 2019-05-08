//
// Created by idz on 2019/4/2.
//

#ifndef MARSSIMULATOR_CLIENT_H
#define MARSSIMULATOR_CLIENT_H


#include <string>
#include <cstring>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>

#include "../utils/Log.h"
#include "../constants.h"
#include "../data/TimeCheck.h"
#include "../data/ImageToSend.h"
#include "../utils/Log.h"
#include "../data/GNC.h"

// As a client
// function : sending gnc

class Client {
public:
    Client(std::string _ip, int _port) : ip(_ip), port(_port) {
        auto y = init();
        PRINT_LOG("CLIENT", "INIT-SOCKET", "STATE", std::to_string(y));
    }

    ~Client() {
        close(fd);
    }

    int send(char buf[], int send_len);
    int send_gnc();
    int send_from_buff(int send_len);
private:
    bool init();
private:
    GNC gnc;
    int fd, port;
    std::string ip;
    sockaddr_in server;
    socklen_t len = sizeof(server);
    unsigned char buffer[BUFFER_SIZE];
};


#endif //MARSSIMULATOR_CLIENT_H
