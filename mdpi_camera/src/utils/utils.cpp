//
// Created by idz on 2019/4/9.
//

#include "utils.h"


unsigned int xy2value(int x, int y) {
    return y * GeoTiffPixelWidth + x;
}

std::chrono::steady_clock::time_point timestamp() {
    return std::chrono::steady_clock::now();
}

double timediff(const std::chrono::steady_clock::time_point &b, const std::chrono::steady_clock::time_point &e) {
    return std::chrono::duration<double, std::milli>(e - b).count();
}

int mtxMtp(double *pMtx0, double *pMtx1, int row1, int col1, double *pMtx2, int row2, int col2) {
    int i, j, k;

    if ((row1 < 2) && (col1 < 2)) {
        for (i = 0; i < row2; i++)
            for (j = 0; j < col2; j++)
                *(pMtx0 + i * col2 + j) = (*(pMtx1)) * (*(pMtx2 + i * col2 + j));
        return 1;
    }
    if ((row2 < 2) && (col2 < 2)) {
        for (i = 0; i < row1; i++)
            for (j = 0; j < col1; j++) {
                *(pMtx0 + i * col1 + j) = (*(pMtx1 + i * col1 + j)) *(*(pMtx2));
            }
        return 1;
    }
    if (fabs(col1 - row2) < 1) {
        for (i = 0; i < row1; i++)
            for (j = 0; j < col2; j++) {
                *(pMtx0 + i * col2 + j) = 0;
                for (k = 0; k < row2; k++)
                    *(pMtx0 + i * col2 + j) += (*(pMtx1 + i * col1 + k)) * (*(pMtx2 + k * col2 + j));
            }
        return 1;
    }

    return 0;
}


void mtxAdd(double *pMtx0, double *pMtx1, double *pMtx2, int row, int col) {
    int i, j;
    for (i = 0; i < row; i++)
        for (j = 0; j < col; j++) {
            *(pMtx0 + i * col + j) = (*(pMtx1 + i * col + j)) + (*(pMtx2 + i * col + j));

        }
}

double norm(double *pMtx0, int cnt) {
    int i;
    double tmp;

    tmp = 0;
    for (i = 0; i < cnt; i++)
        tmp += (*(pMtx0 + i))*(*(pMtx0 + i));
    tmp = sqrt(tmp);

    return tmp;
}

/**
* [FunctionName] [轨道六根数计算位置和速度]
* [output] [orbInfo] [位置与速度]
* [input]  [kplInfo] [轨道六根数]
**/

// kplInfo [a 半长轴, e 偏心率, i轨道倾角, 升交点赤经, w近地幅角 , M平近点角]
void calOrbG(double orbInfo[6], double kplInfo[6]) {
    int i;
    double GM = 3.986005e14;
    /*t,*//*dt,*//* n,*/
    double xPVec[3], xQVec[3], xE;
    double rVec[3], rxPVec[3], rxQVec[3];
    double dxPVec[3], dxQVec[3], drVec[3];
    double drnorm;
    double tmpa, tmpb, tmpc, tmpd;
    /*xPVec[3]、xQVec[3]——近焦点坐标系中的位置矢量在地心赤道系中的投影p^、q^（单位矢量）*/

    xE = kplInfo[5];
    for (i = 0; i < 10; i++) {
        /*迭代计算偏近点角*/
        xE = kplInfo[5] + kplInfo[1] * sin(xE); /* 偏近点角Ei+1=M+e*sin(Ei)，Ei迭代初值为M*/
    }
    /****  以下见轨道力学p141，(4.44)  ***/
    xPVec[0] = cos(kplInfo[3])*cos(kplInfo[4])
               - sin(kplInfo[3])*sin(kplInfo[4])*cos(kplInfo[2]);
    xPVec[1] = sin(kplInfo[3])*cos(kplInfo[4])
               + cos(kplInfo[3])*sin(kplInfo[4])*cos(kplInfo[2]);
    xPVec[2] = sin(kplInfo[4])*sin(kplInfo[2]);
    /*近焦点坐标系中的位置矢量x在地心赤道系中的三轴投影p^*/
    xQVec[0] = -cos(kplInfo[3])*sin(kplInfo[4])
               - sin(kplInfo[3])*cos(kplInfo[4])*cos(kplInfo[2]);
    xQVec[1] = -sin(kplInfo[3])*sin(kplInfo[4])
               + cos(kplInfo[3])*cos(kplInfo[4])*cos(kplInfo[2]);
    xQVec[2] = cos(kplInfo[4])*sin(kplInfo[2]);
    /*近焦点坐标系中的位置矢量y在地心赤道系中的三轴投影q^*/
    tmpa = kplInfo[0] * (cos(xE) - kplInfo[1]); /*tmpa=rcos(f)=x^=a(cos(E)-e)，f——真近点角，p88*/
    tmpb = kplInfo[0] * sqrt(1 - kplInfo[1] * kplInfo[1])*sin(xE); /*tmpb=rsin(f)=y^=a*sqrt(1-e^2)sin(E),p88*/
    tmpc = sqrt(GM*kplInfo[0])*(-sin(xE)); /*近焦点坐标系中的速度分量dx,p61,88*/
    tmpd = sqrt(GM*kplInfo[0])*sqrt(1 - kplInfo[1] * kplInfo[1])*cos(xE); /*近焦点坐标系中的速度分量dy,p61,88*/

    mtxMtp(rxPVec, xPVec, 3, 1, &tmpa, 1, 1);
    mtxMtp(rxQVec, xQVec, 3, 1, &tmpb, 1, 1);
    mtxAdd(rVec, rxPVec, rxQVec, 3, 1);/***相对于近焦点系的位置矢量r=x^p^+y^q^（轨道力学p60、88）*/

    mtxMtp(dxPVec, xPVec, 3, 1, &tmpc, 1, 1);
    mtxMtp(dxQVec, xQVec, 3, 1, &tmpd, 1, 1);
    mtxAdd(drVec, dxPVec, dxQVec, 3, 1); /*相对于近焦点系的速度矢量dr=dx^p^+dy^q^*/

    drnorm = 1.0 / norm(rVec, 3);
    mtxMtp(drVec, drVec, 3, 1, &drnorm, 1, 1);

    orbInfo[0] = rVec[0];
    orbInfo[1] = rVec[1];
    orbInfo[2] = rVec[2];
    orbInfo[3] = drVec[0];
    orbInfo[4] = drVec[1];
    orbInfo[5] = drVec[2];
}

double degrees2radians(double &degree) {
    return degree / 180.0 * PI;
}
time_t time_2_seconds(int year, int month, int day, int hour, int minute, int second) {
    tm t;
    memset(&t, 0, sizeof(t));
    t.tm_year = (year - 1900);
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;
    return mktime(&t);
}

void seconds_2_time(time_t timestamp, char buffer[]) {
    tm *info = localtime(&timestamp);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);
}

unsigned int bytes_2_uint(const unsigned char buffer[]) { // 4
    unsigned int value = 0;
    for (int i = 0; i < 4; ++i) {
        value <<= 8u;
        value += buffer[i];
    }
    return value;
}

unsigned short bytes_2_ushort(const unsigned char buffer[]) {
    unsigned short value = 0;
    for (int i = 0; i < 2; ++i) {
        value <<= 8u;
        value += buffer[i];
    }
    return value;

}

unsigned long long int bytes_2_uvalue(const unsigned char buffer[], int nbytes) {
    unsigned long long int value = 0;
    for (int i = 0; i < nbytes; ++i) {
        value <<= 8u;
        value += buffer[i];
    }
    return value;
}

int bytes_2_int(const unsigned char buffer[]) { // high bytes first
    int x = 0;
    for (int i = 0; i < 4; ++i) {
        ((unsigned char*)&x)[i] = buffer[4 - i - 1];
    }
    return x;
}

short bytes_2_short(const unsigned char buffer[]) { // high bytes first
    short x = 0;
    for (int i = 0; i < 2; ++i) {
        ((unsigned char*)&x)[i] = buffer[2 - i - 1];
    }
    return x;
}

float bytes_2_float(const unsigned char buffer[]) { // high bytes first
    float x = 0;
    for (int i = 0; i < 4; ++i) {
        ((unsigned char*)&x)[i] = buffer[4 - i - 1];
    }
    return x;
}

double bytes_2_double(const unsigned char buffer[]) { // high bytes first
    double x = 0;
    for (int i = 0; i < 8; ++i) {
        ((unsigned char*)&x)[i] = buffer[8 - i - 1];
    }
    return x;
}

void short_2_bytes(const short value, unsigned char buffer[]) {
    for (int i = 0; i < 2; ++i) {
        buffer[i] = ((unsigned char*)&value)[1 - i];
    }
}

void double_2_bytes(double x, unsigned char buffer[]) {
    for (int i = 0; i < 8; ++i) {
        buffer[i] = ((unsigned char*)&x)[8 - i - 1];
    }
}

void value_2_bytes(long long x, unsigned char buffer[]) {
    for (int i = 0; i < 8; ++i) {
        buffer[i] = ((unsigned char*)&x)[8 - i - 1];
    }
}
