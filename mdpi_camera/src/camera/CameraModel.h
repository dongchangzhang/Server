//
// Created by vhdsih on 9/24/18.
//

#ifndef MARSSIMULATOR_MARSMODEL_H
#define MARSSIMULATOR_MARSMODEL_H



#include "perspectiveCamera.h"

#include "../utils/cray.h"
#include "../utils/color.h"
#include "../utils/utils.h"
#include "../utils/csphere.h"
#include "../utils/gvector3.h"
#include "../utils/directlight.h"
#include "../data/GNC.h"
#include "../data/ImageData.h"
#include "../data/DataInjection.h"
#include "../geo/Transform.h"
#include "../value/const_values.h"

#include <cmath>
#include <string>
#include <iostream>


class CameraModel {
public:
    CameraModel();

    ~CameraModel() {
        delete transformer;
    }

    // run camera
    void run_camera(const GNC &gnc, const DataInjection &camera_params, ImageData &image_data);

    void taking_photo_by_direct_light(ImageData &image_data);

    void rotate_point(GVector3 &point, double w);

    // camera coordinate
    void init_camera_coordinate(GVector3 cam_loc, double w);

    // camera
    void set_camera_params(GVector3 &location);

    friend void taking_photo_by_direct_light_helper(CameraModel *cameraModel, ImageData &image_data, int from, int to);

private:
    //
    const int MAX = 5000000;
    short sample_time = 0;

    short expo_time = 0;
    // Mars
    Transform *transformer;
    GVector3 mars_origin = GVector3(0, 0, 0);
    CSphere *sphere_model;
    DirectLight light;
    // Camera
    perspectiveCamera camera;
    GVector3 x_axis, y_axis, z_axis;
};

#endif //MARSSIMULATOR_MARSMODEL_H
