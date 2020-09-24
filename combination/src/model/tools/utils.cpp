//
// Created by idz on 2019/4/9.
//

#include "utils.h"
#include <ctime>
#include <cstring>

float get_scale(u_char image_mode) {
    float scale = 0;
    switch (image_mode) {
        case 0x00:
            // send full photo
            scale = 1;
            break;
        case 0x11:
            scale = 0.5;
            break;
        case 0x22:
            scale = 0.25;
            break;
        case 0x33:
            scale = 0.125;
            break;
        case 0x44:
            // open window - keep origin photo
            scale = 1;
            break;
        default:
            break;
    }
    return scale;
}

double degrees2radians(double &degree) {
    return degree / 180.0 * PI;
}

double bytes_2_double(const unsigned char buffer[]) { // high bytes first
    double x = 0;
    for (int i = 0; i < 8; ++i) {
        ((unsigned char*)&x)[i] = buffer[8 - i - 1];
    }
    return x;
}

unsigned long long int bytes_2_uvalue(const unsigned char buffer[], int nbytes) {
    unsigned long long int value = 0;
    for (int i = 0; i < nbytes; ++i) {
        value <<= 8u;
        value += buffer[i];
    }
    return value;
}

time_t time_2_seconds(int year, int month, int day, int hour, int minute, int second) {
    tm t;
    memset(&t, 0, sizeof(t));
    t.tm_year = (year - 1900);
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;
    return mktime(&t);
}

void seconds_2_time(time_t timestamp, char buffer[]) {
    tm *info = localtime(&timestamp);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);
}
