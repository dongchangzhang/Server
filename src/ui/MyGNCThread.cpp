
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
    int count;
    Client client(ip, port);
    GNC gnc;
    while (!TestDestroy()) {
        while (!handler->dataLoad && !handler->dataLoad) {
            wxMilliSleep(100);
        }
        while ((count = client.send_gnc(gnc)) == -1) {
            wxMilliSleep(100);
        }
        handler->yy = gnc.loc[1];
        handler->zz = gnc.loc[2];
        handler->gncInfo = wxString::Format(
                 _T("轨道编号 (模255)----- { %d }\n"
                    "发送轨道信息实时时间 { %4d/%2d/%2d %2d:%2d:%2d }\n"
                    "太阳方向------------- { x = %.1lf, y = %.1lf, z = %.1lf }\n"
                    "相机位置(单位米)----- { x = %.1lf; y = %.1lf, z = %.1lf }\n"
                    "相机欧拉角度(单位度) { 俯仰角 = %.1lf; 偏航角 = %.1lf; 滚动角 = %.1lf }\n"),
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
