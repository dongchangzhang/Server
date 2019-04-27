//
// Created by z on 19-3-29.
//

#ifndef MARSSIMULATOR_GNC_H
#define MARSSIMULATOR_GNC_H

// LAN interface : send it to camera

// port 5002
#include "../constants.h"

#include <cstring>
#include <ctime>

struct GNC {
    int year = 2019;    // 时间码 北京时 - 年
    int month = 3;    // 时间码 北京时 - 月
    int day=11;    // 时间码 北京时 - 日
    int hour=12;    // 时间码 北京时 - 时
    int minute = 21;    // 时间码 北京时 - 分
    int second=0;    //  时间码 北京时 - 秒
    double quaternion[4];    // 探测器本体系相对于自身轨道系的姿态四元数
    double posture[6];    // 探测器本体系相对 自身轨道系的姿态 （俯仰、偏航、滚 动姿态角、角速度）
    double dist;    // 探测器质心到火心得距离
    double sun[3];    // 探测器轨道坐标系 下太阳方位矢量
    double orbit[6];    // 轨道六根数（半长 轴、偏心率、轨道 倾角、升交点赤经、 近地点幅角、真近 点角）
    double loc[6]; // 火星惯性系下探测 器位置（X、Y、Z、 Vx、Vy、Vz）
    GNC() {};
    void get_gnc(unsigned char buffer[], int &gnc_len, int max_len);
    double get_angle();
    int i = 0;
};


#endif //MARSSIMULATOR_GNC_H
