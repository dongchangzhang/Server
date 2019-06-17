//
// Created by idz on 2019/3/31.
//

#ifndef MARSSIMULATOR_INFO_H
#define MARSSIMULATOR_INFO_H


class INFO {
    // 需要仿真： y；   无需仿真： n
    char type;   // 包类型    y
    char len;    // 包长度    y
    char work_mode;    // 工作模式    y
    char expo_mode;    // 曝光模式    y
    char image_mode;    // 图像模式    y
    short start_row;    // 开窗起始行号    y
    short start_col;    // 开窗起始列数    y
    short row_num;    // 行数    y
    short col_num;    // 列数    y
    char expo_time[6];    // 曝光时刻    y
    short expo_continue;    // 曝光时间    y
    char cmos_gain;    // cmos 增益    n
    short ave_gray_exp;    // 自动曝光平均灰度期望    y
    short ave_gray_cal;    //  自动曝光平均灰度计算    y
    char frame_num;    // 输出的图形帧号
    char SDRAM;    // SDRAM    n
    INFO() {};
    ~INFO();
};


#endif //MARSSIMULATOR_INFO_H
