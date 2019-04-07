//
// Created by z on 19-3-29.
//

#include <cstdlib>
#include "GNC.h"

void GNC::get_gnc(unsigned char buffer[], int &gnc_len, int max_len) {
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
    srand(time(0));
    sun[0] = i++;
    sun[1] = 0;
    sun[2] = 0;
    loc[0] = 7 * RADIUS;
    loc[1] = 0;
    loc[2] = 0;
    loc[3] = -1;
    loc[4] = 0;
    loc[5] = 0;
    memcpy(&buffer[112], sun, sizeof(sun));
    memcpy(&buffer[184], loc, sizeof(loc));
    gnc_len = 232;
}
