//
// Created by vhdsih on 2019-06-16.
//


#include "Constructer.h"
#include "../utils/tools.h"
#include "../model/camera/CameraModel.h"
#include "../model/data/GNC.h"
#include "../model/data/DataInjection.h"
#include "MainFrame.h"

#include <iostream>
#include <unordered_set>
#include <fstream>
#include <queue>
#include <mutex>
#include <thread>
#include <cstring>


static bool save_stop = false;
//static std::mutex mtx;
//static std::queue<ImageData> image_queue;
//static std::queue<std::string> image_name;


//void save_image_helper(ImageData img, std::string &name) {
//    try {
//        img.to_wxImage().SaveFile(name);
//    } catch (...) {
//        std::cout << "catch (...)" << std::endl;
//    }
//
//}
//void save_image() {
//    std::chrono::milliseconds level(100);
//    while (!save_stop || !image_queue.empty()) {
//        if (image_queue.empty()) {
//            std::this_thread::sleep_for(level);
//            continue;
//        }
//
//        mtx.lock();
//        auto img = image_queue.front();
//        auto image_path = image_name.front();
//        image_queue.pop();
//        image_name.pop();
//        mtx.unlock();
//        std::thread th(save_image_helper, img, std::ref(image_path));
//        th.detach();
//    }
//    std::cout << "out thread" << std::endl;
//}

Constructer::Constructer(MainFrame *_handler, int idx, ImageData &_canvas) :
        handler(_handler), index(idx), canvas(_canvas), wxThread(wxTHREAD_JOINABLE) {

}

void *Constructer::Entry() {
    GNC gnc;
    wxImage image;
    CameraModel camera;
    DataInjection commands;
    std::vector<std::string> files;
    std::string images_path(handler->get_image_path());
    std::string head, tail, img_file, info_file;

    int tail_len = std::string(INFO_TAIL).size();

    get_files(images_path, files);

    int n_files = statics(files);


    // send msg to main frame
    handler->set_now(std::max(0, index - 1));
    handler->set_all(n_files);
    handler->set_ratio(float(std::max(0, index-1)) / n_files);
    handler->set_canvas(canvas);
    handler->set_index(0);
    auto *start = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
    wxQueueEvent(handler->GetEventHandler(), start);

    save_stop = false;


    for (int i = index; i < files.size(); ++i) {
        if (TestDestroy()) return nullptr;
        const std::string file = files[i];
        tail = file.substr(file.size() - tail_len);
        head = file.substr(0, file.size() - tail_len);

        info_file = images_path + "/" + file;
        img_file = images_path + "/"  + head + IMG_TAIL;

        try {
            image.LoadFile(img_file);
        } catch (...) {
            continue;
        }

        if (image.GetHeight() == 0 || image.GetWidth() == 0) {
            std::cout << "Error Img" << std::endl;
            continue;
        }
        // read gnc
        u_char gnc_buffer[128];
        memset(gnc_buffer, 0, 128);

        std::ifstream is(info_file, std::ios::in | std::ios::binary);
        is.read((char*)gnc_buffer, GNC_BUF_LEN);
        is.close();

        gnc.set_gnc(gnc_buffer);

        // draw canvas
        camera.run_camera(gnc, commands, image, canvas);
        // send msg to main frame
        handler->set_now(i + 1);
        handler->set_ratio(float(i) / n_files);
        handler->set_canvas(canvas);
        handler->set_index(i + 1);
        auto *update = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
        wxQueueEvent(handler->GetEventHandler(), update);
    }
    // send msg to main frame
    handler->set_now(n_files);
    handler->set_ratio(1);
    auto wx_canvas_3 = canvas.to_wxImage();
    handler->set_canvas(canvas);
    auto *end = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
    wxQueueEvent(handler->GetEventHandler(), end);
    return nullptr;
}

bool Constructer::stop() {
    std::cout << "to stop" << std::endl;
//    this->Wait();
    this->Delete();
    return false;
}

bool Constructer::start() {
    this->Create();
    this->Run();
    return true;
}

int Constructer::statics(std::vector<std::string> &files) {
    int n = 0;
    std::string head, tail, img_file;
    int tail_len = std::string(INFO_TAIL).size();
    std::vector<std::string> legal_files;
    std::unordered_set<std::string> set(files.begin(), files.end());


    for (auto file : files) {
        // illegal file name
        if (file == "." || file == ".." || file.size() <= tail_len) {
            continue;
        }
        tail = file.substr(file.size() - tail_len);
        // not info file
        if (tail != INFO_TAIL) {
            continue;
        }
        head = file.substr(0, file.size() - tail_len);
        img_file = head + IMG_TAIL;
        if (set.find(img_file) != set.end()) {
            ++n;
            legal_files.emplace_back(file);
        }
    }
    std::sort(legal_files.begin(), legal_files.end());
    files = legal_files;
    return n;
}




