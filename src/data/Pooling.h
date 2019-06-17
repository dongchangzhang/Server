//
// Created by idz on 2019/3/31.
//

#ifndef MARSSIMULATOR_POOLING_H
#define MARSSIMULATOR_POOLING_H


class Pooling {
    char head1 = 0xeb;
    char head2 = 0x90;
    char command_type; // 命令类型
    char valid_byte_num;    // 有效长度
    long long data = 0x5A5A;
    char checksum_high;
    char checksum_low;
};


#endif //MARSSIMULATOR_POOLING_H
