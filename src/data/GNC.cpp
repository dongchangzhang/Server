//
// Created by z on 19-3-29.
//

#include <cstdlib>
#include <cmath>
#include <iostream>
#include "GNC.h"
#include "../utils/utils.h"

GNC::GNC() {
    nsec_now = time(nullptr);
    loc[0] = RADIUS;
    loc[1] = RADIUS;
    loc[2] = RADIUS;
}
GNC::GNC(bool _test_sun) : test_sun(_test_sun) {
    nsec_now = time(nullptr);
    loc[0] = RADIUS;
    loc[1] = RADIUS;
    loc[2] = RADIUS;

}


void GNC::get_gnc(unsigned char buffer[], int &gnc_len, int max_len) {
    struct tm *tm_now;
    tm_now = localtime(&nsec_now);
    year = tm_now->tm_year;
    month = tm_now->tm_mon;
    day = tm_now->tm_mday;
    hour = tm_now->tm_hour;
    minute = tm_now->tm_min;
    second = tm_now->tm_sec;

    update_gnc();


    memset(buffer, 0, max_len);
    int start_addr = 0;

    memcpy(&buffer[start_addr], &year, sizeof(year));
    start_addr += sizeof(year);

    memcpy(&buffer[start_addr], &month, sizeof(month));
    start_addr += sizeof(month);

    memcpy(&buffer[start_addr], &day, sizeof(day));
    start_addr += sizeof(day);

    memcpy(&buffer[start_addr], &hour, sizeof(hour));
    start_addr += sizeof(hour);

    memcpy(&buffer[start_addr], &minute, sizeof(minute));
    start_addr += sizeof(minute);

    memcpy(&buffer[start_addr], &second, sizeof(second));
    start_addr += sizeof(second);

    memcpy(&buffer[start_addr], &quaternion, sizeof(quaternion));
    start_addr += sizeof(quaternion);


    double tmp[6];    // 探测器本体系相对 自身轨道系的姿态 （俯仰、偏航、滚 动姿态角、角速度）
    memcpy(tmp, posture, sizeof(posture));
    srand((unsigned)time(0));

    tmp[0] += e_pitch * (rand() % 100) / 100;
    tmp[1] += e_yaw * (rand() % 100) / 100;
    tmp[2] += e_roll * (rand() % 100) / 100;

    memcpy(&buffer[start_addr], &tmp, sizeof(tmp));
    start_addr += sizeof(posture);

    memcpy(&buffer[start_addr], &dist, sizeof(dist));
    start_addr += sizeof(dist);


    memcpy(&buffer[start_addr], &sun, sizeof(sun));
    start_addr += sizeof(sun);

    memcpy(&buffer[start_addr], &orbit, sizeof(orbit));
    start_addr += sizeof(orbit);

    memcpy(&buffer[start_addr], &loc, sizeof(loc));
    start_addr += sizeof(loc);

    gnc_len = start_addr;

}


void GNC::update_gnc() {
    const int N = 360, D = 3656;
    double d = sqrt(loc[1] / 1000 * loc[0]/1000 + loc[1] / 1000 * loc[1]/1000 + loc[2]/1000 * loc[2]/1000) / D;
    nsec_now += (86400 / N);
    float dangle = 4 * 2 * M_PI / N;
    angle += (dangle / d);
    loc[0] =  316500 + 513000 * cos(angle);
    loc[1] =  -8004000 * sin(angle);
    loc[2] =  5843900 + 10473000 * cos(angle);
    if (!test_sun) {
        sun[0] = loc[0];
        sun[1] = loc[1];
        sun[2] = loc[2];
    }
}

void GNC::update_gnc_orb() {
    double xyz_v[6] = {};
    calOrbG(xyz_v, orbit);
    loc[0] = xyz_v[0];
    loc[1] = xyz_v[1];
    loc[2] = xyz_v[2];
}

/**
* [FunctionName] [矩阵乘法]
* [Author] [吴佳奇]
* [E-mail] [wjqjump@163.com]
* [Data]   [2018年11月14日]

pMtx0 = pMtx1 × pMtx2

* [output] [pMtx0] [输出矩阵]
* [input]  [pMtx1] [输入矩阵1]
* [input]  [row1] [行数1]
* [input]  [col1] [列数1]
* [input]  [pMtx2] [输入矩阵2]
* [input]  [row2] [行数2]
* [input]  [col2] [列数2]
**/
int mtxMtp(double *pMtx0, double *pMtx1, int row1, int col1, double *pMtx2, int row2, int col2)
{
    int i, j, k;

    if ((row1 < 2) && (col1 < 2))
    {
        for (i = 0; i < row2; i++)
            for (j = 0; j < col2; j++)
                *(pMtx0 + i * col2 + j) = (*(pMtx1)) * (*(pMtx2 + i * col2 + j));
        return 1;
    }
    if ((row2 < 2) && (col2 < 2))
    {
        for (i = 0; i < row1; i++)
            for (j = 0; j < col1; j++)
            {
                *(pMtx0 + i * col1 + j) = (*(pMtx1 + i * col1 + j)) *(*(pMtx2));
            }
        return 1;
    }
    if (fabs(col1 - row2) < 1)
    {
        for (i = 0; i < row1; i++)
            for (j = 0; j < col2; j++)
            {
                *(pMtx0 + i * col2 + j) = 0;
                for (k = 0; k < row2; k++)
                    *(pMtx0 + i * col2 + j) += (*(pMtx1 + i * col1 + k)) * (*(pMtx2 + k * col2 + j));
            }
        return 1;
    }

    return 0;
}


/**
* [FunctionName] [矩阵加法]
* [Author] [吴佳奇]
* [E-mail] [wjqjump@163.com]
* [Data]   [2018年11月14日]

pMtx0 = pMtx1 + pMtx2

* [output] [pMtx0] [输出矩阵]
* [input]  [pMtx1] [输入矩阵1]
* [input]  [pMtx2] [输入矩阵2]
* [input]  [row] [行数]
* [input]  [col] [列数]
**/
void mtxAdd(double *pMtx0, double *pMtx1, double *pMtx2, int row, int col)
{
    int i, j;

    for (i = 0; i < row; i++)
        for (j = 0; j < col; j++)
        {
            *(pMtx0 + i * col + j) = (*(pMtx1 + i * col + j)) + (*(pMtx2 + i * col + j));

        }
}

/**
* [FunctionName] [向量求模]
* [Author] [吴佳奇]
* [E-mail] [wjqjump@163.com]
* [Data]   [2018年11月14日]

返回值 = norm(pMtx0)

* [output] [返回值] [向量的模]
* [input]  [pMtx0] [输入向量]
* [input]  [cnt] [向量维数]
**/
double norm(double *pMtx0, int cnt)
{
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
* [Author] [吴佳奇]
* [E-mail] [wjqjump@163.com]
* [Data]   [2018年11月14日]

* [output] [orbInfo] [位置与速度]
* [input]  [kplInfo] [轨道六根数]
**/

// kplInfo [a 半长轴, e 偏心率, i轨道倾角, 升交点赤经, w近地幅角 , M平近点角]
void GNC::calOrbG(double orbInfo[6], double kplInfo[6])
{
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
    for (i = 0; i < 10; i++)
    {
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

