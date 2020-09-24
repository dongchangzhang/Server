//
// Created by z on 9/24/18.
//


#include <sstream>
#include <thread>
#include <cmath>
#include "CameraModel.h"
#include "../value/values.h"

// helper for multithread
void taking_photo_by_direct_light_helper(CameraModel *cameraModel, ImageData &image_data, int from, int to) {
    float scale_x = cameraModel->camera.get_scale_h();
    float scale_y = cameraModel->camera.get_scale_v();

    float dx = scale_x / float(IMAGE_WIDTH_GENERATE);
    float dy = scale_y / float(IMAGE_HEIGHT_GENERATE);

    float sy, sx;
//    float base = cameraModel->expo_time;
//    if (base < 0) base = 10;
//    if (base > 1000) base = 1000;
//    base = (base - 150) / 10;
    unsigned char rgb[3] = {0, 0, 0};
    unsigned char r, g, b;
    for (int y = 0; y < IMAGE_HEIGHT_GENERATE; ++y) {
        sy = scale_y - dy * y;
        for (int x = from; x < to; ++x) {
            sx = dx * x;
            CRay ray(cameraModel->camera.generateRay(sx, sy));
            IntersectResult result = cameraModel->sphere_model->isIntersected(ray);
            if (result.isHit) {
                float p_x = result.position.x;
                float p_y = result.position.y;
                float p_z = result.position.z;
                if (p_x == 0 && p_y == 0 && p_z == 0) {
                    continue;
                }
                cameraModel->transformer->GetPixelValueByXYZ(p_x, p_y, p_z, rgb);
                cv::Vec3b pixel;
                Color color = cameraModel->light.intersect(*cameraModel->sphere_model, result, rgb);
                pixel[0] = static_cast<uchar>(std::min(int(color.b), 255));
                pixel[1] = static_cast<uchar>(std::min(int(color.g), 255));
                pixel[2] = static_cast<uchar>(std::min(int(color.r), 255));
                image_data.set_pixel(x, y, pixel[2], pixel[1], pixel[0]);
            }
        }
    }
}

// implements for CameraModel from here

CameraModel::CameraModel() {
    transformer = new Transform();
    sphere_model = new CSphere(mars_origin, transformer->getRadius());
}

void CameraModel::set_camera_params(GVector3 &location) {
    camera = perspectiveCamera(location, z_axis, y_axis, x_axis, CAMERA_FOV_HORIZON, CAMERA_FOV_VERTICAL, CAMERA_FOCAL_DISTANCE);
}

// the implement for the job
void CameraModel::run_camera(const GNC &gnc, const DataInjection &camera_params, ImageData &image_data) {
    // init
    expo_time = camera_params.get_expo_time();

    // read params
    double roll, pitch, yaw;
    yaw = gnc.get_yaw();
    roll = gnc.get_roll();
    pitch = gnc.get_pitch();

    GVector3 sun_loc(gnc.get_x_sun(), gnc.get_y_sun(), gnc.get_z_sun());
    GVector3 cam_loc(gnc.get_x(), gnc.get_y(), gnc.get_z());

    // init sun light
    GVector3 light_dire(-sun_loc.x, -sun_loc.y, -sun_loc.z);
    light_dire.normalize();
    rotate_point(light_dire, gnc.get_angle());
    light = DirectLight(Color::white().multiply(0.2), light_dire, true);

    // rotating the camera
    rotate_point(cam_loc, gnc.get_angle());
    init_camera_coordinate(cam_loc, gnc.get_angle());

    // setting camera
    set_camera_params(cam_loc);
    camera.rotateByEuler(roll, pitch, yaw);
    // run it
    taking_photo_by_direct_light(image_data);
}

void CameraModel::taking_photo_by_direct_light(ImageData &image_data) {
    vector<std::thread> ths;
    int diff = IMAGE_WIDTH_GENERATE / NTHREAD;
    for (int i = 0; i < NTHREAD; ++i) {
        ths.emplace_back(taking_photo_by_direct_light_helper, this, std::ref(image_data), i * diff, min(i*diff + diff, IMAGE_WIDTH_GENERATE));
    }
    std::for_each(ths.begin(), ths.end(),  std::mem_fn(&std::thread::join));
}

void CameraModel::rotate_point(GVector3 &point, double w) {
    double roh = sqrt(point.x * point.x + point.y * point.y);
    double theta = atan2(point.y, point.x) - w;
    point.x = roh * sin(theta);
    point.y = roh * cos(theta);
}

void CameraModel::init_camera_coordinate(GVector3 location, double w) {
    GVector3 up(0, 0, 1);
    z_axis = location.normalize() * -1;
    x_axis = z_axis.crossMul(up);
    x_axis = x_axis.normalize();
    y_axis = x_axis.crossMul(z_axis);
    y_axis = y_axis.normalize();
}


