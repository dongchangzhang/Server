//
// Created by idz on 2019/4/2.
//

#include <zconf.h>
#include <opencv2/opencv.hpp>
#include "Server.h"

bool Server::init() {
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        std::cout << "create sockets fail!" << std::endl;
        return false;
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    int ret = bind(fd, (struct sockaddr*)&server, sizeof(server));
    if (ret < 0) {
        std::cout << "sockets bind fail!" << std::endl;
        return false;
    }
    std::cout << "sockets yes!" << std::endl;
    return true;
}

void Server::recv_photo() {
    int recv_len = -1;
    // get photo info
    std::cout << "SERVER::Photo Getting Info ..." << std::endl;
    if (recv_into_buff(recv_len)) {
        if (buffer[0] != 0x55) {
            std::cout << "not an image info segment" << std::endl;
            return;
        }
    }
    // get data
    std::cout << "SERVER::Photo Getting content ..." << std::endl;
    cv::Mat photo(PHOTO_HEIGHT, PHOTO_WIDTH, CV_8UC3, cv::Scalar::all(0));
    u_char op, id;
    short nline = 0;
    while (recv_into_buff(recv_len)) {
        if (buffer[0] != 0xAA) {
            std::cout << buffer[0] << "not an image line" << std::endl;
            return;
        }
        memcpy(&id, &buffer[10], sizeof(char));
        memcpy(&nline, &buffer[12], sizeof(short));
        memcpy(&photo.data[nline * PHOTO_WIDTH * 3], &buffer[18], PHOTO_WIDTH * sizeof(char) * 3);
        ++nline;
        memcpy(&op, &buffer[17], 1);
        if (op == 0xAA)
            break;
        std::cout << nline << std::endl;
    }
    std::cout << "SERVER::Photo Stat: y" << std::endl;
    cv::imwrite(std::to_string(photo_code++) + ".png", photo);
}

bool Server::recv_into_buff(int &recv_len) {
    memset(buffer, 0, BUFFER_SIZE);
    recv_len = recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&peer, &len);
    if (recv_len == -1) {
        std::cout << "receive data fail!" << std::endl;
        return false;
    }
    return true;
}
