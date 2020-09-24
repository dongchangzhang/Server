//
// Created by idz on 2019/3/31.
//

#ifndef MARSSIMULATOR_DATA_SEG_H
#define MARSSIMULATOR_DATA_SEG_H

#include <opencv2/opencv.hpp>
#include "DataInjection.h"

class DATA_SEG {
private:
    // 需要仿真： y；   无需仿真： n
    unsigned char type = 0xAA;   //  包类型    y
    unsigned short len;    // 包长度     y
    unsigned char expo_time[6] = {0};    // 图像曝光时刻    y
    unsigned char image_mode=0x00;    // 图像模式    y
    unsigned char frame_num;    // 图像帧数    y
    unsigned char SDRAM;    // SDRAM    n
    short index;    // 图像数据包号    y
    short num_row;    // 行号    y
    unsigned char flag_col;    // 列标志    y
    unsigned char head_tail = 0x00;    // 尾标志    y
    DataInjection commands;
public:

    DATA_SEG() {}

    DATA_SEG(u_char et[6], u_char im, u_char fn) : image_mode(im), frame_num(fn) {
        memcpy(expo_time, et, 6);
    }

    ~DATA_SEG() {}

    bool get_info(u_char *p, int &_len, u_char where_stat, u_char flag, short nsegment, short nline, int nphoto);

    void set_image_mode(u_char _image_mode) {
        image_mode = _image_mode;
    }
    void set_commands(DataInjection _commands);
};


#endif //MARSSIMULATOR_DATA_SEG_H
