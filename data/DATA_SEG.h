//
// Created by idz on 2019/3/31.
//

#ifndef MARSSIMULATOR_DATA_SEG_H
#define MARSSIMULATOR_DATA_SEG_H

#include <opencv2/opencv.hpp>
class DATA_SEG {
    // 需要仿真： y；   无需仿真： n
    char type;    //  包类型    y
    short len;    // 包长度     y
    char expo_time[6];    // 图像曝光时刻    y
    char image_mode;    // 图像模式    y
    char frame_num;    // 图像帧数    y
    char SDRAM;    // SDRAM    n
    short index;    // 图像数据包号    y
    short num_row;    // 行号    y
    char flag_col;    // 列标志    y
    char tail;    // 尾标志    y

    // image data;
    cv::Mat *p = nullptr; // 图像数据    y
};


#endif //MARSSIMULATOR_DATA_SEG_H
