//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_CLIENT_H
#define MDPI_CAMERA_CLIENT_H


#include <string>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>


// send photo

class Client {
public:
    Client(std::string _ip, int _port) : ip(_ip), port(_port) {
        auto y = init();
        std::cout << "sockets start? " << y << std::endl;
    }

    ~Client() {
        close(fd);
    }

    int send(char buf[], int send_len);
    void close_client();

private:
    bool init();

private:
    int fd, port;
    socklen_t len;
    std::string ip;
    sockaddr_in server;
};
#endif //MDPI_CAMERA_CLIENT_H
