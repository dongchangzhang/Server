//
// Created by z on 19-4-29.
//

#include "MyRecvThread.h"
#include "../utils/utils.h"


MyRecvThread::MyRecvThread(MyFrame *_handler, int _port) : port(_port), handler(_handler), wxThread(wxTHREAD_DETACHED),
                                                           w(0), h(0), copy_len(0), nline(0) {
    memset(buffer, 0, sizeof(buffer));
}

void *MyRecvThread::Entry() {
    int recv_len, count = 0;
    bool stop = false;
    Server server(port);
    while (!stop) {
        count = 0;
        while (!recv_photo_info(server, recv_len)) std::cout << "error info" << std::endl;
        init_for_recv_photo_segment();
        Status s = S;
        while (s != E) {
            ++count;
            recv_photo_segment(server, recv_len, s);
            if (count % NITERS_TO_UPDATE_UI == 0) {
                handler->mphoto = photo;
                auto *event = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
                event->SetInt(IMG_ID);
                wxQueueEvent(handler->GetEventHandler(), event);
            }
        }
    }
    return nullptr;
}

bool MyRecvThread::recv_photo_info(Server &server, int &recv_len) {
    if (server.recv_into_buff(recv_len, buffer)) {
        std::cout << "SERVER::Photo Getting Info ..." << std::endl;
        if (buffer[0] != 0x55) {
            std::cout << "not an image info segment" << std::endl;
            return false;
        }
    }
    return true;
}

bool MyRecvThread::init_for_recv_photo_segment() {
    int ni, nj;
    nline = 0;
    // image mode
    memcpy(&mode, &buffer[4], 1);
    if (mode != 0x44) {
        get_height_width(mode, h, w);
        copy_len = w * 3 / 2;
    } else if (mode == 0x44) {
        // open window
        memcpy(&ni, &buffer[9], 2);
        memcpy(&nj, &buffer[11], 2);
        h = ni;
        w = nj;
    } else {
        std::cout << "Image Mode Set Error, Need 0x00, 0x11, 0x22, 0x33 or 0x44!" << std::endl;
        return false;
    }
    // get data
    std::cout << "SERVER::Photo Getting content ..." << std::endl;
    photo = cv::Mat(h, w, CV_8UC3, cv::Scalar::all(0));
    return true;
}

bool MyRecvThread::recv_photo_segment(Server &server, int &recv_len, Status &s) {
    uchar where;
    int photo_id;
    if (!server.recv_into_buff(recv_len, buffer)) {
        return false;
    }
    if (buffer[0] != 0xAA) {
        std::cout << buffer[0] << "not an image line" << std::endl;
        return false;
    }
    memcpy(&photo_id, &buffer[10], sizeof(char));
    // open window
    if (mode == 0x44) {
        memcpy(&photo.data[nline * w * 3], &buffer[18], w * sizeof(char) * 3);
        ++nline;
        goto label;
    }
    // else
    memcpy(&nline, &buffer[12], sizeof(short));
    if (mode == 0x00) { /* full image */
        memcpy(&where, &buffer[16], 1);
        if (where == 0x55) {
            memcpy(&photo.data[nline * w * 3], &buffer[18], copy_len);
        } else {
            memcpy(&photo.data[nline * w * 3 + copy_len], &buffer[18], copy_len);
            ++nline;
        }

    } else {
        memcpy(&photo.data[nline * w * 3], &buffer[18], w * sizeof(char) * 3);
        ++nline;
    }
    s = M;
    label:
    memcpy(&where, &buffer[17], 1);
    if (where == 0xAA)
        s = E;
    return true;
}

bool MyRecvThread::start_thread() {
    this->Create();
    this->Run();
    return true;
}

