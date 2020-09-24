//
// Created by vhdsih on 2020/1/21.
//

#ifndef MDPI_CAMERA_IMAGEDATA_H
#define MDPI_CAMERA_IMAGEDATA_H

#include <wx/wx.h>

class ImageData {
public:
    ImageData() = default;
    ImageData(int _width, int _height, int _nchannel=3);
    ImageData(int _width, int _height, int _nchannel, unsigned char *_data);

    ImageData(const ImageData &other);
    ImageData& operator=(const ImageData &other);

    inline void set_mask(int r, int g, int b) {
        mask_r = r;
        mask_g = g;
        mask_b = b;
    }

    inline bool set_pixel(int ncol, int nrow, unsigned char r, unsigned char g, unsigned char b) {
        if (!data) {
            return false;
        } else {
            data[(nrow * width + ncol) * 3 + mask_r] = r;
            data[(nrow * width + ncol) * 3 + mask_g] = g;
            data[(nrow * width + ncol) * 3 + mask_b] = b;
            return true;
        }
    }

    inline bool set_pixel(int ncol, int nrow, int index, unsigned char c) {
        if (!data) {
            return false;
        } else {
            data[(nrow * width + ncol) * 3 + index] = c;
            return true;
        }
    }

    wxImage to_wxImage();

    ~ImageData();

private:
    int width, height, nchannel;
    int mask_r, mask_g, mask_b;
    unsigned char *data = nullptr;
};


#endif //MDPI_CAMERA_IMAGEDATA_H
