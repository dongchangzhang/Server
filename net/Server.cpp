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
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
        std::cout << "sockets bind fail!" << std::endl;
        return false;
    }
    std::cout << "sockets yes!" << std::endl;
    return true;
}

void Server::run() {
    int count = 0;
    bool jump_out = false;
    while (!jump_out) {
        switch (port) {
            case TIME_PORT:
                memset(buffer, 0, BUFFER_SIZE);
                count = recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&dst, &len);
                if (count == -1) {
                    std::cout << "receive data fail!" << std::endl;
                    return;
                }
                tc.update();
                tc.get_time_info(buffer);
                sendto(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&dst, len);
                std::cout << "send time" << std::endl;
                break;
            case INJECTION_POER:
                break;
            case GNC_PORT:
                break;
            case SEND_IMAGE_PORT:
                recv_photo();
                std::cout << std::endl;
                break;
            default:
                std::cout << "port error!" << std::endl;
                jump_out = true;
        }
    }

}
void Server::run_test() {
    socklen_t len;
    int count;
    struct sockaddr_in client_addr;
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        len = sizeof(client_addr);
        count = recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &len);
        if (count == -1) {
            std::cout << "receive data fail!" << std::endl;
            return;
        }
        tc.update();
        tc.get_time_info(buffer);
        sendto(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, len);
        std::cout << "send time" << std::endl;
        sleep(3);
    }


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

void Server::send_gnc() {
    int gnc_len = 0;
    memset(buffer, 0, BUFFER_SIZE);
    gnc.get_gnc(buffer, gnc_len, BUFFER_SIZE);
    std::cout << "SERVER::GNC sending ..." << std::endl;
    int y = sendto(fd, buffer, gnc_len, 0, (struct sockaddr*)&dst, len);
    std::cout << "SERVER::GNC send state: " << y << std::endl;
}

void Server::say_hello() {
    memset(buffer, 0, BUFFER_SIZE);
    buffer[0] = 'H';
    buffer[1] = 'i';
    buffer[2] = '!';
    send_from_buff(3);
}

void Server::recv_hello() {
    int recv_len = 0;
    recv_into_buff(recv_len);
    printf("%s\n", buffer);
}

bool Server::send_from_buff(int send_len) {
    int y_count = sendto(fd, buffer, send_len, 0, (struct sockaddr*) &addr, sizeof(addr));
    return y_count >= 0;
}

bool Server::recv_into_buff(int &recv_len) {
    memset(buffer, 0, BUFFER_SIZE);
    recv_len = recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&dst, &len);
    if (recv_len == -1) {
        std::cout << "receive data fail!" << std::endl;
        return false;
    }
    return true;
}
