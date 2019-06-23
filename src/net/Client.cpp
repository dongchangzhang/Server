//
// Created by idz on 2019/4/2.
//

#include <zconf.h>
#include <chrono>
#include <thread>
#include "Client.h"
#include "../data/ImageToSend.h"

bool Client::init() {
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        PRINT_LOG("CLIENT", "SOCKET", "CREATE", "FAIL!")
        return false;
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

#ifdef TEST_ON_LOCALHOST
    PRINT_LOG("CLIENT", "SERVER", "LOCALHOST ", std::to_string(port))
    server.sin_addr.s_addr = htonl(INADDR_ANY);
#else
    PRINT_LOG("CLIENT", "SERVER", ip + " ", std::to_string(port))
    server.sin_addr.s_addr = inet_addr(ip.c_str());
#endif

    return true;
}

int Client::send_gnc(GNC &_gnc) {
    int gnc_len = 0;
    memset(buffer, 0, BUFFER_SIZE);
    _gnc.get_gnc(buffer, gnc_len, BUFFER_SIZE);
    int count = send_from_buff(gnc_len);
    PRINT_LOG("CLIENT", "GNC", "LEN-SEND ", std::to_string(count))
    return count;
}

int Client::send_from_buff(int send_len) {
    int count = sendto(fd, buffer, send_len, 0, (struct sockaddr*) &server, len);
    return count;
}

int Client::send(char *buf, int send_len) {
    int count = sendto(fd, buf, send_len, 0, (struct sockaddr*) &server, len);
    return count;
}





