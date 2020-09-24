//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_GNC_H
#define MDPI_CAMERA_GNC_H



#include <cmath>
#include <ctime>
#include <cstring>
#include <zconf.h>

#include <iostream>
#include "../utils/gvector3.h"
#include "../utils/utils.h"

class GNC {
public:
    GNC() {
        init();
    }

    explicit GNC(u_char buffer[]) {
        init();
        set_gnc(buffer);
    }

    inline void init() {
        seconds_until_2016 = time_2_seconds(2016, 1, 1, 0, 0, 0);
        seconds_until_2000 = time_2_seconds(2000, 1, 1, 0, 0, 0);
        seconds_offset_2016 = 0;
    }

    inline int copy_gnc(char buffer[]) const {
        // x
        double_2_bytes(location[0], reinterpret_cast<unsigned char *>(&buffer[0]));
        // y
        double_2_bytes(location[1], reinterpret_cast<unsigned char *>(&buffer[8]));
        // z
        double_2_bytes(location[2], reinterpret_cast<unsigned char *>(&buffer[16]));
        // pitch
        double_2_bytes(posture[0], reinterpret_cast<unsigned char *>(&buffer[24]));
        // yaw
        double_2_bytes(posture[1], reinterpret_cast<unsigned char *>(&buffer[32]));
        // roll
        double_2_bytes(posture[2], reinterpret_cast<unsigned char *>(&buffer[40]));

        // sun x
        double_2_bytes(sun[0], reinterpret_cast<unsigned char *>(&buffer[48]));
        // sun y
        double_2_bytes(sun[1], reinterpret_cast<unsigned char *>(&buffer[56]));
        // sun z
        double_2_bytes(sun[2], reinterpret_cast<unsigned char *>(&buffer[64]));

        // seconds from 2000
        time_t seconds_from_2020 = seconds_until_2016 + seconds_offset_2016 - seconds_until_2000;
        value_2_bytes(seconds_from_2020, reinterpret_cast<unsigned char *>(&buffer[72]));

        return 80;
    }

    void cheat();

    void set_gnc(u_char buffer[]);
    void set_gnc_v2(u_char buffer[], int offset);

    inline double get_angle() const {
        return (seconds_until_2016 + seconds_offset_2016 - seconds_until_2000) * SPEED;
    }

    inline char* get_time_raw() const {
        return (char*)time_buffer_raw;
    }

    inline char* get_time() const {
        return (char*)time_buffer;
    }

    inline double get_pitch() const {
        return posture[0];
    }

    inline double get_yaw() const {
        return posture[1];
    }

    inline double get_roll() const {
        return posture[2];
    }

    inline double get_path_a() const {
        return orbit[0];
    }
    inline double get_path_b() const {
        return orbit[1];
    }
    inline double get_path_c() const {
        return orbit[2];
    }
    inline double get_path_d() const {
        return orbit[3];
    }
    inline double get_path_e() const {
        return orbit[4];
    }
    inline double get_path_f() const {
        return orbit[5];
    }

    inline double get_x() const {
        return location[0];
    }

    inline double get_y() const {
        return location[1];
    }

    inline double get_z() const {
        return location[2];
    }
    inline double get_x_speed() const {
        return location[3];
    }

    inline double get_y_speed() const {
        return location[4];
    }

    inline double get_z_speed() const {
        return location[5];
    }

    inline double get_x_sun() const {
        return sun[0];
    }
    inline double get_y_sun() const {
        return sun[1];
    }
    inline double get_z_sun() const {
        return sun[2];
    }

    inline bool is_right() const {
        return right;
    }
    long long get_gnc_seconds_from_1970_1_1_8_0_0() const {
        return seconds_until_2016 + seconds_offset_2016;
    }

private:

    double sun[3] = {0};    // 探测器轨道坐标系 下太阳方位矢量
    double orbit[6] = {0};    // 轨道六根数（半长 轴、偏心率、轨道 倾角、升交点赤经、 近地点幅角、真近 点角）
    double posture[6] = {0};    // 探测器本体系相对 自身轨道系的姿态 （俯仰、偏航、滚动; 速度）
    double location[6] = {0}, distance = 0; // X、Y、Z、 Vx、Vy、Vz

    char time_buffer_raw[6], time_buffer[80];

    unsigned int year = 2016, month = 1, day = 1, hour = 0, minute = 0, second = 0;

    bool right = false;
    time_t seconds_until_2016 = 0, seconds_offset_2016 = 0, seconds_until_2000;

};

#endif //MDPI_CAMERA_GNC_H
