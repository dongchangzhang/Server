//
// Created by idz on 2019/3/31.
//

#ifndef MARSSIMULATOR_DATAINJECTION_H
#define MARSSIMULATOR_DATAINJECTION_H


#include <zconf.h>
#include <iostream>
#include "../utils/utils.h"

class DataInjection {
public:
    DataInjection() = default;
    ~DataInjection() = default;

    bool parser(u_char *buffer);

    inline void set_work_mode(u_char _work_mode);
    inline void set_image_mode(u_char _image_mode);
    inline void set_window_args(short i, short j, short ni, short nj);
    inline void set_sending_mode(bool _send);
    inline void set_saving_info(bool mode, std::string path="");

    inline short get_expo_time() const;
    inline u_char get_work_mode() const;
    inline u_char get_expo_mode() const;
    inline u_char get_image_mode() const;
    inline u_char* get_setting_windows();
    inline void get_windows_params(short &i, short &j, short &ni, short &nj) const;
    inline bool get_saving_mode() const;
    inline std::string get_saving_path() const;


    int get_dij_into_buffer(unsigned char *buffer, int max_len);

    bool is_right() const;
    bool should_send() const;


private:
    bool right = false;
    bool SEND_PHOTO = false;

    u_char work_mode = 0x11;    // 工作模式    y 1
    u_char expo_mode = 0x11;    // 曝光模式    y 2
    u_char image_mode = 0x00;    // 图像模式    y 3
    u_char setting_window[9];    // 开窗设置    y 4
    u_char manual_expo[3];    // 手动曝光参数设置    y 5
    u_char auto_expo[5];    // 自动曝光参数设置    y 6
    u_char temp[7] ={0};
    u_char cmos_1[3];
    u_char cmos_2[3];
    u_char cmos_3;
    u_char sdram;
    u_char _p;

    bool saving_mode = false;
    std::string saving_path;
};

inline void DataInjection::set_work_mode(u_char _work_mode) {
    work_mode = _work_mode;
}

inline void DataInjection::set_image_mode(u_char _image_mode) {
    image_mode = _image_mode;
}
inline void DataInjection::set_window_args(short i, short j, short ni, short nj) {
    short_2_bytes(i, &setting_window[0]);
    short_2_bytes(j, &setting_window[2]);
    short_2_bytes(ni, &setting_window[4]);
    short_2_bytes(nj, &setting_window[6]);
}

inline void DataInjection::set_sending_mode(bool _send) {
    SEND_PHOTO = _send;
}

short DataInjection::get_expo_time() const {
    short tmp = 0;
    memcpy(&tmp, &auto_expo[2], 2);
    return tmp;
}

u_char DataInjection::get_work_mode() const {
    return work_mode;
}

u_char DataInjection::get_expo_mode() const {
    return expo_mode;
}

u_char DataInjection::get_image_mode() const {
    return image_mode;
}
inline void DataInjection::get_windows_params(short &i, short &j, short &ni, short &nj) const {
    i = bytes_2_short(&setting_window[0]);
    j = bytes_2_short(&setting_window[2]);
    ni = bytes_2_short(&setting_window[4]);
    nj = bytes_2_short(&setting_window[6]);
}
inline u_char* DataInjection::get_setting_windows() {
    return setting_window;
}

inline bool DataInjection::is_right() const {
    return right;
}
inline bool DataInjection::should_send() const {
    return SEND_PHOTO;
}

void DataInjection::set_saving_info(bool mode, std::string path) {
    saving_mode = mode;
    saving_path = path;
}

bool DataInjection::get_saving_mode() const {
    return saving_mode;
}

std::string DataInjection::get_saving_path() const {
    return saving_path;
}

#endif //MARSSIMULATOR_DATAINJECTION_H
