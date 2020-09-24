//
// Created by vhdsih on 2019/3/31.
//

#include <cstring>
#include <cstdio>
#include "DataInjection.h"
#include "../value/const_values.h"


bool DataInjection::parser(u_char *buffer) {
    int offset = 0;
    u_char cmd = buffer[0];
    if (cmd != 0x04) {
        std::cerr << "error dij" << std::endl;
    }
    u_char len = buffer[offset + 1];
    unsigned short i_tmp, j_tmp, ni_tmp, nj_tmp;
    switch (buffer[offset + 2]) {
        case 0x11: // work mode
            if (buffer[offset + 3] == 0x11 || buffer[offset + 3] == 0x22) {
                printf("::set work mode 0x%x\n", buffer[offset + 3]);
                work_mode = buffer[offset + 3];
            } else {
                std::cerr << "::wrong work mode " << std::endl;
            }
            break;
        case 0x33: // image mode
            if (buffer[offset + 3] == 0x00 || buffer[offset + 3] == 0x11 || buffer[offset + 3] == 0x22 || buffer[offset + 3] == 0x33 || buffer[offset + 3] == 0x44) {
                image_mode = buffer[offset + 3];
                printf("::set image mode 0x%x\n", buffer[offset + 3]);
            } else {
                std::cerr << "::wrong image mode " << std::endl;
            }
            break;
        case 0x44:
            i_tmp = bytes_2_short(&buffer[offset+3]);
            j_tmp = bytes_2_short(&buffer[offset+5]);
            ni_tmp = bytes_2_short(&buffer[offset+7]);
            nj_tmp = bytes_2_short(&buffer[offset+9]);
            if (i_tmp < 3072 && j_tmp < 4096 && i_tmp + ni_tmp < 3072 && j_tmp + nj_tmp < 4096) {
                std::cout << "::setting window args" << std::endl;
                memcpy(setting_window, &buffer[offset + 3], sizeof(setting_window));
            } else {
                std::cerr << "::error on window args" << std::endl;
            };
            break;
        case 0x66:
            memcpy(auto_expo, &buffer[offset + 3], 5);
            break;
        case 0xAA:
            printf("::sending photo mode\n");
            if (work_mode == 0x22) {
                SEND_PHOTO = true;
                printf("::camera working now, sending image? yes!\n");
            } else if (work_mode == 0x11) {
                printf("::camera not working now, sending image? no!\n");
            } else {
                printf("::error\n");
            }
            break;

        default:
            std::cerr << "::error parser dij!" << std::endl;
            return false;
    }
    return true;
}


int DataInjection::get_dij_into_buffer(unsigned char *buffer, int max_len) {

    memset(buffer, 0, max_len);
    buffer[0] = HEAD1;
    buffer[1] = HEAD2;
    buffer[2] = DIJ_CMD;

    // buffer[3] waiting


    // 1 工作模式 1B
    int start_addr = 4;
    memcpy(&buffer[start_addr], &work_mode, sizeof(work_mode));
    start_addr += sizeof(work_mode);

    // 2 曝光设置 1B
    memcpy(&buffer[start_addr], &expo_mode, sizeof(expo_mode));
    start_addr += sizeof(expo_mode);

    // 3 图像模式 1B
    memcpy(&buffer[start_addr], &image_mode, sizeof(image_mode));
    start_addr += sizeof(image_mode);

    // 4 开窗设置 9B
    memcpy(&buffer[start_addr], setting_window, sizeof(setting_window));
    start_addr += sizeof(setting_window);

    // 5 手动曝光参数 3B
    memcpy(&buffer[start_addr], manual_expo, sizeof(manual_expo));
    start_addr += sizeof(manual_expo);

    // 6 自动曝光参数 5B
    memcpy(&buffer[start_addr], auto_expo, sizeof(auto_expo));
    start_addr += sizeof(auto_expo);

    // 7 镜头温度参数 7B
    memcpy(&buffer[start_addr], temp, sizeof(temp));
    start_addr += sizeof(temp);

    // 8 CMOS 3B
    memcpy(&buffer[start_addr], cmos_1, sizeof(cmos_1));
    start_addr += sizeof(cmos_1);

    // 9 CMOS参数 3B
    memcpy(&buffer[start_addr], cmos_2, sizeof(cmos_2));
    start_addr += sizeof(cmos_2);

    // 10 CMOS 1B
    memcpy(&buffer[start_addr], &cmos_3, sizeof(cmos_3));
    start_addr += sizeof(cmos_3);

    // 11 SDRAM 1B
    memcpy(&buffer[start_addr], &sdram, sizeof(sdram));
    start_addr += sizeof(sdram);

    // 12 1B
    memcpy(&buffer[start_addr], &_p, sizeof(_p));
    start_addr += sizeof(_p);

    // len
    buffer[3] = start_addr - 4;

    // checksum
    unsigned short checksum = 0;

    for (int i = 2; i < start_addr; ++i) {
        checksum += buffer[i];
    }

    buffer[start_addr] = checksum >> 8;
    buffer[start_addr + 1] = checksum & 255;
    return start_addr + 2;
}

