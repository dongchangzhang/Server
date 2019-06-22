//
// Created by z on 19-5-7.
//

#ifndef SERVERGUI_CMDWORKER_H
#define SERVERGUI_CMDWORKER_H

#include <wx/wx.h>
#include "MainFrame.h"

class CmdWorker : public wxThread {
public:
    CmdWorker(MainFrame *_handler, std::string _ip, int _port);
    void *Entry() override;

    bool start_thread();
private:
    MainFrame *handler;
    std::string ip;
    int port;
    uchar buffer[BUFFER_SIZE];
};

#endif //SERVERGUI_CMDWORKER_H
