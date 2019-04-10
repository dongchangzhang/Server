//
// Created by idz on 2019/4/9.
//

#include "utils.h"

float get_scale(u_char image_mode) {
    float scale = 0;
    switch (image_mode) {
        case 0x00:
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
            scale = 1;
            break;
        default:
            break;
    }
    return scale;
}

void get_height_width(u_char image_mode, int &height, int &width) {
    float scale = get_scale(image_mode);

    width = PHOTO_WIDTH * scale;
    height = PHOTO_HEIGHT * scale;
}
