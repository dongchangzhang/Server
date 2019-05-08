//
// Created by z on 19-5-7.
//

#ifndef SERVERGUI_MYCMDTHREAD_H
#define SERVERGUI_MYCMDTHREAD_H

#include <wx/wx.h>
#include "MyFrame.h"

class MyCmdThread : public wxThread {
public:
    MyCmdThread(MyFrame *_handler, std::string _ip, int _port);
    void *Entry() override;

    bool start_thread();
private:
    MyFrame *handler;
    std::string ip;
    int port;
    uchar buffer[BUFFER_SIZE];
};

#endif //SERVERGUI_MYCMDTHREAD_H
