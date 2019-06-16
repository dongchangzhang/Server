//
// Created by idz on 2019/3/31.
//

#ifndef MARSSIMULATOR_DATAINJECTION_H
#define MARSSIMULATOR_DATAINJECTION_H


class DataInjection {
    char head1=0xeb;    // 包头标识
    char head2=0x90;    // 包头标识
    char command_type;    // 命令类型
    char valid_byte_num;    // 有效的字节数目

    // 需要仿真： y；   无需仿真： n
    char work_mode;    // 工作模式    y
    char expo_mode;    // 曝光模式    y
    char image_mode;    // 图像模式    y
    char setting_window[9];    // 开窗设置    y
    char manual_expo[3];    // 手动曝光参数设置    y
    char auto_expo[5];    // 自动曝光参数设置    y
    char temperature[3];    // 温度    n
    char cmos_ele[3];    // cmos 电流    n
    char cmos_work[3];   // cmos 工作参数读写    n
    char output_request;    // 输出请求    n
    char SDRAM;    // 禁用控制    n
    char temp_change;   // 温度传感器切换    n

    char checksum_high;    // 累加和 高字节
    char checksum_low;    // 累加和 低字节
};


#endif //MARSSIMULATOR_DATAINJECTION_H
