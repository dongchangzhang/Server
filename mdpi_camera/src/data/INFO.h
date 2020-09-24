//
// Created by idz on 2019/3/31.
//

#ifndef MARSSIMULATOR_INFO_H
#define MARSSIMULATOR_INFO_H

#include <cstring>
#include "DataInjection.h"

using u_char = unsigned char;

class INFO {
public:
    // 需要仿真： y；   无需仿真： n
    unsigned char type = 0x55;   // 包类型    y
    unsigned char len = 28;    // 包长度    y
    unsigned char work_mode = 0x22;    // 工作模式    y
    unsigned char expo_mode = 0x11;    // 曝光模式    y
    unsigned char image_mode = 0x22;    // 图像模式    y
    short start_row = 0;    // 开窗起始行号    y
    short start_col = 0;    // 开窗起始列数    y
    short row_num = 0;    // 行数    y
    short col_num = 0;    // 列数    y
    unsigned char expo_time[6];    // 曝光时刻    y
    short expo_continue;    // 曝光时间    y
    unsigned char cmos_gain;    // cmos 增益    n
    short ave_gray_exp = 0;    // 自动曝光平均灰度期望    y
    short ave_gray_cal = 0;    //  自动曝光平均灰度计算    y
    unsigned char frame_num = 0;    // 输出的图形帧号
    unsigned char SDRAM = 0;    // SDRAM    n
    DataInjection commands;

    INFO() {};


    INFO(u_char _image_mode) : image_mode(_image_mode) {};

    INFO(u_char wm, u_char em, u_char im, short sr, short sc, short rn, short cn, u_char et[6], short ec, short age,
         short agc, u_char fn) :
            work_mode(wm), expo_mode(em), image_mode(im), start_row(sr), start_col(sc), row_num(rn), col_num(cn),
            expo_continue(ec), ave_gray_exp(age), ave_gray_cal(agc), frame_num(fn) {
        memcpy(expo_time, et, 6);
    };

    ~INFO() {};

    bool get_info(u_char *p, int &len, int max_len, u_char photo_id);
    void set_image_mode(u_char _image_mode) {
        image_mode = _image_mode;
    }
    void set_commands(DataInjection _commands, short _w, short _h);
};


#endif //MARSSIMULATOR_INFO_H
