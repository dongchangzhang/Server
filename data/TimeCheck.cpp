//
// Created by idz on 2019/3/31.
//

#include "TimeCheck.h"
#include <iostream>

int TimeCheck::get_time_info(unsigned char *buffer) {
    memset(buffer, 0, BUFFER_SIZE);
    buffer[0] = head1;
    buffer[1] = head2;
    buffer[2] = command_type;
    buffer[3] = valid_byte_num;
    memcpy(&buffer[4], &second, sizeof(int));
    memcpy(&buffer[8], &millisecond, sizeof(short));
    buffer[10] = 0x11;
    buffer[11] = 0x11;
    buffer[12] = '\0';
    return false;
}

bool TimeCheck::update() {
    second = ((int)(second) +64) % 256;
    return false;
}
