//
// Created by vhdsih on 20-1-20.
//

#include "Camera.h"

#include "../ui/Home.h"
#include "../utils/utils.h"
#include "../value/values.h"
#include "../camera/CameraModel.h"

#include <fstream>

Camera::Camera(wxWindow *_handler) : ThreadBase(_handler) {
}

void *Camera::Entry() {
    int id = 0;
    CameraModel model;

    GNC gnc;
    wxImage image;
    DataInjection dij;

    // set params
    while (!TestDestroy()) {
        if (!read_camera_params(dij)) continue;
        if (dij.get_work_mode() != 0x22) {
            wxMilliSleep(10);
            continue;
        }
        if (!read_gnc(gnc)) {
            wxMilliSleep(1);
            continue;
        }
        ImageData image_data(IMAGE_WIDTH_GENERATE, IMAGE_HEIGHT_GENERATE);
#ifdef PRINT_INFO
        auto b = timestamp();
#endif
        try {
            model.run_camera(gnc, dij, image_data);
        } catch (...) {
            std::cout << "err" << std::endl;
            continue;
        }
#ifdef PRINT_INFO
        auto e = timestamp();
        auto diff = timediff(b, e);
        printf("image generated, using  %f ms :: %d\n", diff, id++);
#endif
        ((Home *) handler)->set_image(image_data); // update ui in home
        auto *event = new wxThreadEvent(wxEVT_THREAD, ID_IMAGE_THREAD_UPDATE);
        wxQueueEvent(handler->GetEventHandler(), event);
        if (dij.should_send() || dij.get_saving_mode()) {
            image = image_data.to_wxImage();
            image.Rescale(4096, 3072, wxIMAGE_QUALITY_HIGH);
        }
        if (dij.should_send()) {
            while (!write_send_information(image, gnc, dij)) {
                wxMilliSleep(1);
            }
            dij.set_sending_mode(false);
        }
        if (dij.get_saving_mode()) {
            std::string info_path = dij.get_saving_path() + "/" + to_string(id) + ".dat";
            write_info(gnc, info_path);
            image.SaveFile(dij.get_saving_path() + "/" + to_string(id) + ".jpeg");
        }
        while (!update_camera_params(dij, UPDATE_SENDING_MODE)) wxMilliSleep(1);
    }
    return nullptr;
}

void Camera::write_info(const GNC &gnc, const string &path) {
    std::ofstream of(path, std::ios::out | std::ios::binary | std::ios::trunc);
    memset(buffer, 0, 128);
    int copy_len = gnc.copy_gnc(buffer);
    of.write(buffer, copy_len);
    of.close();
}
