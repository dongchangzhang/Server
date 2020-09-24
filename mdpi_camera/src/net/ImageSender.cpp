#include <utility>

//
// Created by vhdsih on 2020/1/24.
//


#include "ImageSender.h"
#include "../data/VCDU.h"
#include "../value/values.h"
#include "Client.h"
#include "../ui/Home.h"
#include <cmath>

ImageSender::ImageSender(wxFrame* _handler, const wxImage &_image, const DataInjection &_dij, const GNC &_gnc, std::string _ip, int _port)
    : handler(_handler), dij(_dij), gnc(_gnc), ip(std::move(_ip)), port(_port) {

    index = 0;
    info_pkg_len = 28;
    segment_header_len = 18;

    short i = 0, j = 0, ni = 0, nj = 0, gap = 1;
    dij.get_windows_params(i, j, ni, nj);

    wxImage image;
    std::cout << j << " " << i << " " << nj << " " << ni << std::endl;
    wxRect rect(j, i, nj, ni);


    // get image to be send
    switch (dij.get_image_mode()) {
        case 0x00:
            gap = 2;
            npkg = 3072 * 2;
            segment_body_len = 4096 / 2;
            image = _image.Copy();
            break;
        case 0x11:
            npkg = 3072 / 2;
            segment_body_len = 4096 / 2;
            image = _image.Scale(4096 / 2, 3072 / 2, wxIMAGE_QUALITY_HIGH).Copy();
            break;
        case 0x22:
            npkg = 3072 / 4;
            segment_body_len = 4096 / 4;
            image = _image.Scale(4096 / 4, 3072 / 4, wxIMAGE_QUALITY_HIGH).Copy();
            break;
        case 0x33:
            npkg = 3072 / 8;
            segment_body_len = 4096 / 8;
            image = _image.Scale(4096 / 8, 3072 / 8, wxIMAGE_QUALITY_HIGH).Copy();
            break;
        case 0x44:
            npkg = ni;
            segment_body_len = nj;
            if (ni == 0 || nj == 0 || i < 0 || j < 0 || ni < 0 || nj < 0 ||
            i + ni >= 3072 || j + nj >= 4096) {
                can_send = false;
                return;
            }
            image = _image.GetSubImage(rect);
            break;
        default:
            gap = 2;
            npkg = 3072 * 2;
            segment_body_len = 4096 / 2;
            image = _image.Copy();
            break;
    }
    std::cout <<image.GetWidth() << " - * - " << image.GetHeight() << std::endl;
    // get single pixel r/g/b
    index = 0;
    unsigned char a, b, c;
    unsigned int value = 0;
    bool first_element = true;
    auto &pixels_buffer = get_pixels_buffer();
    memset(pixels_buffer, 0, PIXELS_BUF_MAX_LEN);

    for (int k = 0; k < image.GetWidth() * image.GetHeight(); ++k) {
        if (k % image.GetWidth() == 0) {
            nchannel = -1;
            if (value > 0) {
                b = value & 0xFFu;
                value >>= 8u;
                a = value & 0xFFu;
                pixels_buffer[index++] = a;
                pixels_buffer[index++] = b;
                value = 0;
            }
        }
        nchannel = turn(k / image.GetWidth(), nchannel);
        if (first_element) {
            value = 16 * int(image.GetData()[k * 3 + nchannel]);
        } else {
            value <<= 12u;
            value += 16 * int(image.GetData()[k * 3 + nchannel]);

            c = value & 0xFFu;
            value >>= 8u;
            b = value & 0xFFu;
            value >>= 8u;
            a = value & 0xFFu;
            value = 0u;

            pixels_buffer[index++] = a;
            pixels_buffer[index++] = b;
            pixels_buffer[index++] = c;
        }
        first_element = !first_element;
    }
    if (value > 0) {
        b = value & 0xFFu;
        value >>= 8u;
        a = value & 0xFFu;
        pixels_buffer[index++] = a;
        pixels_buffer[index++] = b;
    }

    // add info and herder for every pkg
    auto &sending_buffer = get_sending_buffer();
    memset(sending_buffer, 0, SEND_BUF_MAX_LEN);
    // copy info
    index = 0;
    init_info();
    memcpy(&sending_buffer[index], info_buffer, info_pkg_len);
    index += info_pkg_len;

    unsigned char _cols_flag, _head_tail_flag;
    int copy_len = int(segment_body_len * 1.5 + 0.5);
    auto &nframe = get_frame_count_from_restart();
    for (short p = 0; p < npkg; ++p) { // rows - 0 - 3072?
        // copy header
        _cols_flag = p % gap == 0 ? 0x55 : 0xAA;
        _head_tail_flag = p == 0 ? 0x55 : p + 1 == npkg? 0xAA : 0x00;
        init_seg_header(nframe++, p, p / gap, _cols_flag, _head_tail_flag);

        memcpy(&sending_buffer[index], seg_header_buffer, segment_header_len);
        index += segment_header_len;

        // copy data
        memcpy(&sending_buffer[index], &pixels_buffer[p * copy_len], copy_len);
        index += copy_len;
    }
}


unsigned int ImageSender::send() {
    if (!can_send) return 0;
    std::cout << "in send !!!" << std::endl;
    Client client(ip, port);
    unsigned int nvcdu = 0;
    auto &sending_buffer = get_sending_buffer();

    while (!((Home *) handler)->set_pos(0)) wxMicroSleep(1);

    int block = index / VCDU_DATA_LEN / 100;
    if (block == 0) block = 1;

    for (int i = 0; i < index; i += VCDU_DATA_LEN) {
        VCDU vcdu(&sending_buffer[i], nvcdu++);
        client.send((char*)vcdu.data, VCDU_LEN);
        if (i % (block * VCDU_DATA_LEN) == 0) {
            while (!((Home *) handler)->set_pos(int(i / float(index) * 100))) wxMicroSleep(1);
            auto *event = new wxThreadEvent(wxEVT_THREAD, ID_SENDING_THREAD_UPDATE);
            wxQueueEvent(handler->GetEventHandler(), event);
        }
        wxMicroSleep(500);
    }

    while (!((Home *) handler)->set_pos(100)) wxMicroSleep(1);
    auto *event = new wxThreadEvent(wxEVT_THREAD, ID_SENDING_THREAD_UPDATE);
    wxQueueEvent(handler->GetEventHandler(), event);

    return nvcdu;
}

void ImageSender::init_info() {
    short i = 0, j = 0, ni = 0, nj = 0;
    dij.get_windows_params(i, j, ni, nj);
    info_buffer[0] = 0x55;
    info_buffer[1] = 28;
    info_buffer[2] = dij.get_work_mode();
    info_buffer[3] = 0x11;
    info_buffer[4] = dij.get_image_mode();
    memcpy(&info_buffer[5], &i, 2);
    memcpy(&info_buffer[7], &j, 2);
    memcpy(&info_buffer[9], &ni, 2);
    memcpy(&info_buffer[11], &nj, 2);
    memcpy(&info_buffer[13], gnc.get_time_raw(), 6);
    info_buffer[19] = 0;
    info_buffer[20] = 0;
    info_buffer[21] = 0;
    info_buffer[22] = 0;
    info_buffer[23] = 0;
    info_buffer[24] = 0;
    info_buffer[25] = 0;
    info_buffer[26] = 0;
    info_buffer[27] = 0;
}

void ImageSender::init_seg_header(unsigned char nframe, short _npkg, short _nline, char cols_flag, char head_tail_flag) {
    seg_header_buffer[0] = 0xAA;
    short len = segment_header_len + segment_body_len;
    memcpy(&seg_header_buffer[1], &len,2);
    memcpy(&seg_header_buffer[3], gnc.get_time_raw(), 6);
    seg_header_buffer[9] = dij.get_image_mode();
    seg_header_buffer[10] = nframe;
    seg_header_buffer[11] = 0;
    short num = 0;
    memcpy(&seg_header_buffer[12], &_npkg,2);
    memcpy(&seg_header_buffer[14], &_nline,2);
    seg_header_buffer[16] = cols_flag;
    seg_header_buffer[17] = head_tail_flag;
}

int ImageSender::turn(int nline, int now) {
    // all b - g - r
    // rg r - g: 2 1 2 1 2 1 ... [line 0 2 4 8] -> 0 1 0 1 0 1 0 1 0 1
    // gb g - b: 1 0 1 0 1 0 ... [line 1 3 5 7] -> 1 2 1 2 1 2 1 2 1 2
    int ret = 0;
    if (now == -1) {
        ret = nline % 2 == 0 ? 0 : 1;
    } else if (nline % 2 == 0) {
        ret = now == 1 ? 0 : 1;
    } else {
        ret = now == 1 ? 2 : 1;
    }
    return ret;
}

