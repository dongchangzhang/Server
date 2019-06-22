//
// Created by z on 19-3-29.
//

#include <cstdlib>
#include <cmath>
#include <iostream>
#include "GNC.h"
#include "../utils/utils.h"

GNC::GNC() {
    nsec_now = time(nullptr);
    loc[0] = RADIUS;
    loc[1] = RADIUS;
    loc[2] = RADIUS;
}
GNC::GNC(bool _test_sun) : test_sun(_test_sun) {

}


void GNC::get_gnc(unsigned char buffer[], int &gnc_len, int max_len) {
    struct tm *tm_now;
    tm_now = localtime(&nsec_now);
    year = tm_now->tm_year;
    month = tm_now->tm_mon;
    day = tm_now->tm_mday;
    hour = tm_now->tm_hour;
    minute = tm_now->tm_min;
    second = tm_now->tm_sec;

    update_gnc();

    memset(buffer, 0, max_len);
    int start_addr = 0;

    memcpy(&buffer[start_addr], &year, sizeof(year));
    start_addr += sizeof(year);

    memcpy(&buffer[start_addr], &month, sizeof(month));
    start_addr += sizeof(month);

    memcpy(&buffer[start_addr], &day, sizeof(day));
    start_addr += sizeof(day);

    memcpy(&buffer[start_addr], &hour, sizeof(hour));
    start_addr += sizeof(hour);

    memcpy(&buffer[start_addr], &minute, sizeof(minute));
    start_addr += sizeof(minute);

    memcpy(&buffer[start_addr], &second, sizeof(second));
    start_addr += sizeof(second);

    memcpy(&buffer[start_addr], &quaternion, sizeof(quaternion));
    start_addr += sizeof(quaternion);

    memcpy(&buffer[start_addr], &posture, sizeof(posture));
    start_addr += sizeof(posture);

    memcpy(&buffer[start_addr], &dist, sizeof(dist));
    start_addr += sizeof(dist);


    memcpy(&buffer[start_addr], &sun, sizeof(sun));
    start_addr += sizeof(sun);

    memcpy(&buffer[start_addr], &orbit, sizeof(orbit));
    start_addr += sizeof(orbit);


    memcpy(&buffer[start_addr], &loc, sizeof(loc));
    start_addr += sizeof(loc);

    gnc_len = start_addr;

}


void GNC::update_gnc() {
    const int N = 360, D = 3656;
    double d = sqrt(loc[1] / 1000 * loc[0]/1000 + loc[1] / 1000 * loc[1]/1000 + loc[2]/1000 * loc[2]/1000) / D;
    nsec_now += (86400 / N);
    float dangle = 4 * 2 * M_PI / N;
    angle += (dangle / d);
    loc[0] =  316500 + 513000 * cos(angle);
    loc[1] =  -8004000 * sin(angle);
    loc[2] =  5843900 + 10473000 * cos(angle);
    if (!test_sun) {
        sun[0] = loc[0];
        sun[1] = loc[1];
        sun[2] = loc[2];
    }
}

