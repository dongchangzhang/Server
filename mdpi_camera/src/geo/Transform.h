//
// Created by z on 9/23/18.
//

#ifndef MARSSIMULATOR_GEOTIFFREADER_H
#define MARSSIMULATOR_GEOTIFFREADER_H

#include <string>
#include <cmath>
#include <gdal_priv.h>
#include <opencv2/opencv.hpp>
#include <unordered_map>
#include "../value/const_values.h"


class Transform {
public:

    Transform();

    ~Transform();

    bool GetPixelValueByXYZ(float x, float y, float z, unsigned char rgb[3]);

    inline double getRadius() {
        return (geoTiffPixelWidth * SinglePixelWidth) / (2 * M_PI);
    }

private:

    inline bool SpaceLocation2Geo(double x, double y, double z, double &lon, double &lat);

    inline bool GeoLocation2Proj(double lon, double lat, double &x, double &y);

    inline bool Projcection2Pixel(double lon, double lat, double &offset_x, double &offset_y);

    inline uint xy2value(int &x, int &y);


private:
    int geoTiffPixelWidth = GeoTiffPixelWidth;
    int geoTiffPixelHeight = GeoTiffPixelHeight;
    double SinglePixelWidth = 1157.708935999999994;

    // coordinate system
    OGRSpatialReference *GEO, PROJ;
    // tool for coordinate transformation
    OGRCoordinateTransformation *GEO2PROJ = nullptr, *PROJ2GEO = nullptr;
};

bool Transform::SpaceLocation2Geo(double x, double y, double z, double &lon, double &lat) {
    double tmp = z / getRadius();
    if (tmp > 1.0) {
        tmp = 1.0;
    } else if (tmp < -1.0) {
        tmp = -1.0;
    }

    lat = asin(tmp) *180 / M_PI;
    lon = atan2(y, x) * 180 / M_PI;
    return true;
}

bool Transform::GeoLocation2Proj(double lon, double lat, double &x, double &y) {
    if( GEO2PROJ == NULL || !GEO2PROJ->Transform(1, &lon, &lat))
        return false;
    x = lon;
    y = lat;
    return true;
}

bool Transform::Projcection2Pixel(double lon, double lat, double &offset_x, double &offset_y) {
    double n3 = ADFGEOTRANSFORM_2 * (lat - ADFGEOTRANSFORM_3) + ADFGEOTRANSFORM_5 * (ADFGEOTRANSFORM_0 - lon);
    double n4 = ADFGEOTRANSFORM_1 * (lat - ADFGEOTRANSFORM_3) + ADFGEOTRANSFORM_4 * (ADFGEOTRANSFORM_0- lon);
    double n5 = ADFGEOTRANSFORM_2 * ADFGEOTRANSFORM_4 - ADFGEOTRANSFORM_1 * ADFGEOTRANSFORM_5;
    offset_x = n3 / n5;
    offset_y = n4 / -n5;
    return true;
}

uint Transform::xy2value(int &x, int &y) {
    return y * geoTiffPixelWidth + x;
}


#endif //MARSSIMULATOR_GEOTIFFREADER_H
