//
// Created by z on 19-4-29.
//

#ifndef SERVERGUI_MYRECVTHREAD_H
#define SERVERGUI_MYRECVTHREAD_H

#include <wx/wx.h>
#include <string>
#include <fstream>

#include "../constants.h"
#include "../net/Server.h"

const int NITERS_TO_UPDATE_UI = 512;
using uchar = unsigned char;


class MyRecvThread : public wxThread {
public:
    enum Status {
        S, M, E
    };
public:
    MyRecvThread(MyFrame *_handler, int _port, std::string _output_dir="./output", int base=10000);


    void *Entry() override;

    bool recv_photo_info(Server &server, int &recv_len);
    bool init_for_recv_photo_segment();
    bool recv_photo_segment(Server &server, int &recv_len, Status &s);

    bool init();

    void write_info(u_char my_buffer[], int from, int len);

    bool start_thread();

private:
    int base = 0;
    int photo_id = 0, photo_save_id = 0;
    cv::Mat photo;
    MyFrame *handler;
    int w, h, copy_len, nline, port;
    uchar mode;
    uchar buffer[BUFFER_SIZE];


    std::string output_dir;
};

#endif //SERVERGUI_MYRECVTHREAD_H
