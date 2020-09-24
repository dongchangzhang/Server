//
// Created by vhdsih on 20-1-18.
//

#ifndef MDPI_CAMERA_PIXEL_H
#define MDPI_CAMERA_PIXEL_H

using uchar = unsigned char;

struct Pixel {
    uchar r = 255;
    uchar g = 255;
    uchar b = 255;
    Pixel() = default;
    Pixel(uchar _r, uchar _g, uchar _b) : r(_r), g(_g), b(_b) {}
    Pixel(uchar buffer[3]) : r(buffer[0]), g(buffer[1]), b(buffer[2]) {}
};

#endif //MDPI_CAMERA_PIXEL_H
