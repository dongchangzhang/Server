//
// Created by idz on 2019/4/9.
//

#ifndef MARSSIMULATOR_UTILS_H
#define MARSSIMULATOR_UTILS_H

#include <zconf.h>
#include "../../conf/constants.h"

double degrees2radians(double &degree);

double bytes_2_double(const unsigned char buffer[]);

unsigned long long int bytes_2_uvalue(const unsigned char buffer[], int nbytes);

void seconds_2_time(time_t timestamp, char buffer[]);

time_t time_2_seconds(int year, int month, int day, int hour, int minute, int second);

#endif //MARSSIMULATOR_UTILS_H
