//
// Created by vhdsih on 2020/1/24.
//

#ifndef MDPI_CAMERA_IMAGESENDER_H
#define MDPI_CAMERA_IMAGESENDER_H

#include <wx/wx.h>
#include "../data/DataInjection.h"
#include "../data/GNC.h"

class ImageSender {
public:
    ImageSender(wxFrame *_handler, const wxImage &_image, const DataInjection &dij, const GNC &_gnc, std::string ip, int port);

    unsigned int send();

private:
    void init_info();
    void init_seg_header(unsigned char nframe, short _npkg, short _nline, char cols_flag, char head_tail_flag);
    int turn(int nline, int now);

private:
    bool can_send = true;
    wxFrame *handler;
    std::string ip;
    int port;
    GNC gnc;
    DataInjection dij;

    int index = 0;
    short npkg, nchannel;
    int info_pkg_len = 0;
    int segment_body_len = 0;
    int segment_header_len = 0;

    unsigned char info_buffer[28]{};
    unsigned char seg_header_buffer[28]{};
};


#endif //MDPI_CAMERA_IMAGESENDER_H
