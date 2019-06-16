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
#include <opencv2/opencv.hpp>

#include "../constants.h"
#include "../data/TimeCheck.h"
#include "../data/GNC.h"
#include "../ui/MyFrame.h"

// As a server
// recv photo

class Server {
public:
    Server() {}
    Server(int _port) : port(_port) {
        init();
    }
    ~Server() {
        close(fd);
    }
    bool recv(u_char *info, int max_len);
    void recv_photo();
    bool recv_into_buff(int &recv_len);
    bool recv_into_buff(int &recv_len, uchar _buffer[]);

    inline int test_server() { return 1; }

public:
    static cv::Mat photo;

private:
    bool init();

private:
    int fd, port;
    int height, width;
    int photo_code = 0;
    sockaddr_in server, peer;
    socklen_t len = sizeof(server);
    unsigned char buffer[BUFFER_SIZE];

    // image to show
    std::string window_name = " - mars - camera - ";
};

#endif //SERVER_SERVER_H
