//
// Created by idz on 2019/3/31.
//

#include "DataInjection.h"

int DataInjection::get_dij_into_buffer(unsigned char *buffer) {
    int start_addr = 0;
    memcpy(&buffer[start_addr], &work_mode, sizeof(work_mode));
    start_addr += sizeof(work_mode);

    memcpy(&buffer[start_addr], &expo_mode, sizeof(expo_mode));
    start_addr += sizeof(expo_mode);

    memcpy(&buffer[start_addr], &image_mode, sizeof(image_mode));
    start_addr += sizeof(image_mode);

    memcpy(&buffer[start_addr], setting_window, sizeof(setting_window));
    start_addr += sizeof(setting_window);

    memcpy(&buffer[start_addr], manual_expo, sizeof(manual_expo));
    start_addr += sizeof(manual_expo);

    memcpy(&buffer[start_addr], auto_expo, sizeof(auto_expo));
    start_addr += sizeof(manual_expo);
    return start_addr;
}
