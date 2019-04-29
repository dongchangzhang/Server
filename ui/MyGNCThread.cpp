
//
// Created by z on 19-4-29.
//

#include <utility>
#include "MyGNCThread.h"
#include "../utils/utils.h"
#include "../net/Client.h"

MyGNCThread::MyGNCThread(MyFrame *_handler, std::string _ip, int _port) : handler(_handler), ip(std::move(_ip)),
                                                                          port(_port),
                                                                          wxThread(wxTHREAD_DETACHED) {
    memset(buffer, 0, sizeof(buffer));

}

void *MyGNCThread::Entry() {
    bool stop = false;
    short count, y, m, d, hh, mm, ss;
    Client client(ip, port);
    while (!stop) {
        while ((count = client.send_gnc()) == -1) {
            wxMilliSleep(100);
        }
//        get_time(y, m, d, hh, mm, ss);
//        snprintf(infobuf, 128, "%4d/%2d/%2d %2d:%2d:%2d -> send gnc %5d, %4d bits\n", y, m, d, hh, mm, ss, id, count);
//        frame->tc1->WriteText(infobuf);

        auto *event = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
        event->SetInt(GNC_ID);
        wxQueueEvent(handler->GetEventHandler(), event);
        wxMilliSleep(250);

    }
    return nullptr;
}

bool MyGNCThread::start_thread() {
    this->Create();
    this->Run();
    return true;
}
