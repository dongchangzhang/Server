//
// Created by z on 19-3-29.
//

#include <cstdlib>
#include "GNC.h"

void GNC::get_gnc(unsigned char buffer[], int &gnc_len, int max_len) {
    memset(buffer, 0, max_len);
    int start_addr = 0;

    minute += 1;
    if (minute >= 60) {
        minute = 0;
        hour += 1;
    }
    if (hour == 24) {
        day += 1;
        hour = 0;
    }
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

    quaternion[0] = 1;
    quaternion[1] = 0;
    quaternion[2] = 0;
    quaternion[3] = 0;

    memcpy(&buffer[start_addr], &quaternion, sizeof(quaternion));
    start_addr += sizeof(quaternion);
    posture[0] = 10;
    posture[1] = 10;
    posture[2] = 10;
    memcpy(&buffer[start_addr], &posture, sizeof(posture));
    start_addr += sizeof(posture);

    memcpy(&buffer[start_addr], &dist, sizeof(dist));
    start_addr += sizeof(dist);

    sun[0] = 1;
    sun[1] = 1;
    sun[2] = 1;

    memcpy(&buffer[start_addr], &sun, sizeof(sun));
    start_addr += sizeof(sun);

    memcpy(&buffer[start_addr], &orbit, sizeof(orbit));
    start_addr += sizeof(orbit);

    loc[0] = RADIUS + 260000;
    loc[1] = 0;
    loc[2] = 0;

    memcpy(&buffer[start_addr], &loc, sizeof(loc));
    start_addr += sizeof(loc);

    gnc_len = start_addr;
}

double GNC::get_angle() {
    return 0;
}
