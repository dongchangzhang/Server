//
// Created by idz on 2019/3/31.
//

#ifndef MARSSIMULATOR_DATAINJECTION_H
#define MARSSIMULATOR_DATAINJECTION_H


#include <cstring>

class DataInjection {

    // 需要仿真： y；   无需仿真： n
    char work_mode = 0x22;    // 工作模式    y
    char expo_mode;    // 曝光模式    y
    char image_mode;    // 图像模式    y
    char setting_window[9];    // 开窗设置    y
    char manual_expo[3];    // 手动曝光参数设置    y
    char auto_expo[5];    // 自动曝光参数设置    y


public:
    DataInjection() = default;

    int get_dij_into_buffer(unsigned char buffer[]);

    inline void set_work_mode(bool running) {
        if (running) {
            work_mode = 0x22;
        } else {
            work_mode = 0x11;
        }
    }

    inline void set_image_mode(int mode) {
        switch (mode) {
            case 1: image_mode = 0x00; break;
            case 2: image_mode = 0x11; break;
            case 3: image_mode = 0x22; break;
            case 4: image_mode = 0x33; break;
            case 5: image_mode = 0x44; break;
            default: image_mode = 0x00;
        }
    }

    inline void set_window(short i, short j, short ni, short nj) {
        if (i < 0 || j < 0 || ni < 0 || nj < 0 ) {
            image_mode = 0x00;
        }
        if (i + ni >= 3072 || j + nj >= 4096) {
            image_mode = 0x00;
        }
        memcpy(&setting_window[0], &i, 2);
        memcpy(&setting_window[2], &j, 2);
        memcpy(&setting_window[4], &ni, 2);
        memcpy(&setting_window[6], &ni, 2);

    }

    inline void set_expo(short t) {
        memcpy(&auto_expo[3], &t, 2);
    }

};


#endif //MARSSIMULATOR_DATAINJECTION_H
