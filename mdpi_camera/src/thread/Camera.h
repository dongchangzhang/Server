//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_CAMERA_H
#define MDPI_CAMERA_CAMERA_H

#include "ThreadBase.h"
#include "../data/GNC.h"

class Camera : public ThreadBase {
public:
    explicit Camera(wxWindow *_handler);
    void *Entry() override;

private:
    char buffer[128];
    void write_info(const GNC &gnc, const std::string &path);

};
#endif //MDPI_CAMERA_CAMERA_H
