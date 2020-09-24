//
// Created by z on 9/23/18.
//

#ifndef MARSSIMULATOR_GEOTIFFREADER_H
#define MARSSIMULATOR_GEOTIFFREADER_H

#include <string>
#include <cmath>
#include <gdal_priv.h>
#include <opencv2/opencv.hpp>

class Transform {
public:

    Transform();

    ~Transform();

    bool GetPixelLocByXYZ(float x, float y, float z, double &pixel_x, double &pixel_y);

    inline double getRadius() {
        return (geoTiffPixelWidth * SinglePixelWidth) / (2 * M_PI);
    }

private:

    bool SpaceLocation2Geo(double x, double y, double z, double &lon, double &lat);

    bool GeoLocation2Proj(double lon, double lat, double &x, double &y);

    bool Projcection2Pixel(double x, double y, double &offset_x, double &offset_y);

private:

    int geoTiffPixelWidth = 0;
    int geoTiffPixelHeight = 0;
    double SinglePixelWidth = 0;

    // coordinate system
    OGRSpatialReference *GEO, PROJ;

    // tool for coordinate transformation
    OGRCoordinateTransformation *GEO2PROJ = nullptr, *PROJ2GEO = nullptr;
};


#endif //MARSSIMULATOR_GEOTIFFREADER_H
