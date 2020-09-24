//
// Created by z on 9/23/18.
//

#include "Transform.h"
#include "../net/Client.h"
#include "../value/values.h"


Transform::Transform() {

    GDALAllRegister();

    // coordinate
    if (PROJ.importFromWkt(WKT) == OGRERR_CORRUPT_DATA)
        exit(0);
    GEO = PROJ.CloneGeogCS();
    GEO2PROJ = OGRCreateCoordinateTransformation(GEO, &PROJ);
    PROJ2GEO = OGRCreateCoordinateTransformation(&PROJ, GEO);
    // size
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


bool Transform::GetPixelValueByXYZ(float x, float y, float z, unsigned char *rgb) {
    auto &mars_pixels = get_mars_pixels_data();
    if (mars_pixels == nullptr) return false;
    double offset_x, offset_y, lon, lat, _x, _y;
    SpaceLocation2Geo(x, y, z, lon, lat);
    auto status = GeoLocation2Proj(lon, lat, _x, _y);
    if (!status) {
        return false;
    }
    Projcection2Pixel(_x, _y, offset_x, offset_y);

    // x1 y1; x1 y2; x2 y1; x2 y2;
    int x1 = int(offset_x) % geoTiffPixelWidth, x2 = (x1 + 1) % geoTiffPixelWidth;
    int y1 = int(offset_y) % geoTiffPixelHeight, y2 = (y1 + 1) % geoTiffPixelHeight;

    if (x1 == 0 || x2 == 0) {
        auto p = mars_pixels[xy2value(x1, y1)];
        rgb[0] = p.r;
        rgb[1] = p.g;
        rgb[2] = p.b;
        return true;
    }


    auto rgb11 = mars_pixels[xy2value(x1, y1)];
    auto rgb12 = mars_pixels[xy2value(x2, y1)];
    auto rgb21 = mars_pixels[xy2value(x1, y2)];
    auto rgb22 = mars_pixels[xy2value(x2, y2)];

    auto m1 = (x2 - offset_x) * (y2 - offset_y);
    auto m2 = (offset_x - x1) * (y2 - offset_y);
    auto m3 = (x2 - offset_x) * (offset_y - y1);
    auto m4 = (offset_x - x1) * (offset_y - y1);

    rgb[0] = m1*rgb11.r + m2*rgb12.r + m3*rgb21.r + m4*rgb22.r;
    rgb[1] = m1*rgb11.g + m2*rgb12.g + m3*rgb21.g + m4*rgb22.g;
    rgb[2] = m1*rgb11.b + m2*rgb12.b + m3*rgb21.b + m4*rgb22.b;
    return true;
}

