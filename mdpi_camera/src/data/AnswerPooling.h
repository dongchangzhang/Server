//
// Created by vhdsih on 19-10-14.
//

#ifndef MARSSIMULATOR_ANSWERPOOLING_H
#define MARSSIMULATOR_ANSWERPOOLING_H

#include <cstring>
#include "../value/const_values.h"
#include "DataInjection.h"

using uchar = unsigned  char;
struct AnswerPooling {

    //1 workmod
    uchar buffer[255];
    AnswerPooling();

    int get_pooling(uchar buf[], int max_len, DataInjection &cmds, int m, int n);
};


#endif //MARSSIMULATOR_ANSWERPOOLING_H
