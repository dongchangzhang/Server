//
// Created by vhdsih on 20-1-20.
//

#include "GncRecvByNet.h"

#include "../data/GNC.h"
#include "../net/Server.h"
#include "../value/values.h"

#include <iostream>

GncRecvByNet::GncRecvByNet(wxWindow *_handler, int _port) :
    ThreadBase(_handler), port(_port), ngnc(0) {

}

void *GncRecvByNet::Entry() {
    const int BUF_LEN = 512;
    u_char buffer[BUF_LEN];
    Server server(port);


    while (!TestDestroy()) {
        std::cout << "waiting gnc data from udp network ..." << std::endl;
        server.recv(buffer, BUF_LEN);
        std::cout << "recv gnc yes" << std::endl;
        GNC gnc(buffer);
        // check
        if (!gnc.is_right()) {
            std::cout << "recv-gnc-pkg-error; continue." << std::endl;
            continue;
        }

        while (!write_show_info(gnc.get_x(), gnc.get_y(), gnc.get_z())) {
            wxMilliSleep(1);
        }

        while (!write_gnc(gnc)) wxMilliSleep(1);

        while (!write_show_gnc(gnc)) wxMilliSleep(1);

        wxMilliSleep(1);
    }
    server.close_server();
    return nullptr;
}

