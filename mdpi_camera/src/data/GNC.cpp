//
// Created by vhdsih on 20-1-20.
//

#include "GNC.h"
#include "../utils/utils.h"

void GNC::set_gnc(u_char buffer[]) { // udp version
    if (buffer == nullptr)
        return;

    int start_addr = 0;

    year = bytes_2_int(&buffer[start_addr]); // year 4
    start_addr += 4;
    month = bytes_2_int(&buffer[start_addr]); // month 4
    start_addr += 4;
    day = bytes_2_int(&buffer[start_addr]); // day 4
    start_addr += 4;
    hour = bytes_2_int(&buffer[start_addr]); // hour 4
    start_addr += 4;
    minute = bytes_2_int(&buffer[start_addr]); // minute 4
    start_addr += 4;
    second = bytes_2_int(&buffer[start_addr]); // second
    start_addr += 4;

    seconds_offset_2016 = time_2_seconds(year, month, day, hour, minute, second) - seconds_until_2016;
    memset(time_buffer, 0, 80);
    seconds_2_time((time_t)get_gnc_seconds_from_1970_1_1_8_0_0(), time_buffer);
    printf("%s\n",time_buffer );

    start_addr += 32; // q

    posture[0] = bytes_2_double(&buffer[start_addr]); // pitch 8 --  du
    start_addr += 8;
    posture[1] = bytes_2_double(&buffer[start_addr]); // yaw 8 --  du
    start_addr += 8;
    posture[2] = bytes_2_double(&buffer[start_addr]); // roll 8 --  du
    start_addr += 8;
    posture[3] = bytes_2_double(&buffer[start_addr]); // pitch_speed 8 -- du/s
    start_addr += 8;
    posture[4] = bytes_2_double(&buffer[start_addr]); // yaw_speed 8 -- du/s
    start_addr += 8;
    posture[5] = bytes_2_double(&buffer[start_addr]); // roll_speed 8 -- du/s
    start_addr += 8;

    distance = bytes_2_double(&buffer[start_addr]); // distance 8 -- m
    start_addr += 8;

    sun[0] = bytes_2_double(&buffer[start_addr]); // sun - x 8 // none unit
    start_addr += 8;
    sun[1] = bytes_2_double(&buffer[start_addr]); // sun - y 8
    start_addr += 8;
    sun[2] = bytes_2_double(&buffer[start_addr]); // sun - z 8
    start_addr += 8;

    orbit[0] = bytes_2_double(&buffer[start_addr]); // orbit - 1 - 8
    start_addr += 8;
    orbit[1] = bytes_2_double(&buffer[start_addr]); // orbit - 2 - 8
    start_addr += 8;
    orbit[2] = bytes_2_double(&buffer[start_addr]); // orbit - 3 - 8
    start_addr += 8;
    orbit[3] = bytes_2_double(&buffer[start_addr]); // orbit - 4 - 8
    start_addr += 8;
    orbit[4] = bytes_2_double(&buffer[start_addr]); // orbit - 5 - 8
    start_addr += 8;
    orbit[5] = bytes_2_double(&buffer[start_addr]); // orbit - 6 - 8
    start_addr += 8;

    location[0] = bytes_2_double(&buffer[start_addr]); // location - x - 8
    start_addr += 8;
    location[1] = bytes_2_double(&buffer[start_addr]); // location - y - 8
    start_addr += 8;
    location[2] = bytes_2_double(&buffer[start_addr]); // location - z - 8
    start_addr += 8;
    location[3] = bytes_2_double(&buffer[start_addr]); // location - x_speed - 8
    start_addr += 8;
    location[4] = bytes_2_double(&buffer[start_addr]); // location - y_speed - 8
    start_addr += 8;
    location[5] = bytes_2_double(&buffer[start_addr]); // location - z_speed - 8

    // test
//    sun[0] = location[0];
//    sun[1] = location[1];
//    sun[2] = location[2];

    right = !(fabs(location[0]) < 1e-6 && fabs(location[1]) < 1e-6);
}

void GNC::set_gnc_v2(u_char buffer[], int offset) { // rs422 version
    // len: 74
    // header 0 1
    // ctrl 2 35
    // len 4
    // cmd 6 7
    // time 8 9 10 11 12 13
    memcpy(time_buffer_raw, &buffer[8 - offset], 6);
    seconds_offset_2016 = (bytes_2_uvalue(&buffer[8 - offset], 6) / 10000.0); // 0.1ms -> s
    memset(time_buffer, 0, 80);
    seconds_2_time((time_t)get_gnc_seconds_from_1970_1_1_8_0_0(), time_buffer);
    printf("%s\n", time_buffer);

    // 俯仰 14 15
    posture[0] = bytes_2_short(&buffer[14 - offset]) * 0.01; // du
    // 偏航 16 17
    posture[1] = bytes_2_short(&buffer[16 - offset]) * 0.01; // du
    // 滚动 18 19
    posture[2] = bytes_2_short(&buffer[18 - offset]) * 0.01; // du
    // 俯仰速度 20 21
    posture[3] = bytes_2_short(&buffer[20 - offset]) * 0.0001; // du/s
    // 偏航速度 22 23
    posture[4] = bytes_2_short(&buffer[22 - offset]) * 0.0001; // du/s
    // 滚动速度 24 25
    posture[4] = bytes_2_short(&buffer[24 - offset]) * 0.0001; // du/s

    // 轨道上下行标志 26 27 28 29
    distance = bytes_2_uvalue(&buffer[27 - offset], 3) * 100; // 0.1 km -> m

    // 轨道六根数 a 30 31 32 33
    orbit[0] = bytes_2_float(&buffer[30 - offset]) * 100; // 0.1 km -> m
    // 偏心率    34 35
    orbit[1] = bytes_2_ushort(&buffer[34 - offset]) * 0.0001;
    // 轨道倾角 36 37
    orbit[2] = bytes_2_ushort(&buffer[36 - offset]) * 0.001;
    // 升交点赤经 38 39
    orbit[3] = bytes_2_ushort(&buffer[38 - offset]) * 0.001;
    // 近地点幅角 40 41
    orbit[4] = bytes_2_ushort(&buffer[40 - offset]) * 0.001;
    // 真近角 42 43
    orbit[5] = bytes_2_ushort(&buffer[42 - offset]) * 0.001;

    // 火星惯性系 X 44 45 46 47
    location[0] = bytes_2_float(&buffer[44 - offset]) * 100; // m
    // 火星惯性系 Y 48 49 50 51
    location[1] = bytes_2_float(&buffer[48 - offset]) * 100; // m
    // 火星惯性系 Z 52 53 54 55
    location[2] = bytes_2_float(&buffer[52 - offset]) * 100; // m
    // VX 56 57 58 59
    location[3] = bytes_2_float(&buffer[56 - offset]) * 0.1; // m/s
    // VY 60 61 62 63
    location[4] = bytes_2_float(&buffer[60 - offset]) * 0.1; // m/s
    // VZ 64 65 66 67
    location[5] = bytes_2_float(&buffer[64 - offset]) * 0.1; // m/s

//    double orbInfo[6];
//    printf("%lf %lf %lf %lf %lf %lf\n", orbit[0], orbit[1], orbit[2], orbit[3], orbit[4], orbit[5]);
//    orbit[2] = degrees2radians(orbit[2]);
//    orbit[3] = degrees2radians(orbit[3]);
//    orbit[4] = degrees2radians(orbit[4]);
//    orbit[5] = degrees2radians(orbit[5]);
//    calOrbG(orbInfo, orbit);

    printf("XYZ: %lf %lf %lf\n", location[0], location[1], location[2]);
//    printf("XYZ-1: %lf %lf %lf\n", orbInfo[0], orbInfo[1], orbInfo[2]);
    printf("distance: %lf\n", distance);

    // 备用 68 69 70 71
    // checksum 72 73

    sun[0] = location[0];
    sun[1] = location[1];
    sun[2] = location[2];
    right = true;

    // !!!!!!!!!!!!!!!!!!!!!!!!
    cheat();

}

void GNC::cheat() {
    // re calculate x y z by distance
    double x = get_x();
    double y = get_y();
    double z = get_z();
    auto len_now = sqrt(x * x + y * y + z * z);
    if (len_now <= 3398000 + 200000) {
        auto k = distance / len_now;
        location[0] = k * x;
        location[1] = k * y;
        location[2] = k * z;
        printf("--------------XYZ: %lf %lf %lf\n", get_x(), get_y(), get_z());
    }
//    if (fabs(len_now - distance) > 1000) {
//        // cheat?
//    }
}
