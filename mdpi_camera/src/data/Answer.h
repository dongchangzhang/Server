//
// Created by idz on 2019/3/31.
//

#ifndef MARSSIMULATOR_ANSWER_H
#define MARSSIMULATOR_ANSWER_H

#include "../value/const_values.h"
#include <cstring>

struct Answer {
    unsigned char buf[4];
    Answer(char ans_type, char answer) {
        buf[0] = HEAD1;
        buf[1] = HEAD2;
        buf[2] = ans_type;
        buf[3] = answer;
    }
    inline int get_buf(unsigned char buffer[]) {
        memset(buffer, 0, 255);
        memcpy(buffer, buf, 4);
        return 4;
    }
};

#endif //MARSSIMULATOR_ANSWER_H
