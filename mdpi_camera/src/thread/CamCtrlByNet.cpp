//
// Created by vhdsih on 20-1-20.
//

#include "CamCtrlByNet.h"
#include "../net/Server.h"
#include "../value/values.h"
#include "../ui/Home.h"

CamCtrlByNet::CamCtrlByNet(wxWindow *_handler, int _port) : ThreadBase(_handler), port(_port) {

}

void *CamCtrlByNet::Entry() {
    const int BUF_LEN = 256;
    u_char buffer[BUF_LEN];
    Server server(port);

    DataInjection dij;
    while (!TestDestroy()) {
#ifdef PRINT_INFO
        std::cout << "waiting camera ctrl from udp network..." << std::endl;
#endif
        server.recv(buffer, BUF_LEN);
#ifdef PRINT_INFO
        std::cout << "camera ctrl recved: yes" << std::endl;
#endif
        while (!read_camera_params(dij)) {
            wxMilliSleep(1);
        }

        if (dij.parser(buffer)) {
            ((Home *) handler)->set_ctrl(dij);
            auto *event = new wxThreadEvent(wxEVT_THREAD, ID_CTRL_THREAD_UPDATE);
            wxQueueEvent(handler->GetEventHandler(), event);
        }
        while (!write_camera_params(dij)) wxMilliSleep(1);
    }
    server.close_server();
    return nullptr;
}
