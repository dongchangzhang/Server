
//
// Created by z on 19-4-29.
//

#include <utility>
#include "GncWorker.h"
#include "../utils/utils.h"
#include "../net/Client.h"

GncWorker::GncWorker(MainFrame *_handler, std::string _ip, int _port) :
        handler(_handler), ip(std::move(_ip)), port(_port), wxThread(wxTHREAD_DETACHED) {
    memset(buffer, 0, sizeof(buffer));

}

void *GncWorker::Entry() {
    int count;
    Client client(ip, port);
    while (!TestDestroy()) {
        while (!handler->dataLoad || handler->is_pause) {
            wxMilliSleep(100);
        }
        while ((count = client.send_gnc(handler->gnc)) == -1) {
            wxMilliSleep(100);
        }
        handler->yy = handler->gnc.loc[1];
        handler->zz = handler->gnc.loc[2];
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
