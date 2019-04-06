//
// Created by z on 19-3-29.
//

#include "GNC.h"

void GNC::get_gnc(unsigned char buffer[], int max_len) {
    memset(buffer, 0, max_len);
    int v = 2014;
    memcpy(&buffer[0], &v, sizeof(int));
    v = 12;
    memcpy(&buffer[4], &v, sizeof(int));
    v = 24;
    memcpy(&buffer[8], &v, sizeof(int));
    v = 12;
    memcpy(&buffer[12], &v, sizeof(int));
    v = 13;
    memcpy(&buffer[16], &v, sizeof(int));
    v = 59;
    memcpy(&buffer[20], &v, sizeof(int));
    sun[0] = 0;
    sun[1] = 0;
    sun[2] = -1;
    loc[0] = 0.1;
    loc[1] = 0.1;
    loc[0] = 2 * RADIUS;
    loc[0] = 0;
    loc[0] = 0;
    loc[0] = -1;
    memcpy(&buffer[112], sun, sizeof(sun));
    memcpy(&buffer[184], loc, sizeof(loc));
}
