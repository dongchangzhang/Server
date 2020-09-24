//
// Created by vhdsih on 20-1-20.
//

#include "Server.h"
#include "../value/const_values.h"

bool Server::init() {
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        std::cerr << "create sockets fail!" << std::endl;
        return false;
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    int ret = bind(fd, (struct sockaddr*)&server, sizeof(server));
    if (ret < 0) {
        std::cerr << "sockets bind fail on " << port << std::endl;
        return false;
    }
    printf("server sockets binded on %d\n", port);
    len = sizeof(server);
    return true;
}

int Server::recv(u_char *buffer, int max_len) {
    return recvfrom(fd, buffer, max_len, 0, (struct sockaddr*)&peer, &len);
}

void Server::close_server() {
    close(fd);
    std::cout << "server closed" << std::endl;
}
