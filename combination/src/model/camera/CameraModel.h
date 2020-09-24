//
// Created by z on 9/24/18.
//

#ifndef MARSSIMULATOR_MARSMODEL_H
#define MARSSIMULATOR_MARSMODEL_H



#include "perspectiveCamera.h"

#include "../tools/cray.h"
#include "../tools/utils.h"
#include "../tools/csphere.h"
#include "../geo/Transform.h"

#include "../data/GNC.h"
#include "../data/gvector3.h"
#include "../data/ImageData.h"
#include "../data/DataInjection.h"

#include "../../conf/constants.h"

#include <cmath>
#include <string>
#include <iostream>

class CameraModel {
public:
    explicit CameraModel(std::string _file="./transform1.tif");

    ~CameraModel() {
        delete transform;
    }

    void set_camera_params(GVector3 location);

    // camera coordinate
    void init_camera_coordinate(GVector3 cam_loc);

    // run camera
    void run_camera(const GNC &gnc, const DataInjection &camera_params, wxImage &image, ImageData &canvas);


    void draw_image(wxImage &image, ImageData &canvas);


    void rotate_point(GVector3 &point, double w);


    friend void draw_image_helper(CameraModel *cameraModel, wxImage &image, ImageData &canvas, int from, int to);

    // getters

    inline double get_cam_fov_hor() { return camera_fov_hor; }

    inline double get_cam_fov_ver() { return camera_fov_ver; }

private:
    // Mars
    Transform *transform;
    GVector3 mars_origin = GVector3(0, 0, 0);
    CSphere *sphere_model;

    // Camera
    perspectiveCamera camera;
    double camera_fov_hor, camera_fov_ver, camera_f_value;

    GVector3 x_axis, y_axis, z_axis;
};

#endif //MARSSIMULATOR_MARSMODEL_H
