//
// Created by z on 19-4-29.
//

#ifndef SERVERGUI_MYRECVTHREAD_H
#define SERVERGUI_MYRECVTHREAD_H

#include <wx/wx.h>

#include "../constants.h"
#include "../net/Server.h"

const int NITERS_TO_UPDATE_UI = 500;
using uchar = unsigned char;


class MyRecvThread : public wxThread {
public:
    enum Status {
        S, M, E
    };
public:
    MyRecvThread(MyFrame *_handler, int _port);
    void *Entry() override;

    bool recv_photo_info(Server &server, int &recv_len);
    bool init_for_recv_photo_segment();
    bool recv_photo_segment(Server &server, int &recv_len, Status &s);

    bool start_thread();
private:
    int photo_id = 0;
    cv::Mat photo;
    MyFrame *handler;
    int w, h, copy_len, nline, port;
    uchar mode;
    uchar buffer[BUFFER_SIZE];
};

#endif //SERVERGUI_MYRECVTHREAD_H
