//
// Created by z on 19-4-29.
//

#ifndef SERVERGUI_MYGNCTHREAD_H
#define SERVERGUI_MYGNCTHREAD_H

#include <wx/wx.h>
#include "MyFrame.h"

const int millis_per_gnc = 1000;

class MyGNCThread : public wxThread {
public:
    MyGNCThread(MyFrame *_handler, std::string _ip, int _port);
    void *Entry() override;

    bool start_thread();
private:
    int photo_id = 0;
    MyFrame *handler;
    std::string ip;
    int port;
    uchar buffer[BUFFER_SIZE];
};

#endif //SERVERGUI_MYGNCTHREAD_H
