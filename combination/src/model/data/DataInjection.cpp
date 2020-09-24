//
// Created by idz on 2019/3/31.
//

#include <cstring>
#include "DataInjection.h"

void DataInjection::_setting_window() {
    short start_i = 100, start_j = 200, ni = 500, nj = 500;
    memcpy(&setting_window[0], &start_i, 2);
    memcpy(&setting_window[2], &start_j, 2);
    memcpy(&setting_window[4], &ni, 2);
    memcpy(&setting_window[6], &nj, 2);
}
