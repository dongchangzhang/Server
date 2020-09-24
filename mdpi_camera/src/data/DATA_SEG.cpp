//
// Created by idz on 2019/3/31.
//

#include "DATA_SEG.h"

bool DATA_SEG::get_info(u_char *p, int &_len, u_char where_stat, u_char flag, short nsegment, short nline, int nphoto) {
    if (p == nullptr)
        return false;
    len = _len;
    memcpy(&p[0], &type, 1);
    memcpy(&p[1], &len, sizeof(short));
    memcpy(&p[3], &expo_time, 6);
    memcpy(&p[9], &image_mode, 1);
    frame_num = nphoto;
    memcpy(&p[10], &frame_num, 1);
    memcpy(&p[11], &SDRAM, 1);
    memcpy(&p[12], &nsegment, sizeof(short));
    num_row = nline;
    memcpy(&p[14], &num_row, sizeof(short));
    memcpy(&p[16], &flag, 1);
    memcpy(&p[17], &where_stat, 1);
    return true;
}

void DATA_SEG::set_commands(DataInjection _commands) {
    commands = _commands;
    image_mode = commands.get_image_mode();
}
