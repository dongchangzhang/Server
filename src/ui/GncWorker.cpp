
//
// Created by z on 19-4-29.
//

#include <utility>
#include "GncWorker.h"
#include "../utils/utils.h"
#include "../net/Client.h"

GncWorker::GncWorker(MainFrame *_handler, std::string _ip, int _port, GNC &_gnc) :
        handler(_handler), ip(std::move(_ip)), port(_port), wxThread(wxTHREAD_DETACHED), gnc(_gnc) {
    memset(buffer, 0, sizeof(buffer));

}

void *GncWorker::Entry() {
    int count;
    Client client(ip, port);
    while (!TestDestroy()) {
        while (!handler->dataLoad || handler->is_pause) {
            std::cout << "pause" << std::endl;
            wxMilliSleep(100);
        }
        while ((count = client.send_gnc(gnc)) == -1) {
            wxMilliSleep(100);
        }
        handler->yy = gnc.loc[1];
        handler->zz = gnc.loc[2];
        auto *event = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
        event->SetInt(GNC_ID);
        wxQueueEvent(handler->GetEventHandler(), event);
        wxMilliSleep(millis_per_gnc);

    }
    return nullptr;
}

bool GncWorker::start_thread() {
    this->Create();
    this->Run();
    return true;
}

bool GncWorker::stop_thread() {
    this->Delete();
    return true;
}
