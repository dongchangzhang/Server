//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_VCDU_H
#define MDPI_CAMERA_VCDU_H



#include <cstring>

using u_char = unsigned char;

class VCDU {
public:
    VCDU() = delete;

    VCDU(u_char _data[], unsigned long long count) {
        memcpy(&data[from], _data, len);
        // len
        char mask = 0xFFu;
        data[8] = count & mask;
        count >>= 8u;
        data[7] = count & mask;
        count >>= 8u;
        data[6] = count & mask;
    }

public:
    u_char data[896];

private:
    inline void init() {
        // sync
        data[0] = 0x1A;
        data[1] = 0xCF;
        data[2] = 0xFC;
        data[3] = 0x1D;

        // main channel && virtual channel
        data[4] = (1u << 6u) + (0x92u >> 2u);
        data[5] = (0x92u << 6u) & 0xFFu + 0x11u;

        // count
        // 3B data[6, 7, 8]
        // flag
        data[9] = 0x0;
        // time
        data[10] = 0x0;
        data[11] = 0x0;
        data[12] = 0x0;
        data[13] = 0x0;
        data[14] = 0x0;
        data[15] = 0x0;
        // other
        data[16] = 0b10111010;
        data[17] = 0b10101010;
        // main_data 18 to 895;
    }
    const int len = 878;
    const int from = 18;

};
#endif //MDPI_CAMERA_VCDU_H
