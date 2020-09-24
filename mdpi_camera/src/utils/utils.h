//
// Created by idz on 2019/4/9.
//

#ifndef MARSSIMULATOR_UTILS_H
#define MARSSIMULATOR_UTILS_H

#include <chrono>
#include <cmath>
#include <ctime>
#include <cstring>
#include "../value/const_values.h"

unsigned int xy2value(int x, int y);

std::chrono::steady_clock::time_point timestamp();

double timediff(const std::chrono::steady_clock::time_point &b, const std::chrono::steady_clock::time_point &e);

int mtxMtp(double *pMtx0, double *pMtx1, int row1, int col1, double *pMtx2, int row2, int col2);

void mtxAdd(double *pMtx0, double *pMtx1, double *pMtx2, int row, int col);

double norm(double *pMtx0, int cnt);

void calOrbG(double orbInfo[6], double kplInfo[6]);

double degrees2radians(double &degree);

void seconds_2_time(time_t timestamp, char buffer[]);

time_t time_2_seconds(int year, int month, int day, int hour, int minute, int second);

unsigned int bytes_2_uint(const unsigned char buffer[]);

unsigned short bytes_2_ushort(const unsigned char buffer[]);

unsigned long long int bytes_2_uvalue(const unsigned char buffer[], int nbytes);

int bytes_2_int(const unsigned char buffer[]);

short bytes_2_short(const unsigned char buffer[]);

float bytes_2_float(const unsigned char buffer[]);

double bytes_2_double(const unsigned char buffer[]);

void short_2_bytes(const short value, unsigned char buffer[]);

void double_2_bytes(double x, unsigned char buffer[]);

void value_2_bytes(long long x, unsigned char buffer[]);

#endif //MARSSIMULATOR_UTILS_H
