//
// Created by vhdsih on 2020/1/27.
//

#include "GncShower.h"


GncShower::GncShower() : new_x(-1), new_y(-1), param_scale(1), param_offset_x(0), param_offset_y(0) {
    wxInitAllImageHandlers();

    mars_image.LoadFile(DEFAULT_MARS_IMAGE);
    camera_image.LoadFile(DEFAULT_CAMERA_IMAGE);
    image = wxImage(CANVAS_W, CANVAS_H);

    init();
    draw_mars(image);
    image_ret = image.Copy();
}

void GncShower::init() {
    mars_x = MARS_X;
    mars_y = MARS_Y;
    mars_w_h = MARS_SIZE;

    param_scale = double(MARS_RADIUS) / mars_w_h;

    param_offset_x = CANVAS_W / 2.0;
    param_offset_y = CANVAS_H / 2.0;

    translation(mars_x, mars_y);
}

wxImage GncShower::add(double _x, double _y, double _z) { // use km, not m
    empty = false; // is the first point to draw?

    // calculate location -- km
    _x /= 1000.0f;
    _y /= 1000.0f;
    _z /= 1000.0f;
    // calculate angle and direction
    if (fabs(_x -last_xyz.x) < 1 && fabs(_y - last_xyz.y) < 1 && fabs(_z - last_xyz.z) < 1) {
        printf("same point!\n");
        return image_ret;
    }
    auto point = GVector3(_x, _y, _z);
    if (!lengthes.empty()) {
        auto _angle = acos(last_xyz.dotMul(point) / (last_xyz.getLength() * point.getLength()));
        auto _is_up = last_xyz.crossMul(point).dotMul(GVector3(0, 0, 1)) > 0;
        if (std::isnan(_angle)) {
            _angle = 0;
        }
        if (_is_up) {
            angle_offset += _angle;
        } else {
            angle_offset -= _angle;
        }
    }
    last_xyz = point;

    std::cout << "angle offset " << angle_offset << std::endl;
    angles.emplace_back(angle_offset);
    lengthes.emplace_back(point.getLength());

    // limit the size of points
    pop();

    // try to draw
    if (!try_draw()) {
        // recalculate and then draw
        redraw();
    }
    return image_ret;
}

void GncShower::pop() {
    if (lengthes.size() > POINT_MAX_SIZE * 2) {
        angles = std::vector<double>(angles.begin() + POINT_MAX_SIZE, angles.end());
        lengthes = std::vector<double>(lengthes.begin() + POINT_MAX_SIZE, lengthes.end());
    }
}

void GncShower::redraw() {
    image = wxImage(CANVAS_W, CANVAS_H); // clear image

    get_scale_params(); // calculate params

    // draw mars
    mars_x = 0;
    mars_y = 0;
    translation(mars_x, mars_y);
    draw_mars(image);

    // draw points
    int _x, _y;
    for (int i = 1; i < lengthes.size() - 1; ++i) {
        _x = lengthes[i] * cos(angles[i]);
        _y = lengthes[i] * sin(angles[i]);
        translation(_x, _y);
        if (_x > 1970 && _x < 2030 || _y > 1970 && _y < 2030) continue;
        draw_point(image, _x, _y);
    }

    // draw camera on copyed image
    image_ret = image.Copy();
    new_x = lengthes[lengthes.size() - 1] * cos(angles[lengthes.size() - 1]);
    new_y = lengthes[lengthes.size() - 1] * sin(angles[lengthes.size() - 1]);
    translation(new_x, new_y);
    draw_camera(image_ret);
}

bool GncShower::try_draw() {
    new_x = lengthes[lengthes.size() - 1] * cos(angles[lengthes.size() - 1]);
    new_y = lengthes[lengthes.size() - 1] * sin(angles[lengthes.size() - 1]);

    translation(new_x, new_y);

    if (is_legal(new_x, new_y)) {
        // draw last point
        if (have_last) {
            draw_point(image, last_x, last_y);
        } else {
            have_last = true;
        }
        last_x = new_x;
        last_y = new_y;
        image_ret = image.Copy();

        // draw camera
        draw_camera(image_ret);
        return true;
    }
    return false;
}

void GncShower::translation(int &_x, int &_y) {
    // scale and move
    _x = double(_x) / param_scale + param_offset_x;
    _y = double(-_y) / param_scale + param_offset_y;
}

bool GncShower::is_legal(int _x, int _y) {
    return (_x > 0 && _x < CANVAS_W && _y > 0 && _y < CANVAS_H);
}

void GncShower::get_scale_params() {
    // calculate scale param
    auto xy_max_base = std::max_element(lengthes.begin(), lengthes.end());

    param_scale = *xy_max_base / (CANVAS_H / 2.0 - EDGE);

    // calculate the size of mars
    mars_w_h = std::max(int(MARS_RADIUS / param_scale), 50);
}


void GncShower::draw_mars(wxImage &_image) {
    _image.Paste(mars_image.Scale(mars_w_h, mars_w_h), mars_x - mars_w_h/2, mars_y - mars_w_h/2);
}

void GncShower::draw_camera(wxImage &_image) {
    if (empty) return;

    _image.Paste(camera_image.Scale(100, 100), new_x - 50, new_y - 50);
}

void GncShower::draw_point(wxImage &_image, int _x, int _y) {
    for (int i = std::max(0, _x - CONTEXT); i <= std::min(CANVAS_W - 1, _x + CONTEXT); ++i) {
        for (int j = std::max(0, _y - CONTEXT); j <= std::min(CANVAS_H - 1, _y + CONTEXT); ++j) {
            u_char gray = 255 * light_rate(_x, _y, i, j);
            _image.SetRGB(i, j, gray, 0, 0);
        }
    }
}