
//
// Created by z on 19-4-29.
//

#include <utility>
#include "MyGNCThread.h"
#include "../utils/utils.h"
#include "../net/Client.h"

MyGNCThread::MyGNCThread(MyFrame *_handler, std::string _ip, int _port) :
        handler(_handler), ip(std::move(_ip)), port(_port), wxThread(wxTHREAD_DETACHED) {
    memset(buffer, 0, sizeof(buffer));

}

void *MyGNCThread::Entry() {
    bool stop = false;
    int count;
    Client client(ip, port);
    GNC gnc;
    while (!stop) {
        while (!handler->dataLoad) {
            wxMilliSleep(100);
        }
        while ((count = client.send_gnc(gnc)) == -1) {
            wxMilliSleep(100);
        }
        snprintf(handler->gncinfo, 512,
                 "GNC ID (%% 255):\n -> %d\n"
                 "Sending Time:\n -> %4d/%2d/%2d %2d:%2d:%2d\n"
                 "Sun Direction:\n -> (%6.1lf, %6.1lf, %6.1lf)\n"
                 "Camera Location:\n -> (%6.1lf, %6.1lf, %6.1lf)\n"
                 "Roll Pitch Yaw:\n ->  (%6.1lf, %6.1lf, %6.1lf)\n",
                 ++photo_id % 255,
                 gnc.year + 1900, gnc.month, gnc.day, gnc.hour, gnc.minute, gnc.second,
                 -gnc.sun[0], -gnc.sun[1], -gnc.sun[2],
                 gnc.loc[0], gnc.loc[1], gnc.loc[2],
                 gnc.posture[0], gnc.posture[1], gnc.posture[2]
        );
        auto *event = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
        event->SetInt(GNC_ID);
        wxQueueEvent(handler->GetEventHandler(), event);
        wxMilliSleep(millis_per_gnc);

    }
    return nullptr;
}

bool MyGNCThread::start_thread() {
    this->Create();
    this->Run();
    return true;
}
