//
// Created by z on 19-3-29.
//

#include "GNC.h"
#include "../tools/cray.h"
#include "../tools/utils.h"

void GNC::set_gnc(u_char buffer[]) {
    if (buffer == nullptr)
        return;

    // x
    loc[0] = bytes_2_double(&buffer[0]);
    // y
    loc[1] = bytes_2_double(&buffer[8]);
    // z
    loc[2] = bytes_2_double(&buffer[16]);
    // pitch
    posture[0] = bytes_2_double(&buffer[24]);
    // yaw
    posture[1] = bytes_2_double(&buffer[32]);
    // roll
    posture[2] = bytes_2_double(&buffer[40]);

    // sun x
    sun[0] = bytes_2_double(&buffer[48]);
    // sun y
    sun[1] = bytes_2_double(&buffer[56]);
    // sun z
    sun[2] = bytes_2_double(&buffer[64]);

    seconds_from_2000 = bytes_2_uvalue(&buffer[72], 8);
}



double GNC::get_angle() const {
    auto second_now = get_second();
    return second_now * SPEED;
}
