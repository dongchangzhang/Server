//
// Created by z on 19-4-29.
//

#include "ImageWorker.h"
#include "../utils/utils.h"
#include <thread>

void save_image(cv::Mat &img, std::string output_dir, int photo_save_id) {
    std::string image_path = output_dir + "/" + std::to_string(photo_save_id) + ".png";
    cv::imwrite(image_path, img);
}

ImageWorker::ImageWorker(MainFrame *_handler, int _port,
                           std::string _output_dir, int _base) :
        port(_port),
        output_dir(_output_dir),
        base(_base),
        handler(_handler),
        photo_save_id(base),
        wxThread(wxTHREAD_DETACHED),
        w(0), h(0), copy_len(0), nline(0) {

    memset(buffer, 0, sizeof(buffer));
    init();
}

void *ImageWorker::Entry() {
    int recv_len, count = 0;
    Server server(port);
    while (!TestDestroy()) {
        count = 0;
        while (!recv_photo_info(server, recv_len)) std::cout << "error info" << std::endl;

        init_for_recv_photo_segment();
        handler->ratio = 0;
        auto *start_recv = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
        start_recv->SetInt(IMG_ID);
        wxQueueEvent(handler->GetEventHandler(), start_recv);

        Status s = S;
        while (s != E) {
            ++count;
            recv_photo_segment(server, recv_len, s);
            if (count % (NITERS_TO_UPDATE_UI) == 0) {
                handler->ratio = float(count / 2) / h;
                auto *seg_sended = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
                seg_sended->SetInt(IMG_ID);
                wxQueueEvent(handler->GetEventHandler(), seg_sended);
            }
        }
        handler->ratio = 1;
        handler->mphoto = photo.clone();
        auto *end = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
        end->SetInt(IMG_ID);
        wxQueueEvent(handler->GetEventHandler(), end);

        std::thread th(save_image, std::ref(photo), output_dir, photo_save_id);
        th.detach();

        ++photo_save_id;
    }
    return nullptr;
}

bool ImageWorker::recv_photo_info(Server &server, int &recv_len) {
    if (server.recv_into_buff(recv_len, buffer)) {
        memcpy(&handler->nframe, &buffer[26], 1);
        if (buffer[0] != 0x55) {
            std::cout << "not an image info segment" << std::endl;
            return false;
        }
    }
    return true;
}

bool ImageWorker::init_for_recv_photo_segment() {
    short ni, nj;
    nline = 0;
    // image mode
    memcpy(&mode, &buffer[4], 1);
    switch (mode) {
        case 0x00:
        case 0x11:
        case 0x22:
        case 0x33:
        case 0x44:
            memcpy(&ni, &buffer[9], 2);
            memcpy(&nj, &buffer[11], 2);
            h = ni;
            w = nj;
            copy_len = w * 3 / 2;
            break;
        default:
            std::cout << "Image Mode Set Error, Need 0x00, 0x11, 0x22, 0x33 or 0x44!" << std::endl;
            return false;
    }
    std::cout << h << " - h - w - " << w << std::endl;

    memcpy(&photo_id, &buffer[26], 2);
    // get data
    photo = cv::Mat(h, w, CV_8UC3, cv::Scalar::all(0));

    // for another app
    write_info(buffer, 28, 232 + 3 * sizeof(double));

    return true;
}

bool ImageWorker::recv_photo_segment(Server &server, int &recv_len, Status &s) {
    uchar where;
    int photo_id;
    if (!server.recv_into_buff(recv_len, buffer)) {
        return false;
    }
    memcpy(&nline, &buffer[14], sizeof(short));
    if (buffer[0] != 0xAA) {
        return false;
    }

    memcpy(&photo_id, &buffer[10], sizeof(char));

    memcpy(&nline, &buffer[12], sizeof(short));
    if (mode == 0x00) { /* full image */
        memcpy(&where, &buffer[16], 1);
        if (where == 0x55) {
            memcpy(&photo.data[nline * w * 3], &buffer[18], copy_len);
        } else {
            memcpy(&photo.data[nline * w * 3 + copy_len], &buffer[18], copy_len);
        }
    } else {
        memcpy(&photo.data[nline * w * 3], &buffer[18], w * sizeof(char) * 3);
    }
    s = M;
    memcpy(&where, &buffer[17], 1);
    if (where == 0xAA) {
        s = E;
    }
    return true;
}

bool ImageWorker::start_thread() {
    this->Create();
    this->Run();
    return true;
}

bool ImageWorker::init() {
    if (output_dir.empty()) {
        return false;
    }
    return false;
}

void ImageWorker::write_info(u_char my_buffer[], int from, int len) {
    std::string info_path = output_dir + "/" + std::to_string(photo_save_id) + ".dat";
    std::ofstream of(info_path, std::ios::out | std::ios::binary | std::ios::trunc);
    of.write((char*)&photo_save_id, sizeof(photo_save_id));
    of.write((char*)&my_buffer[from], len);
    of.close();
}

bool ImageWorker::stop_thread() {
    this->Delete();
    return true;
}
