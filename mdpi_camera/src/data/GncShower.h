//
// Created by vhdsih on 2020/1/27.
//

#ifndef MDPI_CAMERA_GNCSHOWER_H
#define MDPI_CAMERA_GNCSHOWER_H

#include <vector>
#include <cmath>
#include <wx/wx.h>

#include "../utils/gvector3.h"
#include "../value/const_values.h"

class GncShower {
public:
    GncShower();

    wxImage add(double _x, double _y, double _z);
    inline wxImage get_ret() {
        return image_ret;
    }

private:
    void init();
    void pop();
    void redraw();
    bool try_draw();
    void translation(int &_x, int &_y);
    bool is_legal(int _x, int _y);
    void get_scale_params();

    void draw_mars(wxImage &_image);
    void draw_camera(wxImage &_image);
    void draw_point(wxImage &_image, int _x, int _y);

    inline double light_rate(double mu1, double mu2, int x, int y) {
        double a = (x - mu1) * (x - mu1) / (SIGMA1 * SIGMA1);
        double b = (y - mu2) * (y - mu2) / (SIGMA2 * SIGMA2);
        return exp(-0.5 * (a + b));
    }

private:
    const int EDGE = 80;
    const int CONTEXT = 8;
    const int CANVAS_W = 4000;
    const int CANVAS_H = 4000;
    const int POINT_MAX_SIZE = 200;
    const double SIGMA1 = 4.0;
    const double SIGMA2 = 4.0;
    const int MARS_X = 0, MARS_Y = 0;
    const int MARS_RADIUS = 3396, MARS_SIZE = 300;

    bool empty = true, have_last = false;

    wxImage image, image_ret, mars_image, camera_image;
    GVector3 last_xyz; // last raw point
    int last_x = -1, last_y = -1;  // last point to draw

    int new_x = 0, new_y = 0; // new point location: camera location
    int mars_x = 0, mars_y = 0, mars_w_h = 300; // mars_location and its size
    double param_scale, param_offset_x, param_offset_y; // scale params

    double angle_offset = 0;
    std::vector<double> angles;
    std::vector<double> lengthes;
};


#endif //MDPI_CAMERA_GNCSHOWER_H
