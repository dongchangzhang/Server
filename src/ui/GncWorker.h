//
// Created by z on 19-4-29.
//

#ifndef SERVERGUI_GNCWORKER_H
#define SERVERGUI_GNCWORKER_H

#include <wx/wx.h>
#include "MainFrame.h"

const int millis_per_gnc = 1000;

class GncWorker : public wxThread {
public:
    GncWorker(MainFrame *_handler, std::string _ip, int _port);
    void *Entry() override;

    bool start_thread();
private:
    int photo_id = 0;
    MainFrame *handler;
    std::string ip;
    int port;
    uchar buffer[BUFFER_SIZE];
};

#endif //SERVERGUI_GNCWORKER_H
