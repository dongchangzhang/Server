//
// Created by z on 9/24/18.
//


#include <sys/time.h>
#include <fstream>
#include <sstream>
#include <thread>
#include <cmath>
#include "CameraModel.h"

void set_pixel(ImageData &canvas, int pixel_x, int pixel_y, wxImage &image, int x, int y) {
    unsigned char r = 0, g = 0, b = 0, new_r = 0, new_g = 0, new_b = 0;
    auto is_read = canvas.get_pixel(pixel_x, pixel_y, new_r, new_g, new_b);

    if (!is_read) {
        return;
    } else if (new_r == 0 && new_g == 0 && new_b == 0) {
        r = image.GetRed(x, y);
        g = image.GetGreen(x, y);
        b = image.GetBlue(x, y);
        canvas.set_pixel(pixel_x, pixel_y, r, g, b);
    } else {
        r = image.GetRed(x, y);
        g = image.GetGreen(x, y);
        b = image.GetBlue(x, y);
        new_r = static_cast<char>(new_r * 0.5 + r * 0.5);
        new_g = static_cast<char>(new_g * 0.5 + g * 0.5);
        new_b = static_cast<char>(new_b * 0.5 + b * 0.5);
        canvas.set_pixel(pixel_x, pixel_y, new_r, new_g, new_b);
    }
}

// helper for multithread
void draw_image_helper(CameraModel *cameraModel, wxImage &image, ImageData &canvas, int from, int to) {
    float sy, sx;
    double pixel_x, pixel_y;
    float scale_x = cameraModel->camera.get_scale_h();
    float scale_y = cameraModel->camera.get_scale_v();
    float dx = scale_x / DEFAULT_PHOTO_W;
    float dy = scale_y / DEFAULT_PHOTO_H;

    bool readed;
    int x1, x2, y1, y2;
    unsigned char r, g, b;

    for (int y = 0; y < DEFAULT_PHOTO_H; ++y) {
        sy = scale_y - dy * y;
        for (int x = from; x < to; ++x) {
            sx = dx * x;

            r = image.GetRed(x, y);
            g = image.GetGreen(x, y);
            b = image.GetBlue(x, y);

            if ((r == 0 && g == 0 && b == 0)) {
                continue;
            }
            CRay ray(cameraModel->camera.generateRay(sx, sy));
            IntersectResult result = cameraModel->sphere_model->isIntersected(ray);
            if (result.isHit) {
                float p_x = result.position.x;
                float p_y = result.position.y;
                float p_z = result.position.z;
                if (p_x == 0 && p_y == 0 && p_z == 0) {
                    continue;
                }
                cameraModel->transform->GetPixelLocByXYZ(p_x, p_y, p_z, pixel_x, pixel_y);
//                canvas.set_pixel(pixel_x, pixel_y, r, g, b);

                x1 = int(pixel_x) % CANVAS_W;
                x2 = (int(pixel_x) + 1) % CANVAS_W;
                y1 = int(pixel_y) % CANVAS_H;
                y2 = (int(pixel_y) + 1) % CANVAS_H;

                set_pixel(canvas, x1, y1, image, x, y);
                set_pixel(canvas, x2, y1, image, x, y);
                set_pixel(canvas, x1, y2, image, x, y);
                set_pixel(canvas, x2, y2, image, x, y);
            }
        }
    }
}

// implements for CameraModel from here
CameraModel::CameraModel(std::string _file) {
    transform = new Transform();
    sphere_model = new CSphere(mars_origin, transform->getRadius());


}


void CameraModel::set_camera_params(GVector3 location) {
    init_camera_coordinate(location);
    camera = perspectiveCamera(location, z_axis, y_axis, x_axis, camera_fov_hor, camera_fov_ver, camera_f_value);
}


// the implement for the job
void CameraModel::run_camera(const GNC &gnc, const DataInjection &camera_params, wxImage &image, ImageData &canvas) {
    // read params
    double roll, pitch, yaw;
    GVector3 cam_loc;

    gnc.get_loc_of_camera(cam_loc);
    gnc.get_euler(roll, pitch, yaw);

    camera_fov_hor = gnc.get_fov_h();
    camera_fov_ver = gnc.get_fov_v();
    camera_f_value = gnc.get_f_value();

    // rotating
    rotate_point(cam_loc, gnc.get_angle());
    // setting camera
    set_camera_params(cam_loc);
    camera.rotateByEuler(roll, pitch, yaw);
    // run it
    draw_image(image, canvas);
}

void CameraModel::draw_image(wxImage &image, ImageData &canvas) {
    vector<std::thread> ths;

    int diff = DEFAULT_PHOTO_W / NTHREAD;
    for (int i = 0; i < NTHREAD; ++i) {
        ths.emplace_back(draw_image_helper, this, std::ref(image), std::ref(canvas), i * diff, min(i*diff + diff, DEFAULT_PHOTO_W));
    }
    std::for_each(ths.begin(), ths.end(),  std::mem_fn(&std::thread::join));
}


void CameraModel::rotate_point(GVector3 &point, double w) {
    double roh = sqrt(point.x * point.x + point.y * point.y);
    double theta = atan2(point.y, point.x) - w;
    point.x = roh * sin(theta);
    point.y = roh * cos(theta);
}

void CameraModel::init_camera_coordinate(GVector3 location) {
    GVector3 up(0, 0, 0);
    if (abs(location.x) <= 1e-3 && abs(location.y) <= 1e-3) {
        up = GVector3(1, 1, 0);
    } else {
        up = GVector3(0, 0, 1);
    }
    z_axis = location.normalize() * -1;


    x_axis = z_axis.crossMul(up);
    x_axis = x_axis.normalize();

    y_axis = x_axis.crossMul(z_axis);
    y_axis = y_axis.normalize();
}




