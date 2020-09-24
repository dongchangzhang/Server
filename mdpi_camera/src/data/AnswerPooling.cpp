//
// Created by vhdsih on 19-10-14.
//

#include "AnswerPooling.h"

AnswerPooling::AnswerPooling() {
    memset(buffer, 0, 255);
}

int AnswerPooling::get_pooling(uchar buf[], int max_len, DataInjection &cmds, int m, int n) {
    memset(buf, 0, max_len);
    buf[0] = HEAD1;
    buf[1] = HEAD2;
    buf[2] = CHK_CMD;
    buf[3] = 46 - 1;

    // 正确帧数 1
    buf[4] = m;
    // 错误帧数 1
    buf[5] = n;
    // 工作模式 1
    buf[6] = cmds.get_work_mode();
    // 曝光模式 1
    buf[7] = cmds.get_expo_mode();
    // 图像模式
    buf[8] = cmds.get_image_mode();
    // 开窗设置  9 10 11 12 13 14
    long long win = 0;
    short _i, _j, _ni, _nj;
    cmds.get_windows_params(_i, _j, _ni, _nj);
    win = _i;
    win = (win << 12) + _j;
    win = (win << 12) + _ni;
    win = (win << 12) + _nj;
    memcpy(&buf[9], &win, 6);

    // 曝光时间 15 16
    buf[15] = 20;

    //cmos
    buf[17] = 1;

    // 灰度期望  18 19 20
    buf[18] = 0;
    buf[19] = 1;
    buf[20] = 21;

    // 自动曝光时间约束  21 22
    buf[21] = 0;
    buf[22] = 20;

    // 自动曝光背景阈值 23
    buf[23] = 128;
    // 输出图像帧数
    buf[24] = 1;
    // cmos 25 26 27
    // cmos 28
    // temp    29 30 31 32 33 34
    // 自主热控开关 35
    // cmos temp  36 37
    // cmos reg 38 39 40
    // sdram 41
    // exception   42 43
    // time   44 45 46 47 48 49


    // checksum
    unsigned short checksum = 0;

    for (int i = 2; i < 46 + 4; ++i) {

        checksum += buf[i];
    }


    buf[46 + 4] = checksum >> 8u;
    buf[46 + 5] = checksum & 0xFFu;


    return 52;
}
