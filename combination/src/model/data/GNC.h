//
// Created by z on 19-3-29.
//

#ifndef MARSSIMULATOR_GNC_H
#define MARSSIMULATOR_GNC_H

// LAN interface : send it to camera

// port 5002

#include <zconf.h>
#include <cstring>
#include <ctime>
#include "../data/gvector3.h"
#include "../../conf/constants.h"


struct GNC {
    double sun[3];    // 探测器轨道坐标系 下太阳方位矢量
    double loc[3]; // 火星惯性系下探测 器位置（X、Y、Z、 Vx、Vy、Vz）
    double posture[3];    // 探测器本体系相对 自身轨道系的姿态 （俯仰、偏航、滚 动姿态角、角速度）
    time_t seconds_from_2000;

    // another two
    double fov_h = DEFAULT_CAMERA_FOV_HOR, fov_v = DEFAULT_CAMERA_FOV_VER, f_value = DEFAULT_CAMERA_F_DISTANCE;

    GNC() = default;

    GNC(u_char buffer[]) {
        set_gnc(buffer);
    }



    void set_gnc(u_char buffer[]);

    time_t get_second() const {
        return seconds_from_2000;
    }

    double get_angle() const;

    inline void get_loc_of_camera(GVector3 & _loc) const {
        _loc.x = loc[0];
        _loc.y = loc[1];
        _loc.z = loc[2];
    }

    inline void get_loc_of_sun(GVector3 &_loc) const {
        _loc.x = sun[0];
        _loc.y = sun[1];
        _loc.z = sun[2];
    }

    inline void get_euler(double &roll, double &pitch, double &yaw) const {
        pitch = posture[0];
        yaw = posture[1];
        roll = posture[2];
    }


    inline double get_fov_h() const { return fov_h; }
    inline double get_fov_v() const { return fov_v; }
    inline double get_f_value() const { return f_value; }
};


#endif //MARSSIMULATOR_GNC_H
