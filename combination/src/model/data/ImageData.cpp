//
// Created by vhdsih on 2020/1/21.
//

#include <cstring>
#include "ImageData.h"

ImageData::ImageData(int _width, int _height, int _nchannel) :
    width(_width), height(_height), nchannel(_nchannel), mask_r(0), mask_g(1), mask_b(2) {
    data = new unsigned char[width * height * nchannel]();
}

ImageData::ImageData(int _width, int _height, int _nchannel, unsigned char *_data) :
    width(_width), height(_height), nchannel(_nchannel), mask_r(0), mask_g(1), mask_b(2) {
    data = new unsigned char[width * height * nchannel]();
    memcpy(data, _data, sizeof(char) * height * width * nchannel);
}

ImageData::~ImageData() {
    delete[] data;
}

//void ImageData::from_Mat(cv::Mat mat) {
//    // check
//    width = mat.cols;
//    height = mat.rows;
//    nchannel = mat.channels();
//    delete[] data;
//    data = new unsigned char[width * height * nchannel]();
//    memcpy(data, mat.data, sizeof(char) * width * height * nchannel);
//    // b g r -> r g b
//    for (int i = 0; i < width * height; ++i) {
//        std::swap(data[3 * i + 0], data[3 * i + 2]);
//    }
//}

wxImage ImageData::to_wxImage() {
    auto image = wxImage(width, height, new unsigned char[width * height * 3]());
    memcpy(image.GetData(), data, width * height * 3);
    return image;
}

ImageData::ImageData(const ImageData &other) {
    mask_r = other.mask_r;
    mask_g = other.mask_g;
    mask_b = other.mask_b;

    width = other.width;
    height = other.height;
    nchannel = other.nchannel;

    data = new unsigned char[width * height * nchannel]();
    memcpy(data, other.data, sizeof(char) * height * width * nchannel);
}

ImageData &ImageData::operator=(const ImageData &other) {
    delete[] data;

    mask_r = other.mask_r;
    mask_g = other.mask_g;
    mask_b = other.mask_b;

    width = other.width;
    height = other.height;
    nchannel = other.nchannel;

    data = new unsigned char[width * height * nchannel]();
    memcpy(data, other.data, sizeof(char) * height * width * nchannel);
    return *this;
}

