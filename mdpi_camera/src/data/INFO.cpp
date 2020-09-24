//
// Created by idz on 2019/3/31.
//

#include <string.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "INFO.h"

bool INFO::get_info(u_char *p, int &_len, int max_len, uchar photo_id) {
    if (p == nullptr && max_len < 28) {
        len = 0;
        return false;
    }
    _len = 28;
    frame_num = photo_id;
    memset(p, 0, max_len);
    memcpy(&p[0], &type, 1);
    memcpy(&p[1], &len, 1);
    memcpy(&p[2], &work_mode, 1);
    memcpy(&p[3], &expo_mode, 1);
    memcpy(&p[4], &image_mode, 1);
    memcpy(&p[5], &start_row, sizeof(short));
    memcpy(&p[7], &start_col, sizeof(short));
    memcpy(&p[9], &row_num, sizeof(short));
    memcpy(&p[11], &col_num, sizeof(short));
    memcpy(&p[13], &expo_time, 6);
    memcpy(&p[19], &expo_continue, sizeof(short));
    memcpy(&p[21], &cmos_gain, 1);
    memcpy(&p[22], &ave_gray_exp, sizeof(short));
    memcpy(&p[24], &ave_gray_cal, sizeof(short));
    memcpy(&p[26], &frame_num, 1);
    memcpy(&p[27], &SDRAM, 1);
    return true;
}

void INFO::set_commands(DataInjection _commands, short _w, short _h) {
    commands = _commands;
    work_mode = commands.get_work_mode();
    expo_mode = commands.get_expo_mode();
    image_mode = commands.get_image_mode();
    auto it = commands.get_setting_windows();
    memcpy(&start_row, &it[0], 2);
    memcpy(&start_col, &it[2], 2);

    // open window
    if (image_mode == 0x44) {
        memcpy(&row_num, &it[4], 2);
        memcpy(&col_num, &it[6], 2);
        if (row_num != _h || col_num != _w) {
            std::cerr << " Error w & h in INFO.cpp " << std::endl;
            row_num = _h;
            col_num = _w;
        }
    } else { // set image h & w
        row_num = _h;
        col_num = _w;
    }
}
