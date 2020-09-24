//
// Created by vhdsih on 20-1-20.
//

#include "Client.h"
#include <iostream>
#include "../utils/Log.h"
#include "../value/const_values.h"

bool Client::init() {
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        std::cerr << "create socket error!" << std::endl;
        return false;
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
#ifdef TEST_ON_LOCALHOST
    PRINT_LOG("CLIENT", "SERVER", "LOCALHOST ", std::to_string(port))
    server.sin_addr.s_addr = htonl(INADDR_ANY);
#else
    server.sin_addr.s_addr = inet_addr(ip.c_str());
#endif
    len = sizeof(server);
    return true;
}


int Client::send(char *buf, int send_len) {
    return sendto(fd, buf, send_len, 0, (struct sockaddr*) &server, sizeof(server));
}

void Client::close_client() {
    close(fd);
}


