//
// Created by idz on 2019/3/31.
//

#ifndef MARSSIMULATOR_ANSWER_H
#define MARSSIMULATOR_ANSWER_H


class Answer {
    char head1 = 0xeb;
    char head2 = 0x90;
    char command_type;
    char answer_code;
};

class AnswerPooling {
    // 需要仿真： y； 不需要仿真： n
    char head1 = 0xeb;    // 包头    y
    char head2 = 0x90;    // 包头    y
    char command_type;    // 命令类型    y
    char valid_data_len;    // 有效的数据长度    y

    char nfram_received_yes;    // 正确接收数据注入的帧数    y
    char nfram_received_no;    // 错误接收数据注入的帧数    y

    char work_mode;     // 工作模式    y
    char expo_mode;     // 曝光模式    y
    char image_mode;    // 图片模式    y
    short start_row;    // 开窗起始行号    y
    short start_col;    // 开窗起始列号    y
    short row_num;    // 窗口行数    y
    short col_num;    // 窗口列数    y
    short expo_time;    // 曝光时间   y
    char cmos_gain;    // cmos增益    n
    short ave_gray_exp;    // 自动曝光平均灰度期望值    n
    short ave_gray_cal;    // 自动曝光平均灰度计算值    n
    short expo_constraint;    // 自动曝光时间约束    n
    char expo_bkg_threshold;    // 自动曝光背景阈值    n
    char frame_num;    // 输出图像帧计数    n
    char cmos_ele[3];    //  CMOS 电流监控    n
    char cmos_voltage; // cmos 电压    n
    short temperature_control; // 镜头温度监控    n
    char thermal_stat;    // 热控开关状态    n
    short cmos_temperature;    // cmos 温度    n
    char reg[3];    // 寄存器    n
    char SDRAM;    //SDRAM    n
    short except;    // 异常    n
    char time_code[6];    // 时间码    n
    char what; // 备用    n
    short checksum; // 校验   y
};


#endif //MARSSIMULATOR_ANSWER_H
