//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_SENDIMAGE_H
#define MDPI_CAMERA_SENDIMAGE_H


#include <wx/wx.h>
#include "ThreadBase.h"

class SendImage : public ThreadBase {
public:
    explicit SendImage(wxWindow *_handler, std::string ip, int port);
    void *Entry() override;

private:
    int port;
    std::string ip;
};


#endif //MDPI_CAMERA_SENDIMAGE_H
