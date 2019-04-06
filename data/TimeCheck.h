//
// Created by idz on 2019/3/31.
//

#ifndef MARSSIMULATOR_TIMECHECK_H
#define MARSSIMULATOR_TIMECHECK_H
#include "../constants.h"
#include <cstring>

class TimeCheck {
public:
    TimeCheck(){};
    bool update();
    int get_time_info(unsigned char buffer[]);
private:
    unsigned char head1 = 0xeb;    // 包头标识
    unsigned char head2 = 0x90;    // 包头标识
    unsigned char command_type = 0x1;    // 命令类型
    unsigned char valid_byte_num = 6;    // 有效的字节数目
    int second = 24;    // 时间码的秒计数
    short millisecond = 23;    // 时间码的毫秒计数
    unsigned char checksum_high;    // 累加和 高字节
    unsigned char checksum_low;    // 累加和 低字节

};


#endif //MARSSIMULATOR_TIMECHECK_H
