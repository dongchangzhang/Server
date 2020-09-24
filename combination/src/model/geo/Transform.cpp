//
// Created by z on 9/23/18.
//

#include "Transform.h"
#include "../../conf/constants.h"


Transform::Transform() {

    GDALAllRegister();

    // coordinate
    if (PROJ.importFromWkt(WKT) == OGRERR_CORRUPT_DATA)
        exit(0);
    GEO = PROJ.CloneGeogCS();
    GEO2PROJ = OGRCreateCoordinateTransformation(GEO, &PROJ);
    PROJ2GEO = OGRCreateCoordinateTransformation(&PROJ, GEO);
    // size
    geoTiffPixelWidth = CANVAS_W;
    geoTiffPixelHeight = CANVAS_H;
    SinglePixelWidth = ADFGEOTRANSFORM_1;
}

Transform::~Transform() {
    if (GEO2PROJ)
        CPLFree(GEO2PROJ);
    if (PROJ2GEO)
        CPLFree(PROJ2GEO);
    if (GEO)
        CPLFree(GEO);
}

bool Transform::SpaceLocation2Geo(double x, double y, double z, double &lon, double &lat) {
    lat = asin(z / getRadius()) *180 / M_PI;
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

bool Transform::GetPixelLocByXYZ(float x, float y, float z, double &pixel_x, double &pixel_y) {
    double lon, lat;
    SpaceLocation2Geo(x, y, z, lon, lat);

    double  _x, _y;
    auto status = GeoLocation2Proj(lon, lat, _x, _y);
    if (!status) return false;

    Projcection2Pixel(_x, _y, pixel_x, pixel_y);
    if (pixel_x < 0) pixel_x = 0;
    if (pixel_y < 0) pixel_y = 0;
    if (pixel_x >= geoTiffPixelWidth) pixel_x = int(pixel_x) % geoTiffPixelWidth;
    if (pixel_y >= geoTiffPixelHeight) pixel_y = int(pixel_y) % geoTiffPixelHeight;
    return true;
}


