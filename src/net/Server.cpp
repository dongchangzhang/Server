//
// Created by idz on 2019/4/2.
//

#include <zconf.h>
#include <opencv2/opencv.hpp>
#include "Server.h"
#include "../utils/utils.h"
#include <wx/wx.h>

cv::Mat Server::photo;

static void setnonblocking(int sockfd) {
    int flag = fcntl(sockfd, F_GETFL, 0);
    if (flag < 0) {
        std::cout << "fcntl F_GETFL fail" << std::endl;
        return;
    }
    if (fcntl(sockfd, F_SETFL, flag | O_NONBLOCK) < 0) {
        std::cout << "fcntl F_GETFL fail" << std::endl;
    }
}

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
    u_char mode, where;
    short start_i, start_j, ni, nj;
    int recv_len = -1, copy_len;
    // get photo info
    std::cout << "SERVER::Photo Getting Info ..." << std::endl;
    if (recv_into_buff(recv_len)) {
        std::cout << "SERVER::Photo Getting Info ..." << std::endl;
        if (buffer[0] != 0x55) {
            std::cout << "not an image info segment" << std::endl;
            return;
        }
    }
    memcpy(&mode, &buffer[4], 1);
    if (mode != 0x44) {
        get_height_width(mode, height, width);
        copy_len = width * 3 / 2;
    } else if (mode == 0x44) {
        // open window
        memcpy(&ni, &buffer[9], 2);
        memcpy(&nj, &buffer[11], 2);
        height = ni;
        width = nj;
    } else {
        std::cout << "Image Mode Set Error, Need 0x00, 0x11, 0x22, 0x33 or 0x44!" << std::endl;
        return;
    }
    // get data
    std::cout << "SERVER::Photo Getting content ..." << std::endl;
    photo = cv::Mat(height, width, CV_8UC3, cv::Scalar::all(0));
    u_char op, id;
    short nline = 0;
    while (recv_into_buff(recv_len)) {
        if (buffer[0] != 0xAA) {
            std::cout << buffer[0] << "not an image line" << std::endl;
            return;
        }
        memcpy(&id, &buffer[10], sizeof(char));
        if (mode == 0x44) {
            memcpy(&photo.data[nline * width * 3], &buffer[18], width * sizeof(char) * 3);
            ++nline;
            goto label;
        }
        memcpy(&nline, &buffer[12], sizeof(short));
        if (mode == 0x00) {
            memcpy(&where, &buffer[16], 1);
            if (where == 0x55) {
                memcpy(&photo.data[nline * width * 3], &buffer[18], copy_len);
            } else {
                memcpy(&photo.data[nline * width * 3 + copy_len], &buffer[18], copy_len);
                ++nline;
            }

        } else {
                memcpy(&photo.data[nline * width * 3], &buffer[18], width * sizeof(char) * 3);
                ++nline;
        }
    label:
        memcpy(&op, &buffer[17], 1);
        if (op == 0xAA)
            break;
        std::cout << nline << ": " << recv_len << std::endl;
    }
    std::cout << "SERVER::Photo Stat: y " << photo_code << std::endl;
    cv::imwrite(std::to_string(photo_code++) + ".png", photo);
}

bool Server::recv(u_char *info, int max_len) {
    memset(info, 0, max_len);
    int recv_len = recvfrom(fd, info, BUFFER_SIZE, 0, (struct sockaddr*)&peer, &len);
    if (recv_len == -1) {
        return false;
    }
    return true;
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

bool Server::recv_into_buff(int &recv_len, uchar _buffer[]) {
    memset(_buffer, 0, BUFFER_SIZE);
    recv_len = recvfrom(fd, _buffer, BUFFER_SIZE, 0, (struct sockaddr*)&peer, &len);
    if (recv_len == -1) {
        std::cout << port << " receive data fail!" << std::endl;
        return false;
    }
    return true;
}


